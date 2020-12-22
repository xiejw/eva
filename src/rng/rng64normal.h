#ifndef NORMAL_H_
#define NORMAL_H_

#include <stddef.h>

#include "rng64.h"

extern void rng64StdNormal(rng64t* rng, size_t size, double* buffer);

#endif
