#include <stdio.h>
#include <stdlib.h>
// include la libreria posix thread e fondamentale per creare
// un thread e gestire l accesso
#include <pthread.h>
// include le definizioni standard unix qui serve per sleep
#include <unistd.h>

#define RECT_SIZE 5

struct rect {
  int length;
  int breadth;
};

struct rect r[RECT_SIZE];
// creazione variabile lock
pthread_mutex_t lock;

int areaCamp(const void *r1, const void *r2) {
  struct rect tr1 = *(struct rect *)r1;
  struct rect tr2 = *(struct rect *)r2;

  int area1 = tr1.breadth * tr1.length;
  int area2 = tr2.breadth * tr2.length;
  return area1 > area2 ? 1 : 0;
}

void *sorter(void *p) {
  for (;;) {

    pthread_mutex_lock(&lock);
    // puntatore all inizio dell array, numrero di membri, dimensione di ogni
    // singolo elemento, compareazione con areaCamp
    qsort(r, RECT_SIZE, sizeof(struct rect), areaCamp);
    printf("The rect array is\n");
    for (int i = 0; i < RECT_SIZE; i++) {
      printf("%d %d\n", r[i].length, r[i].breadth);
    }
    pthread_mutex_unlock(&lock);
    sleep(10);
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  pthread_t thread;
  int index = 0;

  r[0].length = 10;
  r[0].breadth = 2;

  r[1].length = 34;
  r[1].breadth = 23;

  r[2].length = 32;
  r[2].breadth = 3;

  r[3].length = 2;
  r[3].breadth = 45;

  r[4].length = 2;
  r[4].breadth = 12;
  // creazione semaforo e inserimento variabile lock con attributi predefiniti
  pthread_mutex_init(&lock, NULL);
  // creazione del thread attr predefiniti e inserimento funzione e variabile da
  // inserire;
  pthread_create(&thread, NULL, sorter, (void *)NULL);

  for (;;) {
    struct rect temp_rect;
    int length, breadth;

    scanf("%d %d", &length, &breadth);

    temp_rect.length = length;
    temp_rect.breadth = breadth;
    // blocco thread sorter
    pthread_mutex_lock(&lock);
    // blocco funzione sorter
    r[(index++) % RECT_SIZE] = temp_rect;
    pthread_mutex_unlock(&lock);
  }
  return EXIT_SUCCESS;
}
