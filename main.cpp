#include "big_int.h"

int main() {
  BigInt a, b, c;
  a.Set("ABCDB768238238976478CBDBCB209338498723987CBDFACCABBCC");
  b.Set("BBCDE2232874BBCCDDAACCBBFEFEF2272312304213487698");
  c = a + b;
  c.Print();
  return 0;
}

