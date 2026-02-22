#include "rle.h"
#include "TLVdecoding.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char *decompression(data *sensorData) {

  size_t total_length = 0;
  size_t num_runs = strlen(sensorData->value);

  for (size_t i = 0; i < num_runs; i++) {
    total_length += sensorData->compress[i];
  }

  char *result = (char *)malloc(total_length + 1);
  if (result == NULL)
    return NULL;
  size_t current_pos = 0;
  for (size_t i = 0; i < num_runs; i++) {
    char character = sensorData->value[i];
    uint8_t count = sensorData->compress[i];

    for (uint8_t j = 0; j < count; j++) {
      result[current_pos++] = character;
    }
  }

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
