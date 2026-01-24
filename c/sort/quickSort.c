#include <stdio.h>
void quicksort(int array[], int low, int high);
int partizione(int array[], int low, int high);
void swap(int *a, int *b);
void stampa(int array[], int size);
int main() {
  int n = 10;
  int array[n];
  // stampa array non ordinato
  printf("array non ordinato \n");
  stampa(array, n);
  quicksort(array, 0, n - 1);
  // stampa array ordinato
  printf("array ordinato \n");
  stampa(array, n);
}

// cambia i 2 elementi usando in puntatori di memoria
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}
/*
    Funzione principale del QuickSort
    array[] --> Array da ordinare
    low     --> Indice iniziale
    high    --> Indice finale
*/
void quicksort(int array[], int low, int high) {
  if (low < high) {
    // pi e l'indice di partizionamento
    //  dopo questa chiamata array[pi] e nella posizione corretta
    int pi = partizione(array, low, high);
    // funzione ricorsiva per elementi prima del pivot
    quicksort(array, low, pi - 1);
    // funzione ricorsiva per elementi dopo pivot
    quicksort(array, pi + 1, high);
  }
}
/*
 * Questa funzione prende l' ultimo elemento come pivot,
 * lo posiziona nella parte corretta e posiziona gli elementi
 * piu piccoli a sinistra e quelli piu grandi a destra.
 */

int partizione(int array[], int low, int high) {
  // scelta pivot
  int pivot = array[high];

  // indica l'indice dell'elemento piu piccolo
  // indicala posizione dell'ultimo numero trovato che e minore del pivot
  int i = (low - 1);
  // Ciclo principale scorre l'array dall'inizio alla fine(escluso il pivot)
  for (int j = low; j < high; j++) {

    // se e minore del pivot
    if (array[j] < pivot) {
      i++;                        // incrementa indice
      swap(&array[i], &array[j]); // scambia
    }
  }
  // alla fine posizioan il pivot nella posizione corretta
  swap(&array[i + 1], &array[high]);
  // ritorna l'indice dove si trova il pivot
  return (i + 1);
}

void stampa(int array[], int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", array[i]);
  }
}
