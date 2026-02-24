#ifndef LIST_H
#define LIST_H
#include <stddef.h>
#include <stdint.h>
#define containerof(ptr, type, member)                                         \
  ((type *)((char *)(ptr) - offsetof(type, member)))

struct list_head {

  struct list_head *prev, *next;
};
void add_list(struct list_head *nuovo, struct list_head *testa);
#endif // !LIST_H
