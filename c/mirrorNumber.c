#include <math.h>
#include <stdio.h>

int main(void) {
  int num, square, resto1, resto2, specchio1, specchio2, quadrato, radice;
  int EXIT_SUCCESS = 0;
  specchio1 = 0;
  specchio2 = 0;

  printf("scrivi il numero per vedere se si puo invertire");
  scanf("%d", &num);
  // calcolo non necessario imparare usare math.h
  quadrato = pow(num, 2);
  while (quadrato != 0) {
    resto1 = quadrato % 10;
    specchio1 = specchio1 * 10 + resto1;
    quadrato = quadrato / 10;
  }
  // calcolo per far tornare normale
  radice = sqrt(specchio1);

  while (radice != 0) {
    resto2 = radice % 10;
    specchio2 = specchio2 * 10 + resto2;
    radice = radice / 10;
  }
  if (specchio2 == num) {
    printf("e uno specchio");
  } else {
    printf("non e uno specchio");
  }
  return EXIT_SUCCESS;
}
