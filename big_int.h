#pragma once
#include <cstdint>
#include <cstddef>

class BigInt {
 public:
  BigInt();
  BigInt(size_t);
  ~BigInt();
  BigInt(const BigInt&);
  BigInt& operator=(const BigInt&);
  BigInt operator+(const BigInt&);
  BigInt operator*(const BigInt&);
  void Set(const char*);
  void Print();
  BigInt Power(int);
  BigInt Inverse();
  BigInt Modulo(const BigInt&);

 private:
  void Resize(size_t);
  void Shrink();
  uint32_t* data_;
  size_t length_;
};

