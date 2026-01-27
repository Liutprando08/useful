#include <stdio.h>
#include <stdlib.h>

struct studente {
  char nome[50];
  int roll;
  float marks;

} s;

int main(int argc, char *argv[]) {
  printf("inserisci un nome: ");
  scanf("%s", s.nome);
  printf("inserisci un voto: ");
  scanf("%f", &s.marks);
  printf("numero di matricola: ");
  scanf("%d", &s.roll);
  printf("Name: ");
  puts(s.nome);

  printf("numero di matricola: %d\n", s.roll);

  printf("voto: %.1f\n", s.marks);

  return EXIT_SUCCESS;
}
