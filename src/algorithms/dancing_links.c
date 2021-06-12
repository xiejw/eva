#include "algorithms/dancing_links.h"

#include "adt/vec.h"
#include "stdio.h"

typedef struct dl_node_t dl_node_t;

// -----------------------------------------------------------------------------
// helper methods prototypes.
// -----------------------------------------------------------------------------

static void        newNode(vec_t(dl_node_t) h, int id);
static inline void linkLR(vec_t(dl_node_t) h, int end, int id);
static inline void linkUD(vec_t(dl_node_t) h, int id_c, int id);
static inline void cover_col(vec_t(dl_node_t) h, int c);
static inline void uncover_col(vec_t(dl_node_t) h, int c);
static int         search(vec_t(dl_node_t) h, int k, vec_t(int) sols);

// -----------------------------------------------------------------------------
// implementation.
// -----------------------------------------------------------------------------

dl_table_t *
dlNew(int reserve_n)
{
        assert(reserve_n >= 1);
        dl_table_t *h = malloc(sizeof(dl_table_t));
        h->num_items  = 0;
        h->num_nodes  = 1;
        h->nodes      = NULL;
        vecReserve(h->nodes, reserve_n);
        vecSetSize(h->nodes, 1);
        newNode(h->nodes, 0);
        return h;
}

void
dlFree(dl_table_t *h)
{
        if (h == NULL) return;
        vecFree(h->nodes);
        free(h);
}

void
dlAllocateItems(dl_table_t *ph, int n)
{
        vec_t(dl_node_t) v = ph->nodes;
        assert(vecSize(v) == 1);

        if (vecCap(v) < n + 1) {
                vecReserve(v, n + 1);
        }
        for (int i = 1; i <= n; i++) {
                newNode(v, i);
                linkLR(v, i - 1, i);
        }
        vecSetSize(v, n + 1);
        ph->num_items = n;
}

void
dlAppendOption(dl_table_t *ph, int n, int *col_ids, void *data)
{
        vec_t(dl_node_t) v = ph->nodes;
        int n_id           = vecSize(v);

        for (int i = 0; i < n; i++) {
                int id = n_id + i;
                newNode(v, id);
                linkUD(v, col_ids[i], id);
                v[id].data = data;
                if (i != 0) {
                        linkLR(v, id - 1, id);
                }
        }

        int size = n + n_id;
        vecSetSize(v, size);
        ph->num_nodes = size;
}

void
dlCoverCol(dl_table_t *t, int c)
{
        cover_col(t->nodes, c);
}

int
dlSearchSolution(dl_table_t *t, vec_t(int) sols)
{
        vecSetSize(sols, 0);
        return search(t->nodes, 0, sols);
}

// -----------------------------------------------------------------------------
// helper methods implementation.
// -----------------------------------------------------------------------------

void
newNode(vec_t(dl_node_t) h, int id)
{
        assert(vecCap(h) >= id + 1);
        dl_node_t *p = &h[id];
        p->id        = id;
        p->L         = id;
        p->R         = id;
        p->U         = id;
        p->D         = id;
        p->C         = 0;
}

// link the `id` into table after node `end` (horizantal double link)
void
linkLR(vec_t(dl_node_t) h, int end, int id)
{
        dl_node_t *p = &h[id];
        p->L         = end;
        p->R         = h[end].R;
        h[end].R     = id;
        h[p->R].L    = id;
}

// link the `id` into table with column head `id_c` (vertical double link)
void
linkUD(vec_t(dl_node_t) h, int id_c, int id)
{
        dl_node_t *c = &h[id_c];
        dl_node_t *p = &h[id];
        p->C         = id_c;

        c->S += 1;

        int id_end  = c->U;
        c->U        = id;
        h[id_end].D = id;
        p->D        = id_c;
        p->U        = id_end;
}

void
print(vec_t(dl_node_t) h, int n, int num_items)
{
        for (int i = 0; i < n; i++) {
                dl_node_t *p = &h[i];
                if (i <= num_items)
                        printf("%2d |  L: %2d R: %2d D: %2d U: %2d S: %2d\n", i,
                               p->L, p->R, p->D, p->U, p->S);
                else
                        printf("%2d |  L: %2d R: %2d D: %2d U: %2d C: %2d\n", i,
                               p->L, p->R, p->D, p->U, p->C);
        }
}

void
cover_col(vec_t(dl_node_t) h, int c)
{
        h[h[c].R].L = h[c].L;
        h[h[c].L].R = h[c].R;
        for (int i = h[c].D; i != c; i = h[i].D) {
                for (int j = h[i].R; j != i; j = h[j].R) {
                        h[h[j].D].U = h[j].U;
                        h[h[j].U].D = h[j].D;
                        (h[h[j].C].S)--;
                }
        }
}

void
uncover_col(vec_t(dl_node_t) h, int c)
{
        for (int i = h[c].U; i != c; i = h[i].U) {
                for (int j = h[i].L; j != i; j = h[j].L) {
                        (h[h[j].C].S)++;
                        h[h[j].D].U = j;
                        h[h[j].U].D = j;
                }
        }
        h[h[c].R].L = c;
        h[h[c].L].R = c;
}

int
search(vec_t(dl_node_t) h, int k, vec_t(int) sols)
{
        if (h[0].R == 0) {
                vecSetSize(sols, k);
                return 1;
        }

        int c = h[0].R;
        if (h[c].S == 0) {
                return 0;
        }
        if (vecCap(sols) < k) {
                vecReserve(sols, 2 * vecCap(sols));
        }

        cover_col(h, c);
        for (int r = h[c].D; r != c; r = h[r].D) {
                sols[k] = r;
                for (int j = h[r].R; j != r; j = h[j].R) {
                        cover_col(h, h[j].C);
                }
                if (search(h, k + 1, sols)) return 1;
                for (int j = h[r].L; j != r; j = h[j].L) {
                        uncover_col(h, h[j].C);
                }
        }
        uncover_col(h, c);
        return 0;
}
