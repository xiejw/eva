#include "rng64.h"

#include <assert.h>
#include <stdlib.h>

/*
 * The DotMix algorithrm.
 * ----------------------
 *
 * 1. At each level in the pedigee, the coefficient `gamma` is used to advance
 *    the seed.
 *
 * 2. The addition-modulo, for the Dot in DotMix, is based on
 *
 *        George = 2^64 + 13L.
 *
 *    This is implemented as `sprng64_update`. In particular,
 *
 *   a. Any new seed, after modulo, falls in [0, 2^64) is accepted.
 *   b. For [2^64, 2^64+13L), will try again. To avoid a loop, `gamma`, the
 *      coefficient, is constructed to be larger than 13L,
 *
 * 3. The Mix in DotMix is implemented in `sprng64_mix64`.
 *
 *
 * 2. To avoid a fixed-size coefficient table.
 * The coefficient to generate prng for coefficient.
 *
 *
 * Generating random coefficient.
 * ------------------------------
 * By using PRNG to generate coefficient for different level, a fixed table can
 * be void avoided. The algorithrm is still based on DotMix, but with a
 * length-1 pedigee.
 *
 * 1. The `gamma` coefficient for generating `gamma` is `gamma_gamma_`.
 *
 * 2. To improve the speed of `sprng64_update`, a small Prime number (for
 *    modulo) is used for generating `gamma`, called Percy. It is the
 *    `gamma_prime_` in the code below.
 *
 *    With that, the `sprng64_update` has high chance take the first branch.
 *
 * 3. This gamma seed is then mixed by `sprng64_mix56` to be _truely` random.
 *
 * 4. The generated `gamma` should be no smaller than 13L. See DotMix
 *    algorithrm.
 */

/* Cast of uint64_t is needed for 32-bit platform. */
static const uint64_t gamma_prime_ = (((uint64_t)1L) << 56) - 5; /* Percy. */
static const uint64_t gamma_gamma_ = 0x00281E2DBA6606F3L;
static const double   double_ulp_  = 1.0 / (((uint64_t)1L) << 53);

static uint64_t sprng64_update(uint64_t seed, uint64_t gamma) {
  uint64_t p = seed + gamma;
  return (p >= seed) ? p : (p >= 13L) ? p - 13L : (p - 13L) + gamma;
}

static uint64_t sprng64_mix64(uint64_t z) {
  z = ((z ^ (z >> 33)) * 0xff51afd7ed558ccdL);
  z = ((z ^ (z >> 33)) * 0xc4ceb9fe1a85ec53L);
  return z ^ (z >> 33);
}

static uint64_t sprng64_advance_seed(rng64t* prng) {
  /* Advance one more coefficient at current level. */
  return (prng->seed_ = sprng64_update(prng->seed_, prng->gamma_));
}

static uint64_t sprng64_mix56(uint64_t z) {
  z = ((z ^ (z >> 33)) * 0xff51afd7ed558ccdL) & 0x00FFFFFFFFFFFFFFL;
  z = ((z ^ (z >> 33)) * 0xc4ceb9fe1a85ec53L) & 0x00FFFFFFFFFFFFFFL;
  return z ^ (z >> 33);
}

rng64t* rng64Create(uint64_t seed) {
  return rng64CreateWithGamma(seed, /*gamma_seed=*/0L);
}

rng64t* rng64CreateWithGamma(uint64_t seed, uint64_t gamma_seed) {
  rng64t* prng;

  assert(gamma_seed < gamma_prime_);
  prng = malloc(sizeof(rng64t));

  prng->seed_ = seed;
  gamma_seed += gamma_gamma_;
  if (gamma_seed >= gamma_prime_) gamma_seed -= gamma_prime_;
  prng->gamma_           = sprng64_mix56(gamma_seed) + 13;
  prng->next_gamma_seed_ = gamma_seed;
  return prng;
}

rng64t* rng64Split(rng64t* prng) {
  uint64_t seed       = sprng64_advance_seed(prng);
  uint64_t gamma_seed = prng->next_gamma_seed_;
  return rng64CreateWithGamma(seed, gamma_seed);
}

void rng64Free(rng64t* prng) { free(prng); }

uint64_t rng64NextInt64(rng64t* prng) {
  return sprng64_mix64(sprng64_advance_seed(prng));
}

uint32_t rng64NextInt32(rng64t* prng) {
  return (uint32_t)(rng64NextInt64(prng));
}

double rng64NextDouble(rng64t* prng) {
  return (rng64NextInt64(prng) >> 11) * double_ulp_;
}
