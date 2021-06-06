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
        return strcmp((char *)key1, (char *)key2);
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
        struct dict_t *t = dictNew(&ty, NULL);
        dictFree(t);
        return NULL;
}

static char *
test_add_and_find()
{
        struct dict_t *t = dictNew(&ty, NULL);

        struct dict_entry_t *en = dictAddOrFind(t, "abc");
        dictSetUIntVal(en, 123);
        ASSERT_TRUE("val", dictGetUIntVal(dictFind(t, "abc")) == 123);
        ASSERT_TRUE("val null", NULL == dictFind(t, "bc"));

        dictFree(t);
        return NULL;
}

char *
run_adt_dict_suite()
{
        RUN_TEST(test_init);
        RUN_TEST(test_add_and_find);
        return NULL;
}
