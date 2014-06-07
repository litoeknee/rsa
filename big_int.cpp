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
  for (auto i = data_.rbegin(); i != data_.rend(); ++i) {
    printf("%08X", *i);
  }
  printf("\n");
}

inline size_t Max(size_t a, size_t b) {
  return (a < b) ? b : a;
}

uint64_t At(const vector<uint32_t>& v, size_t pos) {
  if (v.size() <= pos) {
    return 0;
  }
  return v[pos];
}

void AbsAdd(vector<uint32_t>& res, const vector<uint32_t>& a, const vector<uint32_t>& b) {
  size_t length = Max(a.size(), b.size()) + 1;
  uint64_t carry = 0;
  res.resize(length);
  for (size_t i = 0; i < length; ++i) {
    carry += At(a, i) + At(b, i);
    res[i] = carry & 0xFFFFFFFF;
    carry >>= 32;
  }
  Shrink(res);
}

void AbsSub(vector<uint32_t>& res, const vector<uint32_t>& a, const vector<uint32_t>& b) {
  if (AbsCompare(a, b) == 1) {
    AbsSub(res, b, a);
  }
  vector<uint32_t> neg = b, comp;
  while (neg.size() < a.size()) {
    neg.push_back(0);
  }
  AbsBitwiseNot(neg);
  AbsAdd(comp, neg, vector<uint32_t>{1});
  comp.resize(a.size());
  AbsAdd(res, comp, a);
  if (res.size()) {
    res.pop_back();
  }
}

int AbsCompare(const vector<uint32_t>& a, const vector<uint32_t>& b) {
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

void AbsBitwiseNot(std::vector<uint32_t>& a) {
  for (auto& i: a) {
    i = ~i;
  }
}

void Shrink(vector<uint32_t>& v) {
  while (v.size() && !v.back()) {
    v.pop_back();
  }
}

