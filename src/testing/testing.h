#ifndef TESTING_H_
#define TESTING_H_

#include <math.h>  /* fabs */
#include <stdio.h> /* printf */

// -----------------------------------------------------------------------------
// Design:
//
// This unit testing framework is inspired by MinUnit.
//
//     http://www.jera.com/techinfo/jtns/jtn002.html
//
// Core Idea.
// ----------
//
// The test method should have type
//
// ```
// char* test_foo() {
//   return NULL;
// }
// ```
// NULL means successful; any non-NULL string is the error message.
//
//
// Run Test and Suite.
// -------------------
//
// There is a global `tests_run` counting the total tests run.
//
//   RUN_TEST(test_fn)    // Increase `tests_run`
//   RUN_SUITE(test_fn)   // Does not increase `tests_run`
//
//
// Assertion.
// ------------
//
// ASSERT_TRUE(msg, condition)
// ASSERT_ARRAY_CLOSE(msg, expected, got, size, tol)
// -----------------------------------------------------------------------------

extern int tests_run;

#define RUN_TEST(test) RUN_TEST_IMPL(test, #test)

#define RUN_TEST_IMPL(test, name)  \
  do {                             \
    char *msg;                     \
    printf("  Running: %s", name); \
    msg = (test)();                \
    tests_run++;                   \
    if (msg != NULL) {             \
      printf("...Failed.\n");      \
      return msg;                  \
    } else {                       \
      printf(".\n");               \
    }                              \
  } while (0)

#define RUN_SUITE(test)          \
  do {                           \
    char *msg = (test)();        \
    if (msg != NULL) return msg; \
  } while (0)

// -----------------------------------------------------------------------------
// Assertion
// -----------------------------------------------------------------------------

#define ASSERT_TRUE(msg, test) ASSERT_TRUE_IMPL(msg, test, __FILE__, __LINE__)

#define ASSERT_TRUE_IMPL(msg, test, file, lineno) \
  do {                                            \
    if (!(test)) {                                \
      ASSERT_PRINT_LOC(file, lineno);             \
      return msg;                                 \
    }                                             \
  } while (0)

#define ASSERT_ARRAY_CLOSE(msg, expected, got, size, tol) \
  ASSERT_ARRAY_CLOSE_IMPL(msg, expected, got, size, tol, __FILE__, __LINE__)

#define ASSERT_ARRAY_CLOSE_IMPL(msg, expected, got, size, tol, file, lineno) \
  do {                                                                       \
    int i;                                                                   \
    for (i = 0; i < size; i++) {                                             \
      if (fabs(expected[i] - got[i]) >= tol) {                               \
        ASSERT_PRINT_LOC(file, lineno);                                      \
        printf("\n-> at element %d\n-> expected %f\n-> got %f\n", i,         \
               expected[i], got[i]);                                         \
        return msg;                                                          \
      }                                                                      \
    }                                                                        \
  } while (0)

/* Prints the location in two lines with yellow color. */
#define ASSERT_PRINT_LOC(file, lineno) \
  printf("\n\033[1;33m-> File: %s\n-> Line %d\033[0m\n", file, lineno)

#endif
