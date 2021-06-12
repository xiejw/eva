#include "testing/testing.h"

#include "rng/srng64_normal.h"

#include <stdlib.h>

#define SIZE 4

static char *
test_normal()
{
        struct rng64_t *rng = (struct rng64_t *)srng64New(456L);

        double expected[] = {1.348183, -1.670144, 1.431032, 0.632074};
        double got[SIZE];
        rng64StdNormalD(rng, SIZE, got);

        ASSERT_ARRAY_CLOSE("result", expected, got, SIZE, 1e-5);
        rng64Free(rng);

        return NULL;
}

static char *
test_normal_f()
{
        struct rng64_t *rng = (struct rng64_t *)srng64New(456L);

        float expected[] = {1.348183, -1.670144, 1.431032, 0.632074};
        float got[SIZE];
        rng64StdNormalF(rng, SIZE, got);

        ASSERT_ARRAY_CLOSE("result", expected, got, SIZE, 1e-5);
        rng64Free(rng);

        return NULL;
}

char *
run_rng_srng64_normal_suite()
{
        RUN_TEST(test_normal);
        RUN_TEST(test_normal_f);
        return NULL;
}
