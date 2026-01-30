#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 32
#define POOL_BLOCKS 10
#define POOL_SIZE (BLOCK_SIZE * POOL_BLOCKS)

uint8_t memory_pool[POOL_SIZE];
void *free_list_head;

void pool_init() {
  free_list_head = &memory_pool[0];
  for (int i = 0; i < POOL_BLOCKS; i++) {
    uint8_t *current = &memory_pool[i * BLOCK_SIZE];
    uint8_t *next = &memory_pool[(i + 1) * BLOCK_SIZE];
    *(void **)current = next;
  }
  *(void **)&memory_pool[(POOL_BLOCKS - 1) * BLOCK_SIZE] = NULL;
  printf("indirizzo base; %p", (void *)memory_pool);
}

int main(int argc, char *argv[]) {
  pool_init();
  return EXIT_SUCCESS;
}
