#ifndef SRNG64_H_
#define SRNG64_H_

#include <stdint.h>

// Based on "Fast Splittable Pseudorandom Number Generators.
struct srng64_t {
        uint64_t seed_;
        uint64_t gamma_;
        uint64_t next_gamma_seed_;
};

extern struct srng64_t* srng64New(uint64_t seed);
extern struct srng64_t* srng64NewWithGamma(uint64_t seed, uint64_t gamma_seed);
extern void             srng64Free(struct srng64_t* prng);

extern struct srng64_t* srng64Split(struct srng64_t* prng);

extern uint64_t srng64NextUint64(struct srng64_t* prng);
extern uint32_t srng64NextUint32(struct srng64_t* prng);
extern double   srng64NextDouble(struct srng64_t* prng);

#endif
