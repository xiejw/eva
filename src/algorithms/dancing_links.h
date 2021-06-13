#ifndef DANCING_LINK_
#define DANCING_LINK_

#include "adt/vec.h"

struct dl_node_t {
        int id;
        int L;
        int R;
        int U;
        int D;
        union {
                int C;
                int S;
        };
        void *data;
};

struct dl_table_t {
        int num_items;
        int num_nodes;
        vec_t(struct dl_node_t) nodes;
};

extern struct dl_table_t *dlNew(int reserve_n);
extern void               dlFree(struct dl_table_t *h);

extern void dlAllocateItems(struct dl_table_t *h, int n);
extern void dlAppendOption(struct dl_table_t *h, int n, int *col_ids,
                           void *data);

extern void dlCoverCol(struct dl_table_t *t, int c);
extern int  dlSearchSolution(struct dl_table_t *h, vec_t(int) sols);

#define dlNodeData(t, id) ((t)->nodes[id].data)
#endif
