#include "testing/testing.h"

#include "rng/srng64.h"

#include <stdlib.h>

static char*
test_uint64()
{
        struct srng64_t* rng = srng64New(456L);
        ASSERT_TRUE("rng_uint64", 1843008821991917904 ==
                                      rng64NextUint64((struct rng64_t*)rng));
        srng64Free(rng);
        return NULL;
}

char*
run_rng_srng64_suite()
{
        RUN_TEST(test_uint64);
        return NULL;
}
