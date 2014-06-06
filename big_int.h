#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>

class BigInt {
 public:
  BigInt();
  BigInt(const char*);
  BigInt(const BigInt&);
  ~BigInt();
  BigInt& operator=(const BigInt&);
  BigInt operator+(const BigInt&);
  BigInt operator-(const BigInt&);
  void FromString(const char*);
  void Print();

 private:
  void Shrink();
  uint32_t At(size_t) const;
  bool sign_;
  std::vector<uint32_t> data_;
};

size_t Max(size_t, size_t);

