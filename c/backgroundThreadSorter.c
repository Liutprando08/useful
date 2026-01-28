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

pthread_mutex_t lock;

int areaCamp(void *r1, void *r2) {
  struct rect tr1 = *(struct rect *)r1;
  struct rect tr2 = *(struct rect *)r2;

  int area1 = tr1.breadth * tr1.length;
  int area2 = tr2.breadth * tr2.length;
  return area1 > area2 ? 1 : 0;
}

void sorter(void *p) {
  for (;;) {

    pthread_mutex_lock(&lock);
    qsort(r, RECT_SIZE, sizeof(struct rect), areaCamp);
    printf("The rect array is\n");
    for (int i = 0; i < RECT_SIZE; i++) {
      printf("%d %d\n", r[i].length, r[i].breadth);
    }
  }
}
