#include "testing/testing.h"

#include "adt/sds.h"

#include "string.h"

static char *
test_new()
{
        sds_t s = sdsNew("hello");
        ASSERT_TRUE("len", sdsLen(s) == 5);
        ASSERT_TRUE("cap", sdsCap(s) >= 5);
        ASSERT_TRUE("str", strcmp(s, "hello") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_new_null()
{
        sds_t s = sdsNew(NULL);
        ASSERT_TRUE("len", sdsLen(s) == 0);
        ASSERT_TRUE("cap", sdsCap(s) >= 0);
        ASSERT_TRUE("str", strcmp(s, "") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_empty()
{
        sds_t s = sdsEmpty();
        ASSERT_TRUE("len", sdsLen(s) == 0);
        ASSERT_TRUE("cap", sdsCap(s) >= 0);
        ASSERT_TRUE("str", strcmp(s, "") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_empty_with_cap()
{
        sds_t s = sdsEmptyWithCap(10);
        ASSERT_TRUE("len", sdsLen(s) == 0);
        ASSERT_TRUE("cap", sdsCap(s) >= 10);
        ASSERT_TRUE("str", strcmp(s, "") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_dup()
{
        sds_t old_s = sdsNew("hello");
        sds_t s     = sdsDup(old_s);
        sdsFree(old_s);

        ASSERT_TRUE("len", sdsLen(s) == 5);
        ASSERT_TRUE("cap", sdsCap(s) >= 5);
        ASSERT_TRUE("str", strcmp(s, "hello") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_dup_null()
{
        sds_t s = sdsDup(NULL);

        ASSERT_TRUE("len", sdsLen(s) == 0);
        ASSERT_TRUE("cap", sdsCap(s) >= 0);
        ASSERT_TRUE("str", strcmp(s, "") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_cat_len()
{
        sds_t s = sdsNew("hello");
        sdsCatLen(&s, " mlvm", 5);
        ASSERT_TRUE("len", sdsLen(s) == 10);
        ASSERT_TRUE("cap", sdsCap(s) >= 10);
        ASSERT_TRUE("str", strcmp(s, "hello mlvm") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_cat()
{
        sds_t s = sdsNew("hello");
        sdsCat(&s, " mlvm");
        ASSERT_TRUE("len", sdsLen(s) == 10);
        ASSERT_TRUE("cap", sdsCap(s) >= 10);
        ASSERT_TRUE("str", strcmp(s, "hello mlvm") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_cat_sds()
{
        sds_t s = sdsNew("hello");
        sds_t t = sdsNew(" mlvm");
        sdsCatSds(&s, t);
        ASSERT_TRUE("len", sdsLen(s) == 10);
        ASSERT_TRUE("cap", sdsCap(s) >= 10);
        ASSERT_TRUE("str", strcmp(s, "hello mlvm") == 0);
        sdsFree(s);
        sdsFree(t);
        return NULL;
}

static char *
test_cat_printf()
{
        sds_t s = sdsNew("hello");
        sdsReserve(&s, 14);
        size_t cap = sdsCap(s);
        sdsCatPrintf(&s, " %d", 123);

        ASSERT_TRUE("len", sdsLen(s) == 9);
        ASSERT_TRUE("cap", sdsCap(s) >= cap);
        ASSERT_TRUE("str", strcmp(s, "hello 123") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_cat_printf_long()
{
        sds_t s = sdsNew("hello");
        ASSERT_TRUE("cap", sdsCap(s) <= 20);
        sdsCatPrintf(&s, " super super super super long %s %d", "mlvm", 123);

        ASSERT_TRUE("cap", sdsCap(s) > 20);
        ASSERT_TRUE(
            "str",
            strcmp(s, "hello super super super super long mlvm 123") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_reserve()
{
        sds_t s = sdsNew("hello");
        ASSERT_TRUE("len", sdsLen(s) == 5);
        ASSERT_TRUE("cap", sdsCap(s) >= 5);
        ASSERT_TRUE("str", strcmp(s, "hello") == 0);

        sdsReserve(&s, 10);
        ASSERT_TRUE("len", sdsLen(s) == 5);
        ASSERT_TRUE("cap", sdsCap(s) >= 10);
        ASSERT_TRUE("str", strcmp(s, "hello") == 0);

        int old_cap = sdsCap(s);

        sdsReserve(&s, 1);
        ASSERT_TRUE("len", sdsLen(s) == 5);
        ASSERT_TRUE("cap",
                    sdsCap(s) == old_cap);  // should be enough for incr 1.
        ASSERT_TRUE("str", strcmp(s, "hello") == 0);

        sdsFree(s);
        return NULL;
}

static char *
test_cpy()
{
        sds_t s = sdsNew("");
        ASSERT_TRUE("len", sdsLen(s) == 0);
        sdsCpy(&s, "hello");
        ASSERT_TRUE("len", sdsLen(s) == 5);
        ASSERT_TRUE("str", strcmp(s, "hello") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_cpy_len()
{
        sds_t s = sdsNew("");
        ASSERT_TRUE("len", sdsLen(s) == 0);
        sdsCpyLen(&s, "hello", 3);
        ASSERT_TRUE("len", sdsLen(s) == 3);
        ASSERT_TRUE("str", strcmp(s, "hel") == 0);
        sdsFree(s);
        return NULL;
}

static char *
test_cmp()
{
        sds_t s1 = sdsNew("hello");
        sds_t s2 = sdsNew("hello");
        ASSERT_TRUE("cmp", 0 == sdsCmp(s1, s2));

        sdsCpy(&s1, "i");
        ASSERT_TRUE("cmp", 0 < sdsCmp(s1, s2));
        sdsCpy(&s1, "hell");
        ASSERT_TRUE("cmp", 0 > sdsCmp(s1, s2));

        sdsFree(s1);
        sdsFree(s2);
        return NULL;
}

char *
run_adt_sds_suite()
{
        RUN_TEST(test_new);
        RUN_TEST(test_new_null);
        RUN_TEST(test_empty);
        RUN_TEST(test_empty_with_cap);
        RUN_TEST(test_dup);
        RUN_TEST(test_dup_null);
        RUN_TEST(test_cat_len);
        RUN_TEST(test_cat);
        RUN_TEST(test_cat_sds);
        RUN_TEST(test_cat_printf);
        RUN_TEST(test_cat_printf_long);
        RUN_TEST(test_reserve);
        RUN_TEST(test_cpy);
        RUN_TEST(test_cpy_len);
        RUN_TEST(test_cmp);
        return NULL;
}
