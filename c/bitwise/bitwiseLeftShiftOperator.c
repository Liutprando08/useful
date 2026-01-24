#include <stdio.h>
// utilizzo bitwise left shift operator
int main(void) {

  int a = 4;
  int b = 5, c;
  int EXIT_SUCCESS = 0;
  // questo calcolo sposta i bit di a di quanto specifica bitwise
  //  sarebbe come fare a*2^b
  c = a << b;
  printf("%d", c);
  return EXIT_SUCCESS;
}
