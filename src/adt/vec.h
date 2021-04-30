#ifndef VEC_H_
#define VEC_H_

#include <assert.h>  // assert
#include <stdlib.h>  // free

#include "base/defs.h"

// -----------------------------------------------------------------------------
// Design.
//
// +----+----+------
// |size|cap |buf
// +----+----+------
//           |
//  -2   -1   `-- ptr
//
// - Fast assessing. Instead of a field lookup, `ptr` points to the buf head so
//     ptr[x] can be used. With proper vecReserve, the address is safe.
// - Lazy initialization. vecPushBack or vecReserve allocate the real memory.
//     The vec must be initialized as NULL.
// - Dynamic growth. If the space is not enough, buf will be expanded to hold
//     more elements in future.
// - Iteration: for(int i = 0; i < vecSize(v); i++) { v[i] };
// - Fast modification. Reserve a proper cap. Sets all values directly. Calls
//     vecSetSize to corrects the size. Use with caution.
//
// -----------------------------------------------------------------------------
// Onwership.
//
// 1. container `vec` does not own the ownership of each elements. So caller
//    should free them.
// 2. Must call vecFree to release the memory, used by vec, on heap.
//
// -----------------------------------------------------------------------------
// Caveats.
//
// 1. As the buf might be re-allocated (for growth), pass &vec for
//    modificaitons.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// public macros.
// -----------------------------------------------------------------------------

#define vec_t(type)  type*
#define vecNew()     NULL
#define vecFree(vec) _VEC_FREE_IMPL(vec)

#define vecSize(vec)  ((vec) ? ((size_t*)vec)[-2] : (size_t)0)
#define vecCap(vec)   ((vec) ? ((size_t*)vec)[-1] : (size_t)0)
#define vecEmpty(vec) (vecSize(v) == 0)

#define vecSetSize(vec, new_s) _VEC_SET_SIZE_IMPL(vec, new_s)  // return error_t
#define vecReserve(vec, count) _VEC_RESERVE_IMPL(vec, count)   // return error_t
#define vecPushBack(vec, v)    _VEC_PUSH_BACK_IMPL(vec, v)     // return error_t

// -----------------------------------------------------------------------------
// private prototype.
// -----------------------------------------------------------------------------

#define _VEC_FREE_IMPL(vec)                         \
        do {                                        \
                if (vec) free(&((size_t*)vec)[-2]); \
        } while (0)

#define _VEC_SET_SIZE_IMPL(vec, new_s) \
        ((vec) ? (((size_t*)vec)[-2] = (new_s), OK) : ENOTEXIST)

#define _VEC_RESERVE_IMPL(vec, count) \
        _vecReserve((size_t**)(&vec), count, sizeof(*(vec)))

#define _VEC_PUSH_BACK_IMPL(vec, v)                      \
        (_vecGrow((size_t**)(&(vec)), sizeof(*(vec))) || \
         (((vec)[((size_t*)(vec))[-2]] = (v)), ((size_t*)(vec))[-2]++, OK))

#define VEC_INIT_BUF_SIZE 16

extern error_t _vecReserve(_mut_ size_t** vec, size_t new_cap,
                           size_t unit_size);

static inline error_t
_vecGrow(_mut_ size_t** vec, size_t unit_size)
{
        if (!*vec) {
                return _vecReserve(vec, VEC_INIT_BUF_SIZE, unit_size);
        }

        const size_t cap  = (*vec)[-1];
        const size_t size = (*vec)[-2];
        assert(size <= cap);
        if (cap != size) return OK;
        return _vecReserve(vec, 2 * cap, unit_size);
}

#endif
