#ifndef TLVDECODING_H
#define TLVDECODING_H

#include <stddef.h>
#include <stdint.h>
typedef enum { UINT8_T = 1, ARR = 4 } dataType;
typedef struct {
  uint8_t id;
  uint8_t value[ARR];
} data;

data *deserialize(uint8_t *buffer, size_t size);
uint8_t *serialize(uint8_t id, uint32_t value, size_t begin);
#endif // !TLVDECODING_H
