#include "big_int.h"
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

BigInt::BigInt(): sign_(false) {
}

BigInt::BigInt(uint32_t d): sign_(false) {
  if (d) {
    data_.push_back(d);
  }
}

BigInt::BigInt(const char* s): sign_(false) {
  FromString(s);
}

BigInt::BigInt(const BigInt& b): sign_(b.sign_), data_(b.data_) {
}

BigInt::~BigInt() {
}

BigInt& BigInt::operator=(const BigInt& b) {
  if (&b == this) {
    return *this;
  }
  data_ = b.data_;
  sign_ = b.sign_;
  return *this;
}

BigInt BigInt::operator+(const BigInt& b) const {
  BigInt ret;
  if (sign_ == b.sign_) {
    ret.sign_ = sign_;
    AbsAdd(ret.data_, data_, b.data_);
  } else {
    if (AbsCompare(data_, b.data_) <= 0) {
      ret.sign_ = sign_;
      AbsSub(ret.data_, data_, b.data_);
    } else {
      ret.sign_ = b.sign_;
      AbsSub(ret.data_, b.data_, data_);
    }
  }
  return ret;
}

BigInt BigInt::operator-(const BigInt& b) const {
  BigInt ret(b);
  ret.sign_ = !ret.sign_;
  return *this + ret;
}

BigInt BigInt::operator*(const BigInt& b) const {
  BigInt ret;
  ret.sign_ = !(sign_ == b.sign_);
  AbsMult(ret.data_, data_, b.data_);
  return ret;
}

BigInt BigInt::operator%(const BigInt& b) const {
  BigInt ret(*this), divider(b);
  if (ret < divider) {
    return ret;
  }
  size_t shift = ret.Bits() - divider.Bits();
  divider = divider << shift;
  while (shift) {
    if (divider <= ret) {
      ret = ret - divider;
    }
    divider = divider >> 1;
    --shift;
  }
  if (divider <= ret) {
    ret = ret - divider;
  }
  return ret;
}

BigInt BigInt::operator<<(size_t s) const {
  BigInt ret(*this);
  size_t whole = s / 32;
  ret.data_.insert(ret.data_.begin(), whole, 0);
  ret.data_.push_back(0);
  Axpy(ret.data_, 1 << (s % 32), Slice{0}, 0);
  Shrink(ret.data_);
  return ret;
}

BigInt BigInt::operator>>(size_t s) const {
  BigInt ret(*this);
  size_t whole = s / 32;
  if (ret.data_.size() <= whole) {
    ret.data_.clear();
    return ret;
  }
  if (whole) {
    ret.data_.erase(ret.data_.begin(), ret.data_.begin() + whole);
  }
  uint32_t shift = 0, tmp;
  uint32_t remain_shift = s % 32;
  uint32_t shift_to_high = 32 - remain_shift;
  uint32_t mask_low = (1 << remain_shift) - 1;
  for (int i = ret.data_.size() - 1; 0 <= i; --i) {
    tmp = ret.data_[i] & mask_low;
    ret.data_[i] = (ret.data_[i] >> remain_shift) | shift;
    shift = tmp << shift_to_high;
  }
  Shrink(ret.data_);
  return ret;
}

bool BigInt::operator<(const BigInt& b) const {
  if (sign_ != b.sign_) {
    return sign_;
  }
  if (data_.size() < b.data_.size()) {
    return !sign_;
  }
  if (b.data_.size() < data_.size()) {
    return sign_;
  }
  int com = AbsCompare(data_, b.data_);
  switch (com) {
    case 1:
      return !sign_;
    case -1:
      return sign_;
    default:
      return false;
  }
}

bool BigInt::operator>(const BigInt& b) const {
  if (sign_ != b.sign_) {
    return !sign_;
  }
  if (data_.size() < b.data_.size()) {
    return sign_;
  }
  if (b.data_.size() < data_.size()) {
    return !sign_;
  }
  int com = AbsCompare(data_, b.data_);
  switch (com) {
    case 1:
      return sign_;
    case -1:
      return !sign_;
    default:
      return false;
  }
  return false;
}

bool BigInt::operator<=(const BigInt& b) const {
  return !(b < *this);
}

bool BigInt::operator>=(const BigInt& b) const {
  return !(*this < b);
}

bool BigInt::operator==(const BigInt& b) const {
  if (sign_ != b.sign_ || data_.size() != b.data_.size()) {
    return false;
  }
  for (size_t i = data_.size() - 1; 0 <= i; --i) {
    if (data_[i] != b.data_[i]) {
      return false;
    }
  }
  return true;
}

bool BigInt::operator!=(const BigInt& b) const {
  return !(*this == b);
}

bool BigInt::BitAt(size_t s) const {
  return (At(data_, s / 32) >> (s % 32)) & 0x01;
}

size_t BigInt::Bits() const {
  if (!data_.size()) {
    return 0;
  }
  size_t r = 0;
  uint32_t high = data_.back();
  while (high) {
    high >>= 1;
    ++r;
  }
  r += 32 * (data_.size() - 1);
  return r;
}

BigInt BigInt::MontgomeryMult(const BigInt& a, const BigInt& m, size_t n) {
  // m must be odd
  BigInt c = *this * a, adder = m;
  for (size_t i = 0; i < n; ++i) {
    if (c.BitAt(i)) {
      c = c + adder;
    }
    adder = adder << 1;
  }
  c = c >> n;
  c = c % m;
  return c;
}

BigInt BigInt::MontgomeryExp(const BigInt& e, const BigInt& m) {
  // m must be greater than 0
  size_t r_bits = 0;
  uint32_t high = m.data_.back();
  while (high) {
    high >>= 1;
    ++r_bits;
  }
  r_bits += 32 * (m.data_.size() - 1);
  BigInt r("1");
  r = r << (2 * r_bits);
  BigInt base = this->MontgomeryMult(r, m, r_bits);
  BigInt ret = BigInt(1).MontgomeryMult(r, m, r_bits);
  for (int i = e.Bits() - 1; 0 <= i; --i) {
    printf("got %d\n", i);
    ret = ret.MontgomeryMult(ret, m, r_bits);
    if (e.BitAt(i)) {
      ret = ret.MontgomeryMult(base, m, r_bits);
    }
  }
  return ret.MontgomeryMult(BigInt(1), m, r_bits);
}

void BigInt::FromString(const char* str) {
  size_t len = strlen(str);
  size_t length = (len + 7) / 8;
  char tmp[9];
  tmp[8] = 0;
  data_.clear();
  for (size_t i = 0; i < length - 1; ++i) {
    strncpy(tmp, &str[len - (i + 1) * 8], 8);
    data_.push_back(strtol(tmp, 0, 16));
  }
  memset(tmp, 0, sizeof(tmp));
  strncpy(tmp, str, len - (length - 1) * 8);
  data_.push_back(strtol(tmp, 0, 16));
  Shrink(data_);
}

void BigInt::Print() {
  if (sign_) {
    printf("-");
  }
  if (!data_.size()) {
    printf("%08X", 0);
  } else {
    for (auto i = data_.rbegin(); i != data_.rend(); ++i) {
      printf("%08X", *i);
    }
  }
  printf("\n");
}

inline size_t Max(size_t a, size_t b) {
  return (a < b) ? b : a;
}

uint64_t At(const Slice& v, size_t pos) {
  if (v.size() <= pos) {
    return 0;
  }
  return v[pos];
}

void AbsAdd(Slice& res, const Slice& a, const Slice& b) {
  res = a;
  res.resize(Max(a.size(), b.size()) + 1);
  Axpy(res, 1, b, 0);
  Shrink(res);
}

void AbsSub(Slice& res, const Slice& a, const Slice& b) {
  if (AbsCompare(a, b) == 1) {
    AbsSub(res, b, a);
  }
  Slice neg = b, comp;
  neg.resize(a.size(), 0);
  AbsBitwiseNot(neg);
  AbsAdd(comp, neg, Slice{1});
  res = a;
  Axpy(res, 1, comp, 0);
  Shrink(res);
}

void AbsMult(Slice& res, const Slice& a, const Slice& b) {
  Slice tmp;
  size_t size = a.size() + b.size();
  res.clear();
  res.resize(size, 0);
  for (size_t i = 0; i < b.size(); ++i) {
    tmp = a;
    tmp.resize(size, 0);
    Axpy(tmp, b[i], Slice{0}, 0);
    Axpy(res, 1, tmp, i);
  }
  Shrink(res);
}

int AbsCompare(const Slice& a, const Slice& b) {
  if (a.size() < b.size()) {
    return 1;
  }
  if (b.size() < a.size()) {
    return -1;
  }
  for (size_t i = a.size() - 1; 0 <= i; --i) {
    if (a[i] < b[i]) {
      return 1;
    }
    if (b[i] < a[i]) {
      return -1;
    }
  }
  return 0;
}

void AbsBitwiseNot(Slice& a) {
  for (auto& i: a) {
    i = ~i;
  }
}

void Shrink(Slice& v) {
  while (v.size() && !v.back()) {
    v.pop_back();
  }
}

// Compute x = a * x + y
void Axpy(Slice& x, uint32_t a, const Slice& y, size_t base) {
  size_t length = x.size();
  uint64_t carry = 0;
  for (size_t i = base; i < length; ++i) {
    carry += a * At(x, i) + At(y, i - base);
    x[i] = carry & 0xFFFFFFFF;
    carry >>= 32;
  }
}

