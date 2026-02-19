
#include "bloomFilter.h"
#include "TLVdecoding.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static inline uint32_t rotl32(uint32_t x, int8_t r) {
  return (x << r) | (x >> (32 - r));
}

bool check_bit(bloomFilter *bf, size_t n) {
  return (bf->bits[n / 8] & (1 << (n % 8))) != 0;
}
void set_bit(bloomFilter *bf, size_t n) { bf->bits[n / 8] |= (1 << (n % 8)); }
uint32_t fnv1a(data *sd, size_t len) {
  uint8_t *temp = &sd->id;
  uint32_t hash = 0x811c9dc5;
  for (size_t i = 0; i < len; i++) {
    hash ^= temp[i];
    hash *= 0x01000193;
  }
  return hash;
}

uint32_t murmurhash(data *sd, size_t len, uint32_t seed) {
  int nblocks = len / 4;
  uint32_t h1 = seed;
  uint8_t *temp = (uint8_t *)&sd->id;
  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;

  uint32_t *block = (uint32_t *)(temp + nblocks * 4);
  for (int i = -nblocks; i; i++) {
    uint32_t k1 = block[i];

    k1 *= c1;
    k1 = rotl32(k1, 15);
    k1 *= c2;

    h1 ^= k1;
    h1 = rotl32(h1, 13);
    h1 = h1 * 5 + 0xe6546b64;
  }
  const uint8_t *tail = (const uint8_t *)(temp + nblocks * 4);
  uint32_t k1 = 0;

  switch (len & 3) {
  case 3:
    k1 ^= tail[2] << 16;
  case 2:
    k1 ^= tail[1] << 8;
  case 1:
    k1 ^= tail[0];
    k1 *= c1;
    k1 = rotl32(k1, 15);
    k1 *= c2;
    h1 ^= k1;
  }
  h1 ^= len;

  h1 ^= h1 >> 16;
  h1 *= 0x85ebca6b;
  h1 ^= h1 >> 13;
  h1 *= 0xc2b2ae35;
  h1 ^= h1 >> 16;

  return h1;
}

void bloom_add(bloomFilter *bf, data *sd) {
  uint32_t h1 = murmurhash(sd, sizeof(sd->id), 0);
  uint32_t h2 = fnv1a(sd, sizeof(sd->id));
  for (uint8_t i = 0; i < NUM_HASHES; i++) {
    uint32_t combinedHash = h1 + (i * h2);
    size_t bit_pos = combinedHash % bf->size;
    set_bit(bf, bit_pos);
  }
}

bool bloom_check(bloomFilter *bf, data *sd) {
  uint32_t h1 = murmurhash(sd, sizeof(sd->id), 0);
  uint32_t h2 = fnv1a(sd, sizeof(sd->id));
  for (uint8_t i = 0; i < NUM_HASHES; i++) {
    uint32_t combinedHash = h1 + (i * h2);
    size_t bit_pos = combinedHash % bf->size;
    if (!check_bit(bf, bit_pos)) {
      return false;
    }
  }
  return true;
}
bloomFilter *create() {
  bloomFilter *bf = (bloomFilter *)malloc(sizeof(bloomFilter));
  uint8_t bytes_needed = (FILTER_SIZE_BITS + 7) / 8;
  bf->bits = calloc(bytes_needed, sizeof(uint8_t));
  bf->size = FILTER_SIZE_BITS;
  return bf;
}
