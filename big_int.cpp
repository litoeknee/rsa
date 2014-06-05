#include "big_int.h"
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <cstdio>
#include <cstring>

size_t Max(size_t a, size_t b) {
  return (a < b) ? b : a;
}

size_t FloorDivide(size_t a, size_t b) {
  return (a + b - 1) / b;
}

BigInt::BigInt(): data_(new uint32_t[1]), length_(1) {
  memset(data_, 0, length_ * sizeof(uint32_t));
}

BigInt::BigInt(size_t s): data_(new uint32_t[s]), length_(s) {
  memset(data_, 0, length_ * sizeof(uint32_t));
}

BigInt::~BigInt() {
  delete[] data_;
}

BigInt::BigInt(const BigInt& b): data_(new uint32_t[b.length_]), length_(b.length_) {
  memcpy(data_, b.data_, length_ * sizeof(uint32_t));
}

BigInt& BigInt::operator=(const BigInt& b) {
  if (&b == this) {
    return *this;
  }
  Resize(b.length_);
  memcpy(data_, b.data_, length_ * sizeof(uint32_t));
  return *this;
}

BigInt BigInt::operator+(const BigInt& b) {
  BigInt ret(Max(length_, b.length_) + 1);
  for (size_t i = 0; i < ret.length_; ++i) {
    uint64_t tmp = (uint64_t) data_[i] + b.data_[i];
    ret.data_[i] += tmp & 0xFFFFFFFF;
    ret.data_[i + 1] = tmp >> 32;
  }
  ret.Shrink();
  return ret;
}

void BigInt::Set(const char* str) {
  size_t len = strlen(str);
  char tmp[9];
  tmp[8] = 0;
  Resize(FloorDivide(len, 8));
  for (size_t i = 0; i < length_ - 1; ++i) {
    strncpy(tmp, &str[len - (i + 1) * 8], 8);
    data_[i] = strtol(tmp, 0, 16);
  }
  memset(tmp, 0, sizeof(tmp));
  strncpy(tmp, str, len - (length_ - 1) * 8);
  data_[length_ - 1] = strtol(tmp, 0, 16);
}

void BigInt::Print() {
  for (int i = length_ - 1; 0 <= i; --i) {
    printf("%08X", data_[i]);
  }
  printf("\n");
}

void BigInt::Resize(size_t s) {
  delete[] data_;
  length_ = s;
  data_ = new uint32_t[length_];
  memset(data_, 0, length_ * sizeof(uint32_t));
}

void BigInt::Shrink() {
  while (data_[length_ - 1] == 0) {
    --length_;
  }
  if (!length_) {
    ++length_;
  }
}

