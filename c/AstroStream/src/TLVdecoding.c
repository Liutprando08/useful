#include "TLVdecoding.h"
#include "memoryAllocator.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
data *deserialize(uint8_t *buffer, size_t size) {
  data *sensorData = my_alloc();
  for (size_t i = 0; i < size; i++) {
    if (buffer[i] == 0x01) {

      sensorData->id = buffer[i + 2];
    }
    if (buffer[i] == 0x02) {
      memcpy(&sensorData->value, &buffer[i + 2], 4);
      sensorData->value = ntohl(sensorData->value);
    }
    i += 2;
  }
  return sensorData;
}
