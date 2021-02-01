#include "testing/testing.h"

#include "diff.h"

static int is_eql(void* lhs, void* rhs, size_t i, size_t j)
{
        return *(((int*)lhs) + i) == *(j + ((int*)rhs));
}

static char* test_diff()
{
        struct diff_item_t item;
        item.lhs      = (int[]){2, 3};
        item.rhs      = (int[]){2, 3};
        item.lhs_size = 2;
        item.rhs_size = 2;
        item.is_eql   = is_eql;

        ASSERT_TRUE("expect ok", OK == diffItems(&item));
        return NULL;
}

char* run_algorithrms_diff_suite()
{
        RUN_TEST(test_diff);
        return NULL;
}
