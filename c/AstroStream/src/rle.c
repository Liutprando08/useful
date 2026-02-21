#include "rle.h"
#include "TLVdecoding.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint32_t decompression(data *sensorData) {
  uint32_t val = 0;

  for (int i = 0; i + 1 < ARR;) {

    uint8_t rep = sensorData->value[i++];
    uint8_t valore = sensorData->value[i++];
    for (int u = 0; u < rep; u++) {
      val = (val * 10) + valore;
    }
  }
  return val;
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

  sdata->value[i] = '\n';
  return sdata;
}
