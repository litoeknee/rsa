#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>

typedef std::vector<uint32_t> Slice;
// TODO add in-place versions

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
  Slice data_;
};

size_t Max(size_t, size_t);
uint64_t At(const Slice&, size_t);
void AbsAdd(Slice&, const Slice&, const Slice&);
void AbsSub(Slice&, const Slice&, const Slice&);
void AbsMult(Slice&, const Slice&, const Slice&);
int AbsCompare(const Slice&, const Slice&);
void AbsBitwiseNot(Slice&);
void Shrink(Slice&);
void Axpy(Slice&, uint32_t, const Slice&, size_t);

