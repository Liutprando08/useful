#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  // dichiarazione pointer
  int *pc;
  // dichiarazione variabile
  int c;

  c = 22;
  // qua stampiamo l indirizzo di memoria di c
  // '&' specifica di usare il pointer e
  // (void *) permette la portabilita del programma
  printf("address of c: %p\n", (void *)&c);
  printf("value of c: %d\n\n", c);

  // stiamo assegnando l indirizzo di c in pc
  pc = &c;
  // stampiamo indirizzo di tramite pc
  printf("Address of c: %p\n", (void *)pc);
  // stampiamo valore di c
  printf("Value of c: %d\n\n", c);

  c = 11;
  printf("Address of c: %p\n", (void *)pc);
  // stampiamo lo stesso c e '*' davanti a un pointer dice di
  // mostrare contenuto dell indirizzo
  printf("content of c: %d\n\n", *pc);

  // usando '*' riempiamo l indirizzo di memoria
  *pc = 2;
  printf("Address of c: %p\n", (void *)&c);
  printf("Value of c: %d\n\n", c);
  return EXIT_SUCCESS;
}
