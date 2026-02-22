#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include "TLVdecoding.h"
#include <stdint.h>
typedef struct {
  uint8_t id;
  uint32_t value;
} processeddata;
processeddata *process(data *sdata, char *valore);

#endif // !PROCESSDATA_H
