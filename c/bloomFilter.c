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
  int c;

  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % FILTER_SIZE_BITS;
}
void destroy(BloomFilter *bf) {
  free(bf->bits);
  free(bf);
}

void bloom_add(BloomFilter *bf, const char *str) {
  for (int i = 0; i < NUM_HASHES; i++) {
    size_t position = hash(str, i);
    set_bits(bf, position);
  }
}
bool bloom_check(BloomFilter *bf, const char *str) {
  for (int i = 0; i < NUM_HASHES; i++) {
    size_t position = hash(str, i);
    if (!check_bit(bf, position)) {
      return false;
    }
  }
  return true;
}

BloomFilter *create() {
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  size_t bytes_needed = (FILTER_SIZE_BITS + 7) / 8;
  bf->bits = (uint8_t *)calloc(bytes_needed, sizeof(uint8_t));
  bf->size = FILTER_SIZE_BITS;
  return bf;
}

int main(int argc, char *argv[]) {
  BloomFilter *bf = create();
  bloom_add(bf, "mela");

  if (bloom_check(bf, "mela")) {
    printf("si");

  } else {
    printf("no");
  }
  if (bloom_check(bf, "banana")) {
    printf("si");
  } else {
    printf("no");
  }
  return EXIT_SUCCESS;
}
