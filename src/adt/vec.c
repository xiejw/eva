#include "adt/vec.h"

#include <assert.h>
#include <stdio.h>

// -----------------------------------------------------------------------------
// implementation.
// -----------------------------------------------------------------------------

error_t
_vecReserve(size_t **vec, size_t new_cap, size_t unit_size)
{
        const size_t new_s = (new_cap)*unit_size + 2 * sizeof(size_t);

        if (*vec) {
                size_t *new_p = realloc(&(*vec)[-2], new_s);
                if (new_p == NULL) return errMalloc();
                new_p[1] = new_cap;  // size is unchanged.
                *vec     = (void *)(new_p + 2);
        } else {
                size_t *p = malloc(new_s);
                if (p == NULL) return errMalloc();
                p[0] = 0;
                p[1] = new_cap;
                *vec = (void *)(p + 2);
        };
        return OK;
}
