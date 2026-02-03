#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define container_of(ptr, type, member)                                        \
  ((type *)((char *)(ptr) - offsetof(type, member)))

//  creazione struct list_head
struct list_head {
  struct list_head *prev, *next;
};
struct contatto {
  char nome[50];
  struct list_head nodo;
};

void aggiungi(struct list_head *nuovo, struct list_head *testa) {
  struct list_head *vecchio_prossimo = testa->next;
  nuovo->next = vecchio_prossimo;
  nuovo->prev = testa;
  vecchio_prossimo->prev = nuovo;
  testa->next = nuovo;
}

int main(void) {
  struct list_head rubrica_testa = {&rubrica_testa, &rubrica_testa};
  struct contatto *c1 = malloc(sizeof(struct contatto));
  strcpy(c1->nome, "pippo");
  aggiungi(&c1->nodo, &rubrica_testa);

  struct list_head *ptr = rubrica_testa.next;
  struct contatto *recuperato = container_of(ptr, struct contatto, nodo);
  printf("Nome recuperato: %s\n", recuperato->nome);
  printf("Nome recuperato: %s\n", recuperato->nome);
  return EXIT_SUCCESS;
}
