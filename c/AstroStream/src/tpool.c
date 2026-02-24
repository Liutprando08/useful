#include "tpool.h"
#include <pthread.h>
#include <stdlib.h>
void *tpool_worker(void *tpool) {
  tpool_t *pool = (tpool_t *)tpool;
  while (1) {
    pthread_mutex_lock(&(pool->lock));
    while ((pool->count == 0) && (!pool->shutdown)) {
      pthread_cond_wait(&(pool->notify), &(pool->lock));
    }
    if (pool->shutdown && pool->count == 0) {
      pthread_mutex_unlock(&(pool->lock));
      pthread_exit(NULL);
    }
    tpool_task_t task = pool->queue[pool->head];
    pool->head = (pool->head + 1) % pool->queue_size;
    pool->count--;
    pthread_mutex_unlock(&(pool->lock));
    (*(task.function))(task.argument);
  }
  return NULL;
}
tpool_t *threadpool_create(int thread_count, int queue_size) {
  tpool_t *pool = (tpool_t *)malloc(sizeof(tpool_t));
  pool->thread_count = thread_count;
  pool->queue_size = queue_size;
  pool->head = pool->tail = pool->count = pool->shutdown = 0;
  pool->thread = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
  pool->queue = (tpool_task_t *)malloc(sizeof(tpool_task_t) * queue_size);
  pthread_mutex_init(&(pool->lock), NULL);
  pthread_cond_init(&(pool->notify), NULL);
  for (int i = 0; i < thread_count; i++) {
    pthread_create(&(pool->thread[i]), NULL, tpool_worker, (void *)pool);
  }
  return pool;
}
int threadpool_Add(tpool_t *pool, void (*func)(void *), void *arg) {
  pthread_mutex_lock(&(pool->lock));
  if (pool->count == pool->queue_size) {
    pthread_mutex_unlock(&(pool->lock));
    return -1;
  }
  pool->queue[pool->tail].function = func;
  pool->queue[pool->tail].argument = arg;
  pool->tail = (pool->tail + 1) % pool->queue_size;
  pool->count++;
  pthread_cond_signal(&(pool->notify));
  pthread_mutex_unlock(&(pool->lock));
  return 0;
}
int threadpool_destroy(tpool_t *pool) {
  if (pool == NULL) {
    return -1;
  }
  pthread_mutex_lock(&(pool->lock));
  if (pool->shutdown) {
    pthread_mutex_unlock(&(pool->lock));
    return -1;
  }
  pool->shutdown = 1;
  pthread_cond_broadcast(&(pool->notify));
  pthread_mutex_unlock(&(pool->lock));
  for (int i = 0; i < pool->thread_count; i++) {
    pthread_join(pool->thread[i], NULL);
  }
  free(pool->thread);
  free(pool->queue);
  pthread_mutex_destroy(&(pool->lock));
  pthread_cond_destroy(&(pool->notify));
  free(pool);
  return 0;
}
