#include <stdio.h>
#include <stdlib.h>
// definire una macro per preprocesso
#define limit 100

int main(int argc, char *argv[]) {
  unsigned long long int i, j;
  int *primes;
  int z = 1;
  // creazione array tipo void
  primes = malloc(sizeof(int) * limit);

  for (i = 2; i < limit; i++) {
    primes[i] = 1;
  }
  // si utilizza il crivello che implica un metodo di esclusione
  for (i = 2; i < limit; i++) {
    if (primes[i]) {
      for (j = i; i * j < limit; j++) {
        primes[i * j] = 0;
      }
    }
  }
  printf("\nPrime numbers in range 1 to 100 are: \n");
  for (i = 2; i < limit; i++) {
    if (primes[i])
      printf("%llu\n", i);
  }

  return EXIT_SUCCESS;
}
