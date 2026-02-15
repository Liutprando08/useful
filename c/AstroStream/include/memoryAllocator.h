#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H
#include <stdint.h>
#define BLOCK_SIZE 32
#define POOL_BLOCK 10
#define POOL_SIZE (BLOCK_SIZE * POOL_BLOCK)

extern uint32_t memory_pool[];

extern void *free_list_head;

void pool_init();
void *my_alloc();
void my_free(void *ptr);

#endif // !MEMORYALLOCATOR_H
