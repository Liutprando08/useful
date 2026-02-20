#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H
#include <stdint.h>
#define BLOCK_SIZE 32
#define POOL_BLOCK 10
#define POOL_SIZE (BLOCK_SIZE * POOL_BLOCK)

void pool_init();
void *my_alloc();
void my_free(void *ptr);

#endif // !MEMORYALLOCATOR_H
