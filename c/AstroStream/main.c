#include "TLVdecoding.h"
#include "bloomFilter.h"
#include "fixedPoint.h"
#include "list.h"
#include "memoryAllocator.h"
#include "processdata.h"
#include "rle.h"
#include "tpool.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("Test Programma\n");

  data *sensordata = compression("555544466");
  size_t size;
  uint8_t *arr = serialize(sensordata, &size);
  data *sdata = deserialize(arr, size);
  char *valore = decompression(sdata);
  processeddata *processed = process(sdata, valore);
  printf("%u \n", processed->value);
  bloomFilter *bf = create();
  bool temp = bloom_check(bf, processed);
  if (temp == false) {

    bloom_add(bf, processed);
  } else if (temp == true) {
    free(processed);
  }

  return EXIT_SUCCESS;
}
