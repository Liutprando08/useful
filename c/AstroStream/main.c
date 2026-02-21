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
  data *sensordata = compression("567890");
  size_t size;
  uint8_t *arr = serialize(sensordata, &size);
  data *sdata = deserialize(arr, size);
  char *valore = decompression(sdata);
  printf("%s", valore);

  return EXIT_SUCCESS;
}
