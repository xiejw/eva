#include "testing/testing.h"

#include "value.h"

static char *
test_value_kind()
{
        return NULL;
}

char *
run_adt_value_suite()
{
        RUN_TEST(test_value_kind);
        return NULL;
}
