#include "testing/testing.h"

#include "adt/sds.h"

#include "string.h"

static char* test_new() {
  sds_t s = sdsNew("hello");
  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) == 5);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_new_len() {
  sds_t s = sdsNewLen("hello", 10);
  ASSERT_TRUE("len", sdsLen(s) == 10);
  ASSERT_TRUE("cap", sdsCap(s) == 10);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);

  sdsSetLen(s, 5);
  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) == 10);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_new_len_null() {
  sds_t s = sdsNewLen(NULL, 10);
  ASSERT_TRUE("len", sdsLen(s) == 10);
  ASSERT_TRUE("cap", sdsCap(s) == 10);
  ASSERT_TRUE("str", strcmp(s, "") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_empty() {
  sds_t s = sdsEmpty();
  ASSERT_TRUE("len", sdsLen(s) == 0);
  ASSERT_TRUE("cap", sdsCap(s) == 0);
  ASSERT_TRUE("str", strcmp(s, "") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_dup() {
  sds_t old_s = sdsNew("hello");
  sds_t s     = sdsNew(old_s);
  sdsFree(old_s);

  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) == 5);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_cat_len() {
  sds_t s = sdsNew("hello");
  sdsCatLen(&s, " mlvm", 5);
  ASSERT_TRUE("len", sdsLen(s) == 10);
  ASSERT_TRUE("cap", sdsCap(s) >= 10);
  ASSERT_TRUE("str", strcmp(s, "hello mlvm") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_cat() {
  sds_t s = sdsNew("hello");
  sdsCat(&s, " mlvm");
  ASSERT_TRUE("len", sdsLen(s) == 10);
  ASSERT_TRUE("cap", sdsCap(s) >= 10);
  ASSERT_TRUE("str", strcmp(s, "hello mlvm") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_cat_sds() {
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

static char* test_cat_printf() {
  sds_t s = sdsNew("hello");
  sdsCatPrintf(&s, " %s %d", "mlvm", 123);

  ASSERT_TRUE("len", sdsLen(s) == 14);
  ASSERT_TRUE("cap", sdsCap(s) >= 14);
  ASSERT_TRUE("str", strcmp(s, "hello mlvm 123") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_reserve() {
  sds_t s = sdsNew("hello");
  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) == 5);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);

  sdsReserve(&s, 10);
  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) >= 10);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);

  int old_cap = sdsCap(s);

  sdsReserve(&s, 1);
  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) == old_cap);  // should be enough for incr 1.
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);

  sdsFree(s);
  return NULL;
}

static char* test_cpy() {
  sds_t s = sdsNew("");
  ASSERT_TRUE("len", sdsLen(s) == 0);
  sdsCpy(&s, "hello");
  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_cpy_len() {
  sds_t s = sdsNew("");
  ASSERT_TRUE("len", sdsLen(s) == 0);
  sdsCpyLen(&s, "hello", 3);
  ASSERT_TRUE("len", sdsLen(s) == 3);
  ASSERT_TRUE("str", strcmp(s, "hel") == 0);
  sdsFree(s);
  return NULL;
}

char* run_adt_sds_suite() {
  RUN_TEST(test_new);
  RUN_TEST(test_new_len);
  RUN_TEST(test_new_len_null);
  RUN_TEST(test_empty);
  RUN_TEST(test_dup);
  RUN_TEST(test_cat_len);
  RUN_TEST(test_cat);
  RUN_TEST(test_cat_sds);
  RUN_TEST(test_cat_printf);
  RUN_TEST(test_reserve);
  RUN_TEST(test_cpy);
  RUN_TEST(test_cpy_len);
  return NULL;
}
