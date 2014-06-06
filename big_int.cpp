#include "big_int.h"
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <cstdio>
#include <cstring>

inline size_t Max(size_t a, size_t b) {
  return (a < b) ? b : a;
}

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

BigInt BigInt::operator+(const BigInt& b) {
  BigInt ret;
  size_t length = Max(data_.size(), b.data_.size()) + 1;
  uint64_t carry = 0;
  ret.data_.resize(length);
  for (size_t i = 0; i < length; ++i) {
    carry += At(i) + b.At(i);
    ret.data_[i] = carry & 0xFFFFFFFF;
    carry >>= 32;
  }
  ret.Shrink();
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
  for (size_t i = data_.size() - 1; 0 <= i; --i) {
    if (data_[i] < b.data_[i]) {
      return !sign_;
    }
    if (b.data_[i] < data_[i]) {
      return sign_;
    }
  }
  return false;
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
  for (size_t i = data_.size() - 1; 0 <= i; --i) {
    if (data_[i] < b.data_[i]) {
      return sign_;
    }
    if (b.data_[i] < data_[i]) {
      return !sign_;
    }
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
  Shrink();
}

void BigInt::Print() {
  for (auto i = data_.rbegin(); i != data_.rend(); ++i) {
    printf("%08X", *i);
  }
  printf("\n");
}

uint32_t BigInt::At(size_t pos) const {
  if (data_.size() <= pos) {
    return 0;
  }
  return data_[pos];
}

void BigInt::Shrink() {
  while (data_.size() && !data_.back()) {
    data_.pop_back();
  }
}

