#include "TLVdecoding.h"
#include "bloomFilter.h"
#include "fixedPoint.h"
#include "list.h"
#include "memoryAllocator.h"
#include "rle.h"
#include "tpool.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  printf("Test Programma\n");
  size_t begin;
  uint8_t *buffer = serialize((uint8_t)1, (uint32_t)8765342, &begin);
  data *sensordata = deserialize(buffer, begin);
  uint32_t val = decompression(sensordata);
  printf("il valore e %" PRIu32 "\n", val);

  return EXIT_SUCCESS;
}
