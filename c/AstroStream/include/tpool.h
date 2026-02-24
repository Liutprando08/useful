#ifndef TPOOL_H
#define TPOOL_H
#include <pthread.h>
typedef struct {
  void (*function)(void *);
  void *argument;
} tpool_task_t;

typedef struct {
  pthread_mutex_t lock;
  pthread_cond_t notify;
  pthread_t *thread;
  tpool_task_t *queue;
  int thread_count;
  int queue_size;
  int head;
  int tail;
  int count;
  int shutdown;

} tpool_t;
tpool_t *threadpool_create(int thread_count, int queue_size);
int threadpool_Add(tpool_t *pool, void (*func)(void *), void *arg);
int threadpool_destroy(tpool_t *pool);
#endif // !TPOOL_H
