#ifndef SRNG64_NORMAL_H_
#define SRNG64_NORMAL_H_

#include <stddef.h>

#include "rng/srng64.h"

extern void rng64StdNormalD(struct rng64_t* rng, size_t size, double* buffer);
extern void rng64StdNormalF(struct rng64_t* rng, size_t size, float* buffer);

#endif
