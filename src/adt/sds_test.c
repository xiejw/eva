#include "testing/testing.h"

#include "sds.h"

#include "string.h"

static char* test_new() {
  sds s = sdsNew("hello");
  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) == 5);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_new_len() {
  sds s = sdsNewLen("hello", 10);
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
  sds s = sdsNewLen(NULL, 10);
  ASSERT_TRUE("len", sdsLen(s) == 10);
  ASSERT_TRUE("cap", sdsCap(s) == 10);
  ASSERT_TRUE("str", strcmp(s, "") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_empty() {
  sds s = sdsEmpty();
  ASSERT_TRUE("len", sdsLen(s) == 0);
  ASSERT_TRUE("cap", sdsCap(s) == 0);
  ASSERT_TRUE("str", strcmp(s, "") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_dup() {
  sds old_s = sdsNew("hello");
  sds s     = sdsNew(old_s);
  sdsFree(old_s);

  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) == 5);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_cat_len() {
  sds s = sdsNew("hello");
  sdsCatLen(&s, " mlvm", 5);
  ASSERT_TRUE("len", sdsLen(s) == 10);
  ASSERT_TRUE("cap", sdsCap(s) >= 10);
  ASSERT_TRUE("str", strcmp(s, "hello mlvm") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_cat() {
  sds s = sdsNew("hello");
  sdsCat(&s, " mlvm");
  ASSERT_TRUE("len", sdsLen(s) == 10);
  ASSERT_TRUE("cap", sdsCap(s) >= 10);
  ASSERT_TRUE("str", strcmp(s, "hello mlvm") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_cat_sds() {
  sds s = sdsNew("hello");
  sds t = sdsNew(" mlvm");
  sdsCatSds(&s, t);
  ASSERT_TRUE("len", sdsLen(s) == 10);
  ASSERT_TRUE("cap", sdsCap(s) >= 10);
  ASSERT_TRUE("str", strcmp(s, "hello mlvm") == 0);
  sdsFree(s);
  sdsFree(t);
  return NULL;
}

static char* test_cat_printf() {
  sds s = sdsNew("hello");
  sdsCatPrintf(&s, " %s %d", "mlvm", 123);

  ASSERT_TRUE("len", sdsLen(s) == 14);
  ASSERT_TRUE("cap", sdsCap(s) >= 14);
  ASSERT_TRUE("str", strcmp(s, "hello mlvm 123") == 0);
  sdsFree(s);
  return NULL;
}

static char* test_make_room() {
  sds s = sdsNew("hello");
  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) == 5);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);

  sdsMakeRoomFor(&s, 5);
  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) >= 10);
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);

  int old_cap = sdsCap(s);

  sdsMakeRoomFor(&s, 1);
  ASSERT_TRUE("len", sdsLen(s) == 5);
  ASSERT_TRUE("cap", sdsCap(s) == old_cap);  // should be enough for incr 1.
  ASSERT_TRUE("str", strcmp(s, "hello") == 0);

  sdsFree(s);
  return NULL;
}

char* run_sds_suite() {
  RUN_TEST(test_new);
  RUN_TEST(test_new_len);
  RUN_TEST(test_new_len_null);
  RUN_TEST(test_empty);
  RUN_TEST(test_dup);
  RUN_TEST(test_cat_len);
  RUN_TEST(test_cat);
  RUN_TEST(test_cat_sds);
  RUN_TEST(test_cat_printf);
  RUN_TEST(test_make_room);
  return NULL;
}
