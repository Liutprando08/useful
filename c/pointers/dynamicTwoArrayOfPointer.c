#include <stdio.h>
#include <stdlib.h>

int main() {
  int r = 3, c = 4, i, j, count;
  // creazione array di pointers
  int *arr[r];
  // assegnazione array dentro array
  for (i = 0; i < r; i++) {
    arr[i] = (int *)malloc(c * sizeof(int));
  }
  count = 0;
  for (i = 0; i < r; i++) {
    for (j = 0; j < c; j++) {
      *(*arr + i * r + j) = ++count;
    }
  }
  // stampa
  for (i = 0; i < r; i++) {
    for (j = 0; j < c; j++) {
      printf("%d\n", *(*arr + i * r + j));
    }
  }
  return EXIT_SUCCESS;
}
