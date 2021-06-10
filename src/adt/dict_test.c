#include "testing/testing.h"

#include "dict.h"

#include "string.h"

uint64_t
hashFn(const void *key)
{
        const char *str  = (const char *)key;
        uint64_t    hash = 5381;
        while (*str) hash = ((hash << 5) + hash) ^ *str++;
        return hash;
}

int
keyCmp(void *privdata, const void *key1, const void *key2)
{
        return 0 == strcmp((char *)key1, (char *)key2);
}

struct dict_ty_t ty = {
    .hashFn  = hashFn,
    .keyDup  = NULL,
    .valDup  = NULL,
    .keyCmp  = keyCmp,
    .keyFree = NULL,
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
        dictSetUIntVal(en, 123);
        ASSERT_TRUE("val", dictGetUIntVal(dictFind(t, "abc")) == 123);
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
        dictSetUIntVal(en, 123);

        en = dictAddOrFind(t, "abc", &existed);
        ASSERT_TRUE("existed", existed == 1);

        ASSERT_TRUE("same val", dictGetUIntVal(dictFind(t, "abc")) == 123);
        dictFree(t);
        return NULL;
}

static char *
test_expand()
{
        dict_t *t = dictNew(&ty, NULL);

        struct dict_entry_t *en = dictAddOrFind(t, "abc", NULL);
        dictSetUIntVal(en, 123);
        ASSERT_TRUE("val", dictGetUIntVal(dictFind(t, "abc")) == 123);
        size_t size = t->ht.size;

        dictExpand(t, 1024);
        ASSERT_TRUE("new size", t->ht.size > size);
        ASSERT_TRUE("val after hash",
                    dictGetUIntVal(dictFind(t, "abc")) == 123);

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
        return NULL;
}
