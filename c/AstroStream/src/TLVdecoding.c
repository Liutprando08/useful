#include "TLVdecoding.h"
#include "memoryAllocator.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
data *deserialize(uint8_t *buffer, size_t size) {
  data *sensorData = my_alloc();

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
