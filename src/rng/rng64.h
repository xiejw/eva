#ifndef RNG64_H_
#define RNG64_H_

#include <stdint.h>

// The implementation is based on "Fast Splittable Pseudorandom Number
// Generators.
typedef struct {
  uint64_t seed_;
  uint64_t gamma_;
  uint64_t next_gamma_seed_;
} rng64t;

extern rng64t* rng64Create(uint64_t seed);
extern rng64t* rng64CreateWithGamma(uint64_t seed, uint64_t gamma_seed);
extern void    rng64Free(rng64t* prng);
extern rng64t* rng64Split(rng64t* prng);

extern uint64_t rng64NextInt64(rng64t* prng);
extern uint32_t rng64NextInt32(rng64t* prng);
extern double   rng64NextDouble(rng64t* prng);

#endif
