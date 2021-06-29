#include "testing/testing.h"

#include <string.h>

// eva
#include "dict.h"
#include "hashing.h"
#include "value.h"

// -----------------------------------------------------------------------------
// static str
// -----------------------------------------------------------------------------
struct dict_ty_t ty = {
    .hashFn  = hashFnStr,
    .keyDup  = dupFnStr,
    .valDup  = NULL,
    .keyCmp  = keyCmpStr,
    .keyFree = freeFnStr,
    .valFree = NULL,
};

static char *
test_init()
{
        dict_t *t = dictNew(&ty, NULL);
        dictFree(t);
        return NULL;
}

static char *
test_add_and_find()
{
        dict_t *t = dictNew(&ty, NULL);

        struct dict_entry_t *en = dictAddOrFind(t, "abc", NULL);
        dictSetU64(en, 123);
        ASSERT_TRUE("val", dictGetU64(dictFind(t, "abc")) == 123);
        ASSERT_TRUE("val null", NULL == dictFind(t, "bc"));

        dictFree(t);
        return NULL;
}

static char *
test_add_and_find_existed()
{
        int     existed;
        dict_t *t = dictNew(&ty, NULL);

        struct dict_entry_t *en = dictAddOrFind(t, "abc", &existed);
        ASSERT_TRUE("not existed", existed == 0);
        dictSetU64(en, 123);

        en = dictAddOrFind(t, "abc", &existed);
        ASSERT_TRUE("existed", existed == 1);

        ASSERT_TRUE("same val", dictGetU64(dictFind(t, "abc")) == 123);
        dictFree(t);
        return NULL;
}

static char *
test_expand()
{
        dict_t *t = dictNew(&ty, NULL);

        struct dict_entry_t *en = dictAddOrFind(t, "abc", NULL);
        dictSetU64(en, 123);
        ASSERT_TRUE("val", dictGetU64(dictFind(t, "abc")) == 123);
        size_t size = t->ht.size;

        dictExpand(t, 1024);
        ASSERT_TRUE("new size", t->ht.size > size);
        ASSERT_TRUE("val after hash", dictGetU64(dictFind(t, "abc")) == 123);

        dictFree(t);
        return NULL;
}

// -----------------------------------------------------------------------------
// int64
// -----------------------------------------------------------------------------

struct dict_ty_t ty_i64 = {
    .hashFn  = valueHashFnI64,
    .keyDup  = valueDupI64,
    .valDup  = NULL,
    .keyCmp  = valueCmpI64,
    .keyFree = valueFreeI64,
    .valFree = NULL,
};

static char *
test_add_and_find_existed_i64()
{
        int     existed;
        dict_t *t = dictNew(&ty_i64, NULL);

        struct dict_entry_t *en;
        {
                struct value_t key;
                valueSetI64(&key, 123);
                en = dictAddOrFind(t, &key, &existed);
                ASSERT_TRUE("not existed", existed == 0);
                dictSetU64(en, 456);
        }

        {
                struct value_t key = {.i64 = 123};
                en                 = dictAddOrFind(t, &key, &existed);
                ASSERT_TRUE("existed", existed == 1);
                ASSERT_TRUE("same val", dictGetU64(dictFind(t, &key)) == 456);
        }
        dictFree(t);
        return NULL;
}

char *
run_adt_dict_suite()
{
        RUN_TEST(test_init);
        RUN_TEST(test_add_and_find);
        RUN_TEST(test_add_and_find_existed);
        RUN_TEST(test_expand);
        RUN_TEST(test_add_and_find_existed_i64);
        return NULL;
}
