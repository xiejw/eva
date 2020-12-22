#ifndef SRNG64_H_
#define SRNG64_H_

#include <stdint.h>

// The implementation is based on "Fast Splittable Pseudorandom Number
// Generators.
typedef struct {
  uint64_t seed_;
  uint64_t gamma_;
  uint64_t next_gamma_seed_;
} srng64_t;

extern srng64_t* srng64New(uint64_t seed);
extern srng64_t* srng64NewWithGamma(uint64_t seed, uint64_t gamma_seed);
extern void      srng64Free(srng64_t* prng);

extern srng64_t* srng64Split(srng64_t* prng);

extern uint64_t srng64NextUint64(srng64_t* prng);
extern uint32_t srng64NextUint32(srng64_t* prng);
extern double   srng64NextDouble(srng64_t* prng);

#endif
