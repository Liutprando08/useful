#include "rle.h"
#include "TLVdecoding.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char *decompression(data *sensorData) {

  // 1. Calcoliamo la lunghezza totale della stringa finale
  // Dobbiamo sommare tutti i valori contenuti nell'array 'compress'
  size_t total_length = 0;
  size_t num_runs =
      strlen(sensorData->value); // Quanti gruppi di caratteri abbiamo

  for (size_t i = 0; i < num_runs; i++) {
    total_length += sensorData->compress[i];
  }

  // 2. Allocazione memoria per la stringa decompressa
  // +1 per il terminatore di stringa '\0'
  char *result = (char *)malloc(total_length + 1);
  if (result == NULL)
    return NULL;

  // 3. Espansione dei dati
  size_t current_pos = 0;
  for (size_t i = 0; i < num_runs; i++) {
    char character = sensorData->value[i];
    uint8_t count = sensorData->compress[i];

    // Scriviamo il carattere 'character' per 'count' volte
    for (uint8_t j = 0; j < count; j++) {
      result[current_pos++] = character;
    }
  }

  // 4. Chiusura della stringa
  result[current_pos] = '\0';

  return result;
}

data *compression(char *num) {
  int c;
  int i = 0;
  int n = strlen(num);
  data *sdata = (data *)malloc(sizeof(data));
  sdata->compress = (uint8_t *)calloc(n, sizeof(uint8_t));
  sdata->value = (char *)malloc(n + 1);
  while ((c = *num)) {
    sdata->value[i] = *num;
    sdata->compress[i]++;
    num++;
    while (sdata->value[i] == *num) {
      sdata->compress[i]++;
      num++;
    }
    i++;
  }

  sdata->value[i] = '\0';
  return sdata;
}
