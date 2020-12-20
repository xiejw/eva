#include "testing/testing.h"

#include "map.h"

static char* test_map_init() { return NULL; }

char* run_adt_map_suite() {
  RUN_TEST(test_map_init);
  return NULL;
}
