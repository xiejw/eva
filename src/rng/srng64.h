#ifndef SRNG64_H_
#define SRNG64_H_

#include <stdint.h>

// Based on "Fast Splittable Pseudorandom Number Generators.
struct srng64_t {
        uint64_t seed_;
        uint64_t gamma_;
        uint64_t next_gamma_seed_;
};

// Used for generate numbers. Cast srng64_t* to rng64_t* for use.
struct rng64_t {
        uint64_t seed_;
        uint64_t gamma_;
};

extern struct srng64_t *srng64New(uint64_t seed);
extern struct srng64_t *srng64NewWithGamma(uint64_t seed, uint64_t gamma_seed);
extern struct srng64_t *srng64Split(struct srng64_t *srng);
extern void             srng64Free(struct srng64_t *srng);

extern uint64_t rng64NextUint64(struct rng64_t *rng);
extern uint32_t rng64NextUint32(struct rng64_t *rng);
extern double   rng64NextDouble(struct rng64_t *rng);
extern float    rng64NextFloat(struct rng64_t *rng);
extern void     rng64Free(struct rng64_t *rng);

#endif
