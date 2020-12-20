#include "testing/testing.h"

#include "map.h"

static char* test_map_init_empty() {
  map_t(int) m = mapNew();
  mapFree(m);
  return NULL;
}

static char* test_map_init_with_items() {
  map_t(int) m = mapNew();
  mapSet(m, "abc", 123);
  mapFree(m);
  return NULL;
}

char* run_adt_map_suite() {
  RUN_TEST(test_map_init_empty);
  RUN_TEST(test_map_init_with_items);
  return NULL;
}
