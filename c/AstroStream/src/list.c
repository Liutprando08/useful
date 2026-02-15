#include "list.h"
#include <stdint.h>

void add_list(struct list_head *nuovo, struct list_head *testa) {
  struct list_head *vecchio_prossimo = testa->next;
  nuovo->next = vecchio_prossimo;
  nuovo->prev = testa;
  testa->next = nuovo;
  vecchio_prossimo->prev = nuovo;
}
