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
  void* data;
};

typedef struct {
  int num_items;
  int num_nodes;
  vec_t(struct dl_node_t) nodes;
} dl_table_t;

extern dl_table_t* dlNew(int reserve_n);
extern void        dlFree(dl_table_t* h);

extern void dlAllocateItems(dl_table_t* h, int n);
extern void dlAppendOption(dl_table_t* h, int n, int* option, void* data);

extern void dlCoverCol(dl_table_t* t, int c);
extern int  dlSearchSolution(dl_table_t* h, vec_t(int) sols);
#endif
