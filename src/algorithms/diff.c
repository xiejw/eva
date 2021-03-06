#include "diff.h"

#include <stddef.h>  // ptrdiff_t
#include <stdio.h>
#include <string.h>

#include "adt/vec.h"

typedef int (*eql_fn_t)(void *lhs, void *rhs, size_t i, size_t j);

// github.com/golang/tools/blob/master/internal/lsp/diff/myers/diff.go
error_t
diffItems(struct diff_item_t *info)
{
        error_t   err    = OK;
        size_t    m      = info->lhs_size;
        size_t    n      = info->rhs_size;
        ptrdiff_t offset = m + n;
        void     *lhs    = info->lhs;
        void     *rhs    = info->rhs;

        // macro to alloc a v with enough zeros.
#define prepareV(v)                                        \
        do {                                               \
                size_t buf_size = 2 * (m + n) + 1;         \
                vecReserve((v), buf_size);                 \
                memset((v), 0, buf_size * sizeof(size_t)); \
        } while (0)

        // prepare the v.
        vec_t(size_t) v = vecNew();
        prepareV(v);

        // prepare the trace
        vec_t(vec_t(size_t)) trace = vecNew();
        vecReserve(trace, n + m + 1);
        for (size_t i = 0; i < n + m + 1; i++) {
                trace[i] = vecNew();
                prepareV(trace[i]);
        }
        vecSetSize(trace, n + m + 1);
        info->trace = trace;

        eql_fn_t eql_fn = info->is_eql;

        // Iterate through the maximum possible length of the SES (N+M).
        for (ptrdiff_t d = 0; d <= m + n; d++) {
                // k lines are represented by the equation y = x - k. We move
                // in increments of 2 because end points for even d are on even
                // k lines.
                for (ptrdiff_t k = -d; k <= d; k += 2) {
                        // At each point, we either go down or to the right. We
                        // go down if k == -d, and we go to the right if k ==
                        // d. We also prioritize the maximum x value, because
                        // we prefer deletions to insertions.
                        size_t x;
                        if (k == -d ||
                            (k != d && v[k - 1 + offset] < v[k + 1 + offset])) {
                                x = v[k + 1 + offset];  // down
                        } else {
                                x = v[k - 1 + offset] + 1;  // right
                        }

                        size_t y = x - k;

                        // Diagonal moves while we have equal contents.
                        while (x < m && y < n && eql_fn(lhs, rhs, x, y) == 1) {
                                x++;
                                y++;
                        }

                        v[k + offset] = x;

                        // return if we've exceeded the maximum values.
                        if (x == m && y == n) {
                                // Makes sure to save the state of the array
                                // before returning. copy(copyV, V) trace[d] =
                                // copyV
                                info->d = d;
                                goto exit;
                        }
                }
        }
        err = errNew("should not be reached.");

exit:
        vecFree(v);
        return err;
}

void
diffFree(struct diff_item_t *info)
{
        if (info->trace == NULL) {
                return;
        }

        size_t len = vecSize(info->trace);
        for (size_t i = 0; i < len; i++) {
                vecFree(info->trace[i]);
        }
        vecFree(info->trace);
}
