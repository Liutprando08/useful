#include "memoryAllocator.h"
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
uint32_t memory_pool[POOL_SIZE];
void *free_list_head;
void pool_init() {
  free_list_head = &memory_pool[0];
  for (int i = 0; i < POOL_BLOCK; i++) {
    void *current = &memory_pool[i * BLOCK_SIZE];
    void *next = &memory_pool[(i + 1) * BLOCK_SIZE];
    *(void **)current = next;
  }
  *(void **)&memory_pool[(POOL_BLOCK - 1) * BLOCK_SIZE] = NULL;
}
void *my_malloc() {
  if (free_list_head == NULL) {
  }
  void *ptr = free_list_head;
  free_list_head = *(void **)ptr;
  return ptr;
}

void my_free(void *ptr) {
  *(void **)ptr = free_list_head;
  free_list_head = ptr;
}
