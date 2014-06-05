#pragma once
#include <cstdint>
#include <cstddef>

class BigInt {
 public:
  BigInt();
  ~BigInt();
  BigInt(const BigInt&);
  BigInt& operator=(const BigInt&);
  BigInt operator+(const BigInt&);
  BigInt operator*(const BigInt&);
  BigInt Power(int);
  BigInt Inverse();
  BigInt Modulo(const BigInt&);

 private:
  void Resize(size_t);
  uint32_t* data_;
  size_t length_;
};

