#include "test.h"

// Testing Hierarchy.
// ------------------
// 1. For any module, one method is defined as `suite`.
// 2. The `suite` returns `char*` which has the same meaing as normal test fn.
// 3. It must return non-NULL if any test in it failed. How it runs tests is
//    undefined. Typically it runs a flat list of tests.
//
// Example of vec_test.c
// ---------------------
//
//   #include "testing/testing.h"
//
//   static char* test_vec_init() {
//     vect(int) v = NULL;
//     ASSERT_TRUE("size is 0", vecSize(v) == 0);
//     vecFree(v);
//     return NULL;
//   }
//
//   char* run_vec_suite() {
//     RUN_TEST(test_vec_init);
//     return NULL;
//   }

int tests_run = 0;  // declared in testing/testing.h

int main() {
  // ---------------------------------------------------------------------------
  // Adds all suites.
  //
  // Convenstion is for foo, a test suite fn run_foo_suite is called. For
  // customized case, use ADD_SUITE_NAME_AND_FN.

  ADD_SUITE(adt_vec);            // adt/vec_test.c
  ADD_SUITE(adt_map);            // adt/map_test.c
  ADD_SUITE(adt_sds);            // adt/sds_test.c
  ADD_SUITE(cron);               // cron/cron_test.c
  ADD_SUITE(rng_srng64);         // rng/srng64_test.c
  ADD_SUITE(rng_srng64_normal);  // rng/srng64_normal_test.c

  // ---------------------------------------------------------------------------
  // Runs all suites and reports.
  int suites_failed = run_all_suites();
  return suites_failed ? -1 : 0;
}
