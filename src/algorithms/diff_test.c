#include "testing/testing.h"

#include "diff.h"

static int is_eql(void* lhs, void* rhs, size_t i, size_t j)
{
        return *(i + ((int*)lhs)) == *(j + ((int*)rhs));
}

static char* test_diff_eq()
{
        struct diff_item_t item;
        item.lhs      = (int[]){2, 3};
        item.rhs      = (int[]){2, 3};
        item.lhs_size = 2;
        item.rhs_size = 2;
        item.is_eql   = is_eql;

        ASSERT_TRUE("expect ok", OK == diffItems(&item));
        ASSERT_TRUE("d is 0", 0 == item.d);
        diffFree(&item);
        return NULL;
}

static char* test_diff_lhs_1()
{
        struct diff_item_t item;
        item.lhs      = (int[]){2, 3, 3};
        item.rhs      = (int[]){2, 3};
        item.lhs_size = 3;
        item.rhs_size = 2;
        item.is_eql   = is_eql;

        ASSERT_TRUE("expect ok", OK == diffItems(&item));
        ASSERT_TRUE("d is 1", 1 == item.d);
        diffFree(&item);
        return NULL;
}

static char* test_diff_rhs_1()
{
        struct diff_item_t item;
        item.lhs      = (int[]){2, 3, 3};
        item.rhs      = (int[]){2, 3, -1, 3};
        item.lhs_size = 3;
        item.rhs_size = 4;
        item.is_eql   = is_eql;

        ASSERT_TRUE("expect ok", OK == diffItems(&item));
        ASSERT_TRUE("d is 1", 1 == item.d);
        return NULL;
}

static char* test_diff_complicated()
{
        struct diff_item_t item;
        item.lhs      = (int[]){2, 10, 3, 23, 3};
        item.rhs      = (int[]){-290, -49, 2, 3, -1, 3, -23 - 1};
        item.lhs_size = 5;
        item.rhs_size = 8;
        item.is_eql   = is_eql;

        ASSERT_TRUE("expect ok", OK == diffItems(&item));
        ASSERT_TRUE("d is 1", 7 == item.d);
        diffFree(&item);
        return NULL;
}

char* run_algorithrms_diff_suite()
{
        RUN_TEST(test_diff_eq);
        RUN_TEST(test_diff_lhs_1);
        RUN_TEST(test_diff_rhs_1);
        RUN_TEST(test_diff_complicated);
        return NULL;
}
