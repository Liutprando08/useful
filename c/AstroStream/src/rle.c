#include "TLVdecoding.h"
#include <stdint.h>

uint32_t decompression(data *sensorData) {
  uint8_t temp[10];
  int t_idx = 0;
  uint32_t val = 0;

  for (int i = 0; i + 1 < ARR;) {

    uint8_t rep = sensorData->value[i++];
    uint8_t valore = sensorData->value[i++];
    for (int u = 0; u < rep; u++) {
      temp[t_idx++] = valore;
    }
  }
  for (int t = 0; t < t_idx; t++) {

    val = (val * 10) + temp[t];
  }
  return val;
}
