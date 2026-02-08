#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILTER_SIZE_BITS 1024
#define NUM_HASHES 3

typedef struct {
  uint8_t *bits;
  size_t size;
} BloomFilter;

void set_bits(BloomFilter *bf, size_t n) { bf->bits[n / 8] |= (1 << (n % 8)); }

bool check_bit(BloomFilter *bf, size_t n) {
  return (bf->bits[n / 8] & (1 << (n % 8))) != 0;
}

size_t hash(const char *str, int seed) {
  size_t hash = 5381 + seed;

  return hash % FILTER_SIZE_BITS;
}
