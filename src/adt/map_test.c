#include "testing/testing.h"

#include "map.h"

static char* test_map_new() {
  map_t(int) m = mapNew();
  ASSERT_TRUE("size", 0 == mapSize(m));
  ASSERT_TRUE("buckets", 0 == mapNBuckets(m));
  mapFree(m);
  return NULL;
}

static char* test_map_get_set() {
  map_t(int) m = mapNew();
  ASSERT_TRUE("size", 0 == mapSize(m));
  ASSERT_TRUE("buckets", 0 == mapNBuckets(m));
  ASSERT_TRUE("item", NULL == mapGet(m, "abc"));

  mapSet(m, "abc", 123);
  ASSERT_TRUE("size", 1 == mapSize(m));
  ASSERT_TRUE("buckets", 16 <= mapNBuckets(m));
  ASSERT_TRUE("item", 123 == *mapGet(m, "abc"));
  mapFree(m);
  return NULL;
}

static char* test_map_reserve() {
  map_t(int) m = mapNew();
  ASSERT_TRUE("size", 0 == mapSize(m));
  ASSERT_TRUE("buckets", 0 == mapNBuckets(m));

  mapReserve(m, 32);
  ASSERT_TRUE("size", 0 == mapSize(m));
  ASSERT_TRUE("buckets", 32 == mapNBuckets(m));

  mapReserve(m, 124);
  ASSERT_TRUE("size", 0 == mapSize(m));
  ASSERT_TRUE("buckets", 128 == mapNBuckets(m));
  mapFree(m);
  return NULL;
}

char* run_adt_map_suite() {
  RUN_TEST(test_map_new);
  RUN_TEST(test_map_get_set);
  RUN_TEST(test_map_reserve);
  return NULL;
}
