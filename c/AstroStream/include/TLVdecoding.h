#ifndef TLVDECODING_H
#define TLVDECODING_H

#include <stddef.h>
#include <stdint.h>
typedef enum { UINT8_T = 1, UINT32_T = 4, ARR } dataType;
typedef struct {
  uint8_t id;
  uint32_t value;
  uint8_t index[ARR];
} data;

data *deserialize(uint8_t *buffer, size_t size);

#endif // !TLVDECODING_H
