#include <stdio.h>

int main() {
  int n = 100;
  int array[n], c, d, t;

  for (c = 1; c <= n - 1; c++) {
    d = c;
    while (d > 0 & array[d - 1] > array[d]) {
      t = array[d - 1];
      array[d - 1] = array[d];
      array[d] = t;
      d--;
    }
  }

  printf("array in ordine crescenti\n");

  for (c = 0; c < n; c++) {
    printf("%d ", array[c]);
  }

  return 0;
}
