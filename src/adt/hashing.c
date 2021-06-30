#include "hashing.h"

#include <stdlib.h>
#include <string.h>

uint64_t
hashFnStr(const void *key)
{
        const char *str  = (const char *)key;
        uint64_t    hash = 5381;
        while (*str) hash = ((hash << 5) + hash) ^ *str++;
        return hash;
}

int
keyCmpFnStr(void *privdata, const void *key1, const void *key2)
{
        return 0 == strcmp((char *)key1, (char *)key2);
}

void *
dupFnStr(void *privdata, const void *old_str)
{
        char *new_str = malloc(sizeof(char) * (strlen((char *)old_str) + 1));
        strcpy(new_str, (char *)old_str);
        return new_str;
}

void
freeFnStr(void *privdata, void *p)
{
        free(p);
}

uint64_t
hashFnPtr(const void *key)
{
        return (intptr_t)key;
}

int
keyCmpFnPtr(void *privdata, const void *key1, const void *key2)
{
        return key1 == key2;
}
