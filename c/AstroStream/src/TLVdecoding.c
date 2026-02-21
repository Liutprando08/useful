#include "TLVdecoding.h"
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
      memcpy(&sensorData->id, &buffer[i], type);
    }

    else if (tag == 0x02) {
      sensorData->value = (char *)malloc(type + 1);
      memcpy(sensorData->value, &buffer[i], type);
      sensorData->value[type] = '\0';
    } else if (tag == 0x03) {
      sensorData->compress = (uint8_t *)malloc(type);
      memcpy(sensorData->compress, &buffer[i], type);
    }
    i += type;
  }
  return sensorData;
}

uint8_t *serialize(data *sdata, size_t *size) {
  size_t val_len = strlen(sdata->value);
  size_t comp_len = val_len;
  size_t total_size = 1 + 1 + 1 + (2 + val_len) + (2 + comp_len);
  size_t begin = 0;
  uint8_t *buffer = (uint8_t *)malloc(total_size);
  buffer[begin++] = 0x01;
  buffer[begin++] = UINT8_T;
  buffer[begin++] = sdata->id;
  buffer[begin++] = 0x02;
  buffer[begin++] = (uint8_t)val_len;
  memcpy(&buffer[begin], sdata->value, val_len);
  begin += val_len;
  buffer[begin++] = 0x03;
  buffer[begin++] = (uint8_t)comp_len;
  memcpy(&buffer[begin], sdata->compress, comp_len);
  begin += comp_len;
  *size = begin;
  return buffer;
}
