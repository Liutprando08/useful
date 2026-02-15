#include "memoryAllocator.h"
#include <stdint.h>
#include <stdlib.h>
uint32_t memory_pool[POOL_SIZE];
void *free_list_head;

// creation allocator pool
void pool_init() {
  // assigning location free space
  free_list_head = &memory_pool[0];
  // creating list
  for (int i = 0; i < POOL_BLOCK; i++) {
    void *current = &memory_pool[i * BLOCK_SIZE];
    void *next = &memory_pool[(i + 1) * BLOCK_SIZE];
    *(void **)current = next;
  }
  // setting last point to pointer in NULL
  *(void **)&memory_pool[(POOL_BLOCK - 1) * BLOCK_SIZE] = NULL;
}
void *my_malloc() {
  // control if is out of memory
  if (free_list_head == NULL) {
    return NULL;
  }
  // you put ptr becomes free_list_head
  void *ptr = free_list_head;
  // free_list_head becomes next
  free_list_head = *(void **)ptr;
  return ptr;
}

void my_free(void *ptr) {
  if (ptr == NULL) {
    return;
  }
  // you put free_list_head inside ptr
  *(void **)ptr = free_list_head;
  // free_list_head becomes ptr
  free_list_head = ptr;
}
