#include "testing/testing.h"

#include "vec.h"

static char* test_vec_init() {
  vec_t(int) v = vecNew();
  ASSERT_TRUE("size is 0", vecSize(v) == 0);
  ASSERT_TRUE("cap is 0", vecCap(v) == 0);
  vecFree(v);
  return NULL;
}

static char* test_vec_push_back() {
  vec_t(int) v = vecNew();
  vecPushBack(v, 123);
  ASSERT_TRUE("v[0] is 123", v[0] == 123);
  ASSERT_TRUE("size is 1", vecSize(v) == 1);
  ASSERT_TRUE("cap is >0", vecCap(v) > 0);
  vecFree(v);
  return NULL;
}

static char* test_vec_reserve() {
  vec_t(int) v = vecNew();
  vecReserve(v, 100);
  ASSERT_TRUE("size is 0", vecSize(v) == 0);
  ASSERT_TRUE("cap is 100", vecCap(v) == 100);
  vecFree(v);
  return NULL;
}

static char* test_vec_grow() {
  vec_t(int) v = vecNew();
  vecReserve(v, 2);
  ASSERT_TRUE("size is 0", vecSize(v) == 0);
  ASSERT_TRUE("cap is 2", vecCap(v) == 2);

  vecPushBack(v, 0);
  ASSERT_TRUE("v[i]", v[0] == 0);
  ASSERT_TRUE("size", vecSize(v) == 1);
  ASSERT_TRUE("cap", vecCap(v) == 2);

  vecPushBack(v, 1);
  ASSERT_TRUE("v[i]", v[1] == 1);
  ASSERT_TRUE("size", vecSize(v) == 2);
  ASSERT_TRUE("cap", vecCap(v) == 2);

  vecPushBack(v, 2);
  ASSERT_TRUE("v[i]", v[2] == 2);
  ASSERT_TRUE("size", vecSize(v) == 3);
  ASSERT_TRUE("cap", vecCap(v) == 4);

  vecFree(v);
  return NULL;
}

char* run_adt_vec_suite() {
  RUN_TEST(test_vec_init);
  RUN_TEST(test_vec_push_back);
  RUN_TEST(test_vec_reserve);
  RUN_TEST(test_vec_grow);
  return NULL;
}
