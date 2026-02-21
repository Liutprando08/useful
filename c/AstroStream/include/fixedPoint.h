#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

#include <stdint.h>
typedef uint32_t Fixed_t;
#define SHIFT_AMOUNT 16
#define FIXED_ONE (1 << SHIFT_AMOUNT)
#define INT_TO_FIXED(x) ((x) << SHIFT_AMOUNT)
#define FIXED_TO_INT(x) ((x) >> SHIFT_AMOUNT)

#endif
