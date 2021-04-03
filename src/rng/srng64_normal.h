#ifndef SRNG64_NORMAL_H_
#define SRNG64_NORMAL_H_

#include <stddef.h>

#include "rng/srng64.h"

extern void srng64StdNormal(struct srng64_t* rng, size_t size, double* buffer);

#endif
