#include "TLVdecoding.h"
#include <stdint.h>

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
