#include "big_int.h"
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <cstring>

BigInt::BigInt(): data_(new uint32_t[1]), length_(1) {
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

void BigInt::Resize(size_t s) {
  delete[] data_;
  length_ = s;
  data_ = new uint32_t[length_];
  memset(data_, 0, length_ * sizeof(uint32_t));
}

