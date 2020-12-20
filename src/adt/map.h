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
// Private prototype.
// -----------------------------------------------------------------------------

struct map_node_t;

typedef struct {
  struct map_node_t** buckets;
  unsigned            nbuckets, nnodes;
} map_base_t;

static inline void _mapGrow(_mut_ void** m, int vsize) {
  if (*m) {
  } else {
    void* p = malloc(vsize);
    *m      = p;
    memset(p, 0, vsize);
  }
}

static inline void _mapFree(void* m) {
  if (m == NULL) return;
  free(m);
}

// -----------------------------------------------------------------------------
// Public macros.
// -----------------------------------------------------------------------------

#define map_t(type)  \
  struct {           \
    map_base_t base; \
    type*      ref;  \
  }*
#define mapNew() NULL
#define mapSet(m, k, v)                    \
  do {                                     \
    _mapGrow((void*)(&(m)), sizeof(*(m))); \
  } while (0)
#define mapFree(m) _mapFree(m)

#endif
