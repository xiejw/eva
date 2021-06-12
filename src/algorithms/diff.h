#ifndef DIFF_H_
#define DIFF_H_

#include <stdlib.h>  // size_t

#include "adt/vec.h"
#include "base/error.h"

struct diff_item_t {
        // filled by caller.
        void  *lhs;
        void  *rhs;
        size_t lhs_size;
        size_t rhs_size;
        int (*is_eql)(void *lhs, void *rhs, size_t i, size_t j);

        // filled with results.
        size_t d;
        vec_t(vec_t(size_t)) trace;
};

extern error_t diffItems(struct diff_item_t *);
extern void    diffFree(struct diff_item_t *);

#endif
