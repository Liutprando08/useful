#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  void (*function)(void *);
  void *argument;
} threadpool_task_t;

typedef struct {
  pthread_mutex_t lock;
  pthread_cond_t notify;
  pthread_t *thread;
  threadpool_task_t *queue;
  int thread_count;
  int queue_size;
  int head;
  int tail;
  int count;
  int shutdown;
} threadpool_t;

static void *threadpool_worker(void *threadpool) {
  threadpool_t *pool = (threadpool_t *)threadpool;
  while (1) {
    pthread_mutex_lock(&(pool->lock));
    while ((pool->count == 0) && (!pool->shutdown)) {
      pthread_cond_wait(&(pool->notify), &(pool->lock));
    }
    if (pool->shutdown) {
      pthread_mutex_unlock(&(pool->lock));
      pthread_exit(NULL);
    }
    threadpool_task_t task = pool->queue[pool->head];
    pool->head = (pool->head + 1) % pool->queue_size;
    pool->count--;
    pthread_mutex_unlock(&(pool->lock));
    (*(task.function))(task.argument);
  }
  return NULL;
}

threadpool_t *threadpool_create(int thread_count, int queue_size) {
  threadpool_t *pool = (threadpool_t *)malloc(sizeof(threadpool_t));
  pool->thread_count = thread_count;
  pool->queue_size = queue_size;
  pool->head = pool->tail = pool->count = pool->shutdown = 0;
  pool->thread = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
  pool->queue =
      (threadpool_task_t *)malloc(sizeof(threadpool_task_t) * queue_size);
  pthread_mutex_init(&(pool->lock), NULL);
  pthread_cond_init(&(pool->notify), NULL);
  for (int i = 0; i < thread_count; i++) {
    pthread_create(&(pool->thread[i]), NULL, threadpool_worker, (void *)pool);
  }
  return pool;
}

int threadpool_add(threadpool_t *pool, void (*func)(void *), void *arg) {
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

int threadpool_destroy(threadpool_t *pool) {
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

void simula_lavoro_pesante(void *arg) {
  int *num = (int *)arg;
  printf("[Worker %lu] inizia task %d \n", pthread_self(), *num);

  sleep(1);
  printf("[worker %lu] finito task %d \n", pthread_self(), *num);
  free(arg);
}

int main(int argc, char *argv[]) {
  printf("crea 4 worker\n");

  threadpool_t *pool = threadpool_create(4, 10);
  printf("aggiungi 10 task \n");
  for (int i = 0; i < 10; i++) {
    int *arg = malloc(sizeof(int));
    *arg = i;

    if (threadpool_add(pool, simula_lavoro_pesante, arg) != 0) {
      printf("Errore: coda piena epr il task %d\n", i);
      free(arg);
    }
  }
  printf("tutti i task sottomessi, il main thread attende 5 secondi \n");
  sleep(5);
  printf("Distruzione del pool e uscita \n");
  threadpool_destroy(pool);
  printf("fatto");
  return EXIT_SUCCESS;
}
