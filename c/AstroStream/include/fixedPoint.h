#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

#include <stdint.h>
typedef uint32_t Fixed_t;
#define SHIFT_AMOUNT 16
#define FIXED_ONE (1 << SHIFT_AMOUNT)
#define INT_TO_FIXED(x) ((x) << SHIFT_AMOUNT)
#define FIXED_TO_INT(x) ((x) >> SHIFT_AMOUNT)
void fp_print(Fixed_t n);
Fixed_t fp_add(Fixed_t a, Fixed_t b);
Fixed_t fp_sub(Fixed_t a, Fixed_t b);
Fixed_t fp_mul(Fixed_t a, Fixed_t b);
Fixed_t fp_div(Fixed_t num, Fixed_t den);

#endif
