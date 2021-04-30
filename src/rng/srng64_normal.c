#include "rng/srng64_normal.h"

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

/* unit of least precision */
static const double double_ulp_ = 1.0 / ((uint64_t)1L << 53);
static const double two_pi_     = 2.0 * 3.141592653589793238;

static const float double_ulp_f_ = 1.0 / ((uint64_t)1L << 53);
static const float two_pi_f_     = 2.0 * 3.141592653589793238;

/*
 * The implementation is based on Box–Muller transform.
 *
 * For each pair of [0, 1) uniform rn, a pair of independent, standard,
 * normally distributed rn are generated.
 */
void
srng64StdNormalD(struct srng64_t* rng64, size_t size, double* buffer)
{
        size_t  i;
        size_t  num_seeds = size % 2 == 0 ? size : size + 1; /* Must be even. */
        double* uniforms  = malloc(num_seeds * sizeof(double));

        assert(size > 0);

        for (i = 0; i < num_seeds;) {
                uint64_t seed = srng64NextUint64(rng64);
                double   u    = (seed >> 11) * double_ulp_;
                /* The first rn in each pair is used by log, so cannot be zero.
                 */
                if (i % 2 == 1 || u >= DBL_EPSILON) uniforms[i++] = u;
        }

        for (i = 0; i < size;) {
                double u_1 = uniforms[i];
                double u_2 = uniforms[i + 1];

                double r     = sqrt(-2.0 * log(u_1));
                double theta = two_pi_ * u_2;

                buffer[i] = r * cos(theta);
                if (i + 1 < size) buffer[i + 1] = r * sin(theta);
                i += 2;
        }

        free(uniforms);
}

void
srng64StdNormalF(struct srng64_t* rng64, size_t size, float* buffer)
{
        size_t i;
        size_t num_seeds = size % 2 == 0 ? size : size + 1; /* Must be even. */
        float* uniforms  = malloc(num_seeds * sizeof(float));

        assert(size > 0);

        for (i = 0; i < num_seeds;) {
                uint64_t seed = srng64NextUint64(rng64);
                float    u    = (seed >> 11) * double_ulp_f_;
                /* The first rn in each pair is used by log, so cannot be zero.
                 */
                if (i % 2 == 1 || u >= FLT_EPSILON) uniforms[i++] = u;
        }

        for (i = 0; i < size;) {
                float u_1 = uniforms[i];
                float u_2 = uniforms[i + 1];

                float r     = sqrt(-2.0 * log(u_1));
                float theta = two_pi_f_ * u_2;

                buffer[i] = r * cos(theta);
                if (i + 1 < size) buffer[i + 1] = r * sin(theta);
                i += 2;
        }

        free(uniforms);
}
