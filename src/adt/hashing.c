#include "hashing.h"

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
keyCmpStr(void *privdata, const void *key1, const void *key2)
{
        return 0 == strcmp((char *)key1, (char *)key2);
}
