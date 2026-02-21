#include "TLVdecoding.h"
#include "bloomFilter.h"
#include "fixedPoint.h"
#include "list.h"
#include "memoryAllocator.h"
#include "rle.h"
#include "tpool.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  printf("Test Programma\n");
  size_t begin = 0;
  uint8_t *buffer = serialize((uint8_t)1, (uint32_t)8765342, begin);

  return EXIT_SUCCESS;
}
