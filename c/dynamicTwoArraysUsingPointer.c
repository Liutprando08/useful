#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int r = 3, c = 4;
  // creazione matroce tra i puntatori
  int *array = (int *)malloc(r * c * sizeof(int));
  int i, j, count = 0;

  for (i = 0; i < r; i++) {
    for (j = 0; j < c; j++) {
      // assegnare valore alla matrice sempre tramite puntatori
      // corrispinderebbe a una roba simile
      //  0 + 0*1 + 1 questo vuol dire che riempie la casella (0,1)
      *(array + i * c + j) = ++count;
    }
  }
  // stampa matrice
  for (i = 0; i < r; i++) {
    for (j = 0; j < c; j++) {
      printf("%d\n", *(array + i * c + j));
    }
  }
  return EXIT_SUCCESS;
}
