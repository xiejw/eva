#include "testing/testing.h"

#include "map.h"

#define ASSERT_NO_ERR(v) ASSERT_TRUE("no err", OK == (v))

static char*
test_map_new()
{
        map_t(int) m = mapNew();
        ASSERT_TRUE("size", 0 == mapSize(m));
        ASSERT_TRUE("buckets", 0 == mapNBuckets(m));
        mapFree(m);
        return NULL;
}

static char*
test_map_get_set()
{
        map_t(int) m = mapNew();
        ASSERT_TRUE("size", 0 == mapSize(m));
        ASSERT_TRUE("buckets", 0 == mapNBuckets(m));
        ASSERT_TRUE("item", NULL == mapGet(m, "abc"));

        ASSERT_NO_ERR(mapSet(m, "abc", 123));
        ASSERT_TRUE("size", 1 == mapSize(m));
        ASSERT_TRUE("buckets", 16 <= mapNBuckets(m));
        ASSERT_TRUE("item", 123 == *mapGet(m, "abc"));
        mapFree(m);
        return NULL;
}

static char*
test_map_reserve()
{
        map_t(int) m = mapNew();
        ASSERT_TRUE("size", 0 == mapSize(m));
        ASSERT_TRUE("buckets", 0 == mapNBuckets(m));

        ASSERT_NO_ERR(mapReserve(m, 32));
        ASSERT_TRUE("size", 0 == mapSize(m));
        ASSERT_TRUE("buckets", 32 == mapNBuckets(m));

        ASSERT_NO_ERR(mapReserve(m, 124));
        ASSERT_TRUE("size", 0 == mapSize(m));
        ASSERT_TRUE("buckets", 128 == mapNBuckets(m));
        mapFree(m);
        return NULL;
}

static char*
test_map_foreach()
{
        map_t(int) m = mapNew();
        ASSERT_TRUE("size", 0 == mapSize(m));

        ASSERT_NO_ERR(mapSet(m, "a", 1));
        ASSERT_NO_ERR(mapSet(m, "b", 2));
        ASSERT_NO_ERR(mapSet(m, "c", 3));
        ASSERT_TRUE("size", 3 == mapSize(m));

        int count = 0;
        MAP_FOREACH(m, NULL, NULL)
        count++;
        ASSERT_TRUE("count", 3 == count);

        int         found_1 = 0;
        int         found_2 = 0;
        int         found_3 = 0;
        const char* key;
        int*        v;
        MAP_FOREACH(m, &key, &v)
        {
                if (strcmp(key, "a") == 0) {
                        ASSERT_TRUE("no found", 0 == found_1);
                        found_1 = 1;
                        ASSERT_TRUE("value", 1 == *v);
                } else if (strcmp(key, "b") == 0) {
                        ASSERT_TRUE("no found", 0 == found_2);
                        found_2 = 1;
                        ASSERT_TRUE("value", 2 == *v);
                } else if (strcmp(key, "c") == 0) {
                        ASSERT_TRUE("no found", 0 == found_3);
                        found_3 = 1;
                        ASSERT_TRUE("value", 3 == *v);
                } else {
                        ASSERT_TRUE("fail", 0);
                }
        }

        ASSERT_TRUE("found", 1 == found_1);
        ASSERT_TRUE("found", 1 == found_2);
        ASSERT_TRUE("found", 1 == found_3);

        mapFree(m);
        return NULL;
}

char*
run_adt_map_suite()
{
        RUN_TEST(test_map_new);
        RUN_TEST(test_map_get_set);
        RUN_TEST(test_map_reserve);
        RUN_TEST(test_map_foreach);
        return NULL;
}
