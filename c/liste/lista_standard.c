#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct contatto {
  char nome[50];
  char numero[20];
  struct contatto *indice;
};

struct contatto *aggiungi(char *nome, char *numero, struct contatto *indice) {
  struct contatto *nuovo = malloc(sizeof(struct contatto));
  strcpy(nuovo->nome, nome);
  strcpy(nuovo->numero, numero);
  nuovo->indice = indice;

  return nuovo;
}
void stampa(struct contatto *stam) {
  struct contatto *corrente = stam;

  while (corrente != NULL) {
    printf("Nome: %s, Tel: %s\n", corrente->nome, corrente->numero);
    corrente = corrente->indice;
  }
}
int main(int argc, char *argv[]) {

  struct contatto *rubrica = NULL;
  rubrica = aggiungi("Pippo", "321345312", rubrica);
  rubrica = aggiungi("gigio", "567899990", rubrica);
  stampa(rubrica);
  return EXIT_SUCCESS;
}
