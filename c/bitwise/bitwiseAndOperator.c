#include <stdio.h>
// bitwise and operator
int main(void) {
  int a = 4, b = 5, c, EXIT_SUCCESS = 0;
  // confronta i bit dei 2 numeri ne seguente modo:
  // 1 & 1 = 1
  // 0 & 1 = 0
  // 1 & 0 = 0
  // 0 & 0 = 0
  c = a & b;
  printf("%d\n", c);
  return EXIT_SUCCESS;
}
