#include "algorithms/dancing_links.h"

#include "adt/vec.h"
#include "stdio.h"

typedef struct dl_node_t {
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
} dl_node_t;

void newNode(vec_t(dl_node_t) h, int id) {
  assert(vecCap(h) >= id + 1);
  dl_node_t* p = &h[id];
  p->id        = id;
  p->L         = id;
  p->R         = id;
  p->U         = id;
  p->D         = id;
  p->C         = 0;
}

void linkLR(vec_t(dl_node_t) h, int end, int id) {
  dl_node_t* p = &h[id];
  p->L         = end;
  p->R         = h[end].R;
  h[end].R     = id;
  h[p->R].L    = id;
}

void linkUD(vec_t(dl_node_t) h, int id_c, int id) {
  dl_node_t* c = &h[id_c];
  dl_node_t* p = &h[id];
  p->C         = id_c;

  c->S += 1;

  int id_end  = c->U;
  c->U        = id;
  h[id_end].D = id;
  p->D        = id_c;
  p->U        = id_end;
}

void print(vec_t(dl_node_t) h, int n, int num_items) {
  for (int i = 0; i < n; i++) {
    dl_node_t* p = &h[i];
    if (i <= num_items)
      printf("%2d |  L: %2d R: %2d D: %2d U: %2d S: %2d\n", i, p->L, p->R, p->D,
             p->U, p->S);
    else
      printf("%2d |  L: %2d R: %2d D: %2d U: %2d C: %2d\n", i, p->L, p->R, p->D,
             p->U, p->C);
  }
}

void cover_col(vec_t(dl_node_t) h, int c) {
  h[h[c].R].L = h[c].L;
  h[h[c].L].R = h[c].R;
  // TODO
  for (int i = h[c].D; i != c; i = h[i].D) {
    for (int j = h[i].R; j != i; j = h[j].R) {
      h[h[j].D].U = h[j].U;
      h[h[j].U].D = h[j].D;
      (h[h[j].C].S)--;
    }
  }
}

void uncover_col(vec_t(dl_node_t) h, int c) {
  // TODO
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

int search(vec_t(dl_node_t) h, int k, vec_t(int) sols);

int dlSearchSolution(dl_table_t* t, vec_t(int) sols) {
  vecSetSize(sols, 0);
  return search(t->nodes, 0, sols);
}

int search(vec_t(dl_node_t) h, int k, vec_t(int) sols) {
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

// impl
//
void        dlInit(dl_table_t* h, int reserve_n);
dl_table_t* dlNew(int reserve_n) {
  dl_table_t* h = malloc(sizeof(dl_table_t));
  dlInit(h, reserve_n);
  return h;
}

void dlInit(dl_table_t* h, int reserve_n) {
  assert(reserve_n >= 0);
  h->num_items = 0;
  h->num_nodes = 1;
  h->nodes     = NULL;
  vecReserve(h->nodes, reserve_n + 1);
  vecSetSize(h->nodes, 1);
  newNode(h->nodes, 0);
}

void dlFree(dl_table_t* h) {
  if (h == NULL) return;
  vecFree(h->nodes);
  free(h);
}

void dlAllocateItems(dl_table_t* ph, int n) {
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

#define setData(h, i, msg) (h)[(i)].data = (msg)
void dlAppendOption(dl_table_t* ph, int n, int* option, void* data) {
  vec_t(dl_node_t) v = ph->nodes;
  // next pos.
  int n_id = vecSize(v);

  for (int i = 0; i < n; i++) {
    int id = n_id + i;
    newNode(v, id);
    linkUD(v, option[i], id);
    setData(v, id, data);
    if (i != 0) {
      linkLR(v, id - 1, id);
    }
  }

  int size = n + n_id;
  vecSetSize(v, size);
  ph->num_nodes = size;
}

int main() {
  // use set size and reserve to allocate space.
  // pushBack is expensive.
  dl_table_t* t      = dlNew(24);
  vec_t(dl_node_t) h = t->nodes;

  dlAllocateItems(t, 7);

#define setHeads3(h, a, b, c) ((h)[0] = (a), (h)[1] = (b), (h)[2] = (c))
#define setHeads2(h, a, b)    ((h)[0] = (a), (h)[1] = (b))

  // row 1
  int heads[3];
  setHeads3(heads, 3, 5, 6);
  dlAppendOption(t, 3, heads, "r1");

  // row 2
  setHeads3(heads, 1, 4, 7);
  dlAppendOption(t, 3, heads, "r2");

  // row 3
  setHeads3(heads, 2, 3, 6);
  dlAppendOption(t, 3, heads, "r3");

  // row 4
  setHeads2(heads, 1, 4);
  dlAppendOption(t, 2, heads, "r4");

  // row 5
  setHeads2(heads, 2, 7);
  dlAppendOption(t, 2, heads, "r5");

  // row 6
  setHeads3(heads, 4, 5, 7);
  dlAppendOption(t, 3, heads, "r6");

  print(h, t->num_nodes, t->num_items);

  vec_t(int) sols = vecNew();
  vecReserve(sols, 4);

  if (dlSearchSolution(t, sols)) {
    printf("found solution, printing...\n");
    for (int i = 0; i < vecSize(sols); i++) {
      printf("  %2d: %s\n", sols[i], t->nodes[sols[i]].data);
    }
  } else {
    printf("no solution. bye!\n");
  }

  vecFree(sols);
  dlFree(t);
}
