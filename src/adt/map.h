// Copyright (c) 2020 xiejw
// Copyright (c) 2014 rxi (github.com/rxi/map)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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

#define mapSet(m, k, v)  _MAP_SET_IMPL(m, k, v)  // return error_t
#define mapGet(m, k)     _MAP_GET_IMPL(m, k)  // return NULL-able point to value.
#define mapReserve(m, n) _MAP_RESERVE_IMPL(m, n)  // return error_t

#define MAP_FOREACH(m, k, v) _MAP_FOREACH_IMPL(m, k, v)

// -----------------------------------------------------------------------------
// private prototype.
// -----------------------------------------------------------------------------

typedef struct {
  unsigned           bucketidx;
  struct map_node_t* node;
} map_iter_t;

extern void    _mapFree(map_base_t*);
extern error_t _mapSet(map_base_t* m, const char* key, void* value, int vsize);
extern void*   _mapGet(map_base_t* m, const char* key);
extern error_t _mapResize(map_base_t* m, int nbuckets);
extern int     _mapNext(void*, map_iter_t*, const char**, void**);

static inline error_t _mapInit(_mut_ void** m, int vsize) {
  if (*m == NULL) {
    void* p = malloc(vsize);
    if (p == NULL) return errMalloc();
    *m = p;
    memset(p, 0, vsize);
  }
  return OK;
}

static inline int power_ceiling(int x) {
  if (x > 1) {
    int power = 2;
    x--;
    while (x >>= 1) power <<= 1;
    return power;
  }
  return 1;
}

#define _MAP_FOREACH_IMPL(m, k, v) \
  for (map_iter_t iter = {-1, NULL}; _mapNext((m), &iter, (k), (void**)(v));)

#define _MAP_RESERVE_IMPL(m, n)             \
  (_mapInit((void*)(&(m)), sizeof(*(m))) || \
   ((n <= mapNBuckets((m))) ? OK : _mapResize(&(m)->base, power_ceiling(n))))

#define _MAP_SET_IMPL(m, k, v)              \
  (_mapInit((void*)(&(m)), sizeof(*(m))) || \
   ((m)->tmp = (v), _mapSet(&(m)->base, (k), &(m)->tmp, sizeof((m)->tmp))))

#define _MAP_GET_IMPL(m, k) \
  ((m) != NULL ? ((m)->ref = _mapGet(&(m)->base, (k))) : NULL)

#endif
