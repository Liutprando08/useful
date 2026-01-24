#include <stdio.h>
// problema con bitwise and per vedere se un numero e pari o dispari
int main(void) {
  int a = 4, b = 0;
  int EXIT_SUCCESS = 0;
  a &= b;
  if (0 == a)
    printf("Even Number\n");
  else
    printf("Odd Number\n");
  return EXIT_SUCCESS;
}
