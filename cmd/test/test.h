#ifndef TEST_MAIN_H_
#define TEST_MAIN_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Test suites data structures.
// -----------------------------------------------------------------------------

extern int tests_run;

typedef struct {
        char* name;
        char* (*fn)();
} test_suite_t;

static test_suite_t* test_suites      = NULL;
static int           test_suite_cap   = 0;
static int           test_suite_count = 0;

// Runs all suites and releases `test_suites`.
static int
run_all_suites()
{
        // Runs all suites.
        int suites_failed = 0;
        for (int i = 0; i < test_suite_count; i++) {
                printf("Running suite: %s\n", test_suites[i].name);

                char* result = test_suites[i].fn();
                if (result != 0) {
                        suites_failed++;
                        printf("\033[1;31mERROR: %s\033[0m\n",
                               result); /* Red */
                } else {
                        printf(
                            "\033[1;32mALL TESTS PASSED.\033[0m\n"); /* Green */
                }
        }

        // Release resources.
        free(test_suites);

        // Reports.
        if (!suites_failed)
                printf("\033[1;32m\n>>> Tests run: %d\033[0m\n", tests_run);
        else
                printf("\033[1;31m\n!!! Test suites failed: %d\033[0m\n",
                       suites_failed);

        return suites_failed;
}

// -----------------------------------------------------------------------------
// Helper macros.
// -----------------------------------------------------------------------------

#define TEST_SUITES_INIT_SIZE 32

#define ADD_SUITE(suite_name) \
        ADD_SUITE_NAME_AND_FN(#suite_name, run_##suite_name##_suite)

#define ADD_SUITE_NAME_AND_FN(suite_name, func)                                  \
        char* func();                                                            \
        if (test_suite_cap <= test_suite_count) {                                \
                test_suite_cap = test_suite_cap ? 2 * test_suite_cap             \
                                                : TEST_SUITES_INIT_SIZE;         \
                test_suites    = realloc(test_suites,                            \
                                      test_suite_cap * sizeof(test_suite_t)); \
        }                                                                        \
        test_suites[test_suite_count].name = suite_name;                         \
        test_suites[test_suite_count++].fn = func;

#endif
