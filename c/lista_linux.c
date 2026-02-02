#include <stddef.h>
#include <stdio.h>

struct list_head {
  struct list_head *prev, *next;
};
struct contatto {
  char nome[50];
  struct list_head nodo;
};
