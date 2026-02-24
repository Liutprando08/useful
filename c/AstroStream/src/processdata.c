#include "processdata.h"
#include "TLVdecoding.h"
#include "memoryAllocator.h"
#include <stdint.h>
#include <stdlib.h>
processeddata *process(data *sdata, char *valore) {
  pool_init();
  processeddata *processed = (processeddata *)my_malloc();
  processed->id = sdata->id;
  char *endptr;
  processed->value = strtoul(valore, &endptr, 10);
  free(sdata->value);
  free(sdata->compress);
  free(valore);
  free(sdata);
  return processed;
}
