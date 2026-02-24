#include "TLVdecoding.h"
#include "bloomFilter.h"
#include "fixedPoint.h"
#include "list.h"
#include "memoryAllocator.h"
#include "processdata.h"
#include "rle.h"
#include "tpool.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct {
  data *sdata;
  char *valore;
  bloomFilter *bf;
  struct list_head *lista;
} process_args_t;
pthread_mutex_t bf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
void task_process_data(void *arg) {
  process_args_t *args = (process_args_t *)arg;
  printf("[TASK] Inizio elaborazione per valore: %s\n",
         args->valore ? args->valore : "NULL");

  processeddata *processed = process(args->sdata, args->valore);
  pthread_mutex_lock(&bf_mutex);
  bool gia_visto = bloom_check(args->bf, processed);
  if (!processed) {
    printf("[TASK] Errore: process ha restituito NULL\n");
    free(args);
    return;
  }
  if (!gia_visto) {
    bloom_add(args->bf, processed);
    pthread_mutex_unlock(&bf_mutex);
    pthread_mutex_lock(&list_mutex);
    add_list(&processed->nodo, args->lista);
    pthread_mutex_unlock(&list_mutex);

    printf("[Thread %ld] Inserito valore: %u\n", (long)pthread_self(),
           processed->value);
  } else {
    pthread_mutex_unlock(&bf_mutex);
    my_free(processed);
  }
  free(args);
}
void print_rubrica(struct list_head *head) {
  struct list_head *iter;
  printf("\n--- CONTENUTO RUBRICA FINALE ---\n");

  for (iter = head->next; iter != head; iter = iter->next) {

    processeddata *data = containerof(iter, processeddata, nodo);

    printf("ID Sensore: %u | Valore Elaborato: ", data->id);
    fp_print(data->value);
    printf("\n");
  }
  printf("--------------------------------\n");
}
void clear_rubrica(struct list_head *head) {
  struct list_head *iter = head->next;
  while (iter != head) {
    struct list_head *next = iter->next;
    processeddata *data = containerof(iter, processeddata, nodo);

    my_free(data);
    iter = next;
  }
  head->next = head->prev = head;
}
int main() {
  printf("Test Programma\n");
  tpool_t *pool = threadpool_create(4, 10);
  bloomFilter *bf = create();
  data *sensordata = compression("555544466");
  size_t size;
  struct list_head rubrica_testa = {&rubrica_testa, &rubrica_testa};

  uint8_t *arr = serialize(sensordata, &size);
  free(sensordata->value);
  free(sensordata->compress);
  free(sensordata);
  data *sdata = deserialize(arr, size);
  free(arr);
  char *valore = decompression(sdata);
  process_args_t *args = malloc(sizeof(process_args_t));
  args->sdata = sdata;
  args->valore = valore;
  args->bf = bf;
  args->lista = &rubrica_testa;
  threadpool_Add(pool, task_process_data, args);
  threadpool_destroy(pool);

  print_rubrica(&rubrica_testa);
  clear_rubrica(&rubrica_testa);
  free(bf->bits);
  free(bf);
  return EXIT_SUCCESS;
}
