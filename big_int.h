#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>

typedef std::vector<uint32_t> Slice;

class BigInt {
 public:
  BigInt();
  BigInt(uint32_t);
  BigInt(const char*);
  BigInt(const BigInt&);
  ~BigInt();
  BigInt& operator=(const BigInt&);
  BigInt operator+(const BigInt&) const;
  BigInt operator-(const BigInt&) const;
  BigInt operator*(const BigInt&) const;
  BigInt operator%(const BigInt&) const;
  BigInt operator<<(size_t) const;
  BigInt operator>>(size_t) const;
  bool operator<(const BigInt&) const;
  bool operator>(const BigInt&) const;
  bool operator<=(const BigInt&) const;
  bool operator>=(const BigInt&) const;
  bool operator==(const BigInt&) const;
  bool operator!=(const BigInt&) const;
  bool BitAt(size_t) const;
  size_t Bits() const;
  BigInt MontgomeryMult(const BigInt&, const BigInt&, size_t);
  BigInt MontgomeryExp(const BigInt&, const BigInt&);
  void FromString(const char*);
  void Print();

 private:
  bool sign_;
  std::vector<uint32_t> data_;
};

size_t Max(size_t, size_t);
uint64_t At(const std::vector<uint32_t>&, size_t);
void AbsAdd(std::vector<uint32_t>&, const std::vector<uint32_t>&, const std::vector<uint32_t>&);
void AbsSub(std::vector<uint32_t>&, const std::vector<uint32_t>&, const std::vector<uint32_t>&);
void AbsMult(Slice&, const Slice&, const Slice&);
int AbsCompare(const std::vector<uint32_t>&, const std::vector<uint32_t>&);
void AbsBitwiseNot(std::vector<uint32_t>&);
void Shrink(std::vector<uint32_t>&);
void Axpy(Slice&, uint32_t, const Slice&, size_t);

