#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t fixed_t;

#define SHIFT_AMOUNT 16
#define FIXED_ONE (1 << SHIFT_AMOUNT)

#define INT_TO_FIXED(x) ((x) << SHIFT_AMOUNT)
#define FIXED_TO_INT(x) ((x) >> SHIFT_AMOUNT)

fixed_t fp_add(fixed_t a, fixed_t b) { return a + b; }

fixed_t fp_sub(fixed_t a, fixed_t b) { return a - b; }

fixed_t fp_mul(fixed_t a, fixed_t b) {
  int64_t temp = (int64_t)a * (int64_t)b;
  return (fixed_t)(temp >> SHIFT_AMOUNT);
}

fixed_t fp_div(fixed_t num, fixed_t den) {
  if (den == 0) {
    return 0;
  }
  int64_t temp = ((int64_t)num << SHIFT_AMOUNT);
  return (fixed_t)(temp / den);
}

void fp_print(fixed_t n) {
  int32_t intero = FIXED_TO_INT(n);
  int32_t fraz_raw = n & 0xFFFF;
  int32_t fraz_dec = (fraz_raw * 10000) >> 16;

  if (n < 0 && intero == 0) {
    printf("-0.%04d\n", fraz_dec);
  } else {
    printf("%d.%04d\n", intero, abs(fraz_dec));
  }
}
int main(int argc, char *argv[]) {
  fixed_t pi = 205887;
  fixed_t r = INT_TO_FIXED(3);

  fixed_t r_quadro = fp_mul(r, r);
  fixed_t area = fp_mul(pi, r_quadro);
  printf("Raggio: ");
  fp_print(r);
  return EXIT_SUCCESS;
}
