#include "testing/testing.h"

#include "rng/srng64_normal.h"

#define SIZE 4

static char*
test_normal()
{
        struct srng64_t* rng = srng64New(456L);

        double expected[] = {1.348183, -1.670144, 1.431032, 0.632074};
        double got[SIZE];
        srng64StdNormalD(rng, SIZE, got);

        ASSERT_ARRAY_CLOSE("result", expected, got, SIZE, 1e-5);
        srng64Free(rng);

        return NULL;
}

static char*
test_normal_f()
{
        struct srng64_t* rng = srng64New(456L);

        float expected[] = {1.348183, -1.670144, 1.431032, 0.632074};
        float got[SIZE];
        srng64StdNormalF(rng, SIZE, got);

        ASSERT_ARRAY_CLOSE("result", expected, got, SIZE, 1e-5);
        srng64Free(rng);

        return NULL;
}

char*
run_rng_srng64_normal_suite()
{
        RUN_TEST(test_normal);
        RUN_TEST(test_normal_f);
        return NULL;
}
