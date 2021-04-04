#ifndef SRNG64_NORMAL_H_
#define SRNG64_NORMAL_H_

#include <stddef.h>

#include "rng/srng64.h"

extern void srng64StdNormalD(struct srng64_t* rng, size_t size, double* buffer);
extern void srng64StdNormalF(struct srng64_t* rng, size_t size, float* buffer);

#endif
