#ifndef HASHING_H_
#define HASHING_H_

#include <stdint.h>  // uint64_j

// All function conforming the hashing, comparing, duplicating, and freeing fns
// signature required by dict.h. See struct dict_ty_t.
//
// - For integer, e.g., i32, i64, u32, u64, hashing func, see value.h

uint64_t hashFnStr(const void *key);
int      keyCmpStr(void *privdata, const void *key1, const void *key2);
void    *dupFnStr(void *privdata, const void *p);
void     freeFnStr(void *privdata, void *p);

#endif
