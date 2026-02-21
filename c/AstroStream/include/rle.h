#ifndef RLE_H
#define RLE_H
#include "TLVdecoding.h"
#include <stdint.h>

uint32_t decompression(data *sensordata);
data *compression(char *num);
#endif // !RLE_H
