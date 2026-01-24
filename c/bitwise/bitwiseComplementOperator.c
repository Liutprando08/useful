#include <stdio.h>
// bitwise complement operator
int main(void) {
  int a = 4, b, EXIT_SUCCESS = 0;
  // inverte il valore dei bit
  // sarebbe come fare -(a+1)
  b = ~a;
  printf("%d\n", b);
  return EXIT_SUCCESS;
}
