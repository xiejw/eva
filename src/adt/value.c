#include "value.h"

#include <stdlib.h>

int
valueCmpI64(void *privdata, const void *key1, const void *key2)
{
        return valueGetI64(key1) == valueGetI64(key2);
}

int valueCmpF32(void *privdata, const void *, const void *);

void *
valueDupI64(void *privdata, const void *p)
{
        struct value_t *new_p = malloc(sizeof(*new_p));
        new_p->kind           = valueGetKind(p);
        new_p->i64            = valueGetI64(p);
        return new_p;
}

void *valueDupF32(void *privdata, const void *);

void
valueFreeI64(void *privdata, void *p)
{
        free(p);
}

void
valueFreeF32(void *privdata, void *p)
{
        free(p);
}

// Dummy impl
uint64_t
valueHashFnI64(const void *key)
{
        return ((struct value_t *)key)->i64;
}
