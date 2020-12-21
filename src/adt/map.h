#ifndef MAP_H_
#define MAP_H_

#include "stdlib.h"
#include "string.h"

#include "base/defs.h"

// -----------------------------------------------------------------------------
// Onwership.
//
// 1. container `vec` does not own the ownership of each elements. So caller
//    should free them.
// 2. Must call vecFree to release the memory, used by vec, on heap.
//
// github.com/rxi/map

// -----------------------------------------------------------------------------
// data structure.
// -----------------------------------------------------------------------------

struct map_node_t;

typedef struct {
  struct map_node_t** buckets;
  unsigned            nbuckets, nnodes;
} map_base_t;

// -----------------------------------------------------------------------------
// public macros.
// -----------------------------------------------------------------------------

#define map_t(type)  \
  struct {           \
    map_base_t base; \
    type*      ref;  \
    type       tmp;  \
  }*

#define mapNew()   NULL
#define mapFree(m) _mapFree((map_base_t*)(m))

#define mapSize(m)     ((m) != NULL ? (m)->base.nnodes : 0)
#define mapNBuckets(m) ((m) != NULL ? (m)->base.nbuckets : 0)
// mapReserve

// return error_t
#define mapSet(m, k, v)                                   \
  (_mapInit((void*)(&(m)), sizeof(*(m))), (m)->tmp = (v), \
   _mapSet(&(m)->base, (k), &(m)->tmp, sizeof((m)->tmp)))

// return NULL-able address pointing to value.
#define mapGet(m, k) \
  ((m) != NULL ? ((m)->ref = _mapGet(&(m)->base, (k))) : NULL)

// -----------------------------------------------------------------------------
// private prototype.
// -----------------------------------------------------------------------------

static inline void _mapInit(_mut_ void** m, int vsize);
extern void        _mapFree(map_base_t*);
extern error_t _mapSet(map_base_t* m, const char* key, void* value, int vsize);
extern void*   _mapGet(map_base_t* m, const char* key);

void _mapInit(_mut_ void** m, int vsize) {
  if (*m == NULL) {
    void* p = malloc(vsize);
    *m      = p;
    memset(p, 0, vsize);
  }
}

#endif
