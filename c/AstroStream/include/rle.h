#ifndef RLE_H
#define RLE_H
#include "TLVdecoding.h"
#include <stdint.h>

char *decompression(data *sensordata);
data *compression(char *num);
#endif // !RLE_H
