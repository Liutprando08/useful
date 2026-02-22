#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include "processdata.h"
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#define FILTER_SIZE_BITS 1024
#define NUM_HASHES 3

typedef struct {
  uint8_t *bits;
  size_t size;
} bloomFilter;
bloomFilter *create();
bool bloom_check(bloomFilter *bf, processeddata *sd);
void bloom_add(bloomFilter *bf, processeddata *sd);

#endif // !BLOOMFILTER_H
