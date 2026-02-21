#include "TLVdecoding.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
data *deserialize(uint8_t *buffer, size_t size) {
  data *sensorData = (data *)malloc(sizeof(data));

  size_t i = 0;
  while (i + 2 <= size) {
    uint8_t tag = buffer[i++];
    uint8_t type = buffer[i++];
    if (tag == 0x01) {
      if (type == UINT8_T && i < size) {
        sensorData->id = buffer[i++];
      } else if (type == ARR && i + ARR <= size) {
        i += ARR;
      }
    }

    else if (tag == 0x02) {
      if (type == ARR && i + ARR <= size) {
        memcpy(&sensorData->value, &buffer[i], ARR);
        i += ARR;
      } else if (type == UINT8_T && i < size) {
        i++;
      }
    }
  }
  return sensorData;
}

uint8_t *serialize(uint8_t id, uint32_t value, size_t begin) {
  uint8_t *buffer = (uint8_t *)malloc(id * value * sizeof(uint8_t));
  buffer[begin++] = 0x01;
  buffer[begin++] = UINT8_T;
  buffer[begin++] = id;
  buffer[begin++] = 0x02;
  buffer[begin++] = ARR;
  memcpy(&buffer[begin], &value, ARR);
  begin += ARR;
  return buffer;
}
