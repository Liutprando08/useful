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
      }
      if (type == UINT32_T && i + 4 <= size) {
        i += 4;
      }
    } else if (tag == 0x02) {
      if (type == UINT8_T && i < size) {
        i += 2;
      } else if (type == UINT32_T && i + 4 <= size) {

        memcpy(&sensorData->value, &buffer[i], 4);
        sensorData->value = ntohl(sensorData->value);
        i += 4;
      }
    }
  }
  return sensorData;
}
