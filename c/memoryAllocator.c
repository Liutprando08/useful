#include <assert.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
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
pthread_mutex_t lock;

void *pool_init(void *p) {
  pthread_mutex_lock(&lock);
  free_list_head = &memory_pool[0];
  for (int i = 0; i < POOL_BLOCKS - 1; i++) {
    void *current = &memory_pool[i * BLOCK_SIZE];
    void *next = &memory_pool[(i + 1) * BLOCK_SIZE];
    *(void **)current = next;
  }
  *(void **)&memory_pool[(POOL_BLOCKS - 1) * BLOCK_SIZE] = NULL;
  printf("indirizzo base; %p", (void *)memory_pool);
  pthread_mutex_unlock(&lock);
}
void *my_malloc(void *p) {
  if (free_list_head == NULL) {
    printf("Out of memory\n");

    return NULL;
  }
  void *ptr = free_list_head;
  free_list_head = *(void **)ptr;
  printf("liberato blocco a: %p\n", ptr);
  return ptr;
}
void my_free(void *ptr) {

  if (ptr == NULL) {
    return;
  }
  *(void **)ptr = free_list_head;
  free_list_head = ptr;
  printf("liberato blocco a: %p\n", ptr);
}
struct payload {
  int id;
  char data[20];
};

int main(int argc, char *argv[]) {
  void *result;
  pthread_t thread;
  pthread_mutex_init(&lock, NULL);
  pthread_create(&thread, NULL, pool_init, (void *)NULL);
  int status = pthread_create(&thread, NULL, my_malloc, (void *)NULL);
  pthread_join(thread, result);
  struct payload *p1 = (struct payload *)result;
  struct payload *p2 = (struct payload *)result;

  struct payload *p3 = (struct payload *)result;
  my_malloc(p2);
  pthread_mutex_lock(&lock);

  if (p1)
    p1->id = 100;
  if (p2)
    p2->id = 200;

  my_free(p2);
  pthread_mutex_unlock(&lock);
  struct payload *p4 = (struct payload *)result;
  pthread_mutex_lock(&lock);
  if (p4 == p2) {
    printf("test superato\n");

  } else {
    printf("test fallito\n");
  }
  pthread_mutex_unlock(&lock);
  return EXIT_SUCCESS;
}
