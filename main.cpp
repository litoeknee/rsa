#include "big_int.h"

int main() {
  BigInt c;
  BigInt a("ABCDB768238238976478CBDBCB209338498723987CBDFACCABBCC");
  BigInt b("BBCDE2232874BBCCDDAACCBBFEFEF2272312304213487698");
  // BigInt b("0");
  a.Print();
  b.Print();
  c = a + b;
  c.Print();
  c = a - b;
  c.Print();
  c = a * b;
  c.Print();
  BigInt d = a / b;
  d.Print();
  return 0;
}

