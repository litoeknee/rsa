#include "big_int.h"

int main() {
  BigInt c;
  BigInt a("ABCDB768238238976478CBDBCB209338498723987CBDFACCABBCC");
  BigInt b("BBCDE2232874BBCCDDAACCBBFEFEF2272312304213487698");
  a.Print();
  b.Print();
  c = a - b;
  c.Print();
  return 0;
}

