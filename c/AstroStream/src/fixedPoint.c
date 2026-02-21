#include "fixedPoint.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
Fixed_t fp_add(Fixed_t a, Fixed_t b) { return a + b; }
Fixed_t fp_sub(Fixed_t a, Fixed_t b) { return a - b; }

Fixed_t fp_mul(Fixed_t a, Fixed_t b) {
  int64_t temp = (int64_t)a * (int64_t)b;
  return (Fixed_t)(temp >> SHIFT_AMOUNT);
}

Fixed_t fp_div(Fixed_t num, Fixed_t den) {
  if (den == 0) {
    return 0;
  }
  int64_t temp = ((int64_t)num << SHIFT_AMOUNT);
  return (Fixed_t)(temp / den);
}

void fp_print(Fixed_t n) {
  int32_t intero = FIXED_TO_INT(n);
  int32_t fraz_raw = n & 0xFFFF;
  int32_t fraz_dec = (fraz_raw * 10000) >> 16;
  if (intero == 0) {
    printf("-0.%04d\n", fraz_dec);
  } else {
    printf("%d.%04d\n", intero, abs(fraz_dec));
  }
}
