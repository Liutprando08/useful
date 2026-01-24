#include <stdio.h>

int main() {
  // dichiarazione grandezza array

  int n = 100;

  // dichiarazione array, counter, decrementa, temporaneo
  int array[n], c, d, t;
  // ciclo principale che scorre array
  for (c = 1; c <= n - 1; c++) {
    // dare valore a decremento in modo tale
    // di fare comparezione con i precedenti
    d = c;
    // ciclo per comparazione
    while (d > 0 & array[d - 1] > array[d]) {
      // scambio dati se minore di precedenti
      t = array[d - 1];
      array[d - 1] = array[d];
      array[d] = t;
      d--;
    }
  }

  printf("array in ordine crescenti\n");
  // stampa dell'array
  for (c = 0; c < n; c++) {
    printf("%d ", array[c]);
  }

  return 0;
}
