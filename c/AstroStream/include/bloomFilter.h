#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <stddef.h>
#include <stdint.h>
#define FILTER_SIZE_BITS 1024
#define NUM_HASHES 3

typedef struct {
  uint8_t *bits;
  size_t size;
} bloomFilter;
#endif // !BLOOMFILTER_H
