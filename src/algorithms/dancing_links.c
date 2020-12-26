// #include "dancing_links.h"

#include "adt/vec.h"
#include "stdio.h"

typedef struct {
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

void print(vec_t(dl_node_t) h, int n, int num_heads) {
  for (int i = 0; i < n; i++) {
    dl_node_t* p = &h[i];
    if (i <= num_heads)
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

void search(vec_t(dl_node_t) h, int k, vec_t(int) sols) {
  if (h[0].R == 0) {
    printf("found solution, printing...\n");
    for (int i = 0; i < k; i++) {
      printf("  %2d: %s\n", sols[i], h[sols[i]].data);
    }
    return;
  }

  int c = h[0].R;
  if (h[c].S == 0) {
    return;
  }
  cover_col(h, c);
  for (int r = h[c].D; r != c; r = h[r].D) {
    assert(vecCap(sols) >= k);
    sols[k] = r;
    for (int j = h[r].R; j != r; j = h[j].R) {
      cover_col(h, h[j].C);
    }
    search(h, k + 1, sols);
    for (int j = h[r].L; j != r; j = h[j].L) {
      uncover_col(h, h[j].C);
    }
  }
  uncover_col(h, c);
}

int main() {
  // use set size and reserve to allocate space.
  // pushBack is expensive.
  vec_t(dl_node_t) h = vecNew();
  vecReserve(h, 24 + 1);

  int n = 8;

  newNode(h, 0);
  for (int i = 1; i <= 7; i++) {
    newNode(h, i);
    linkLR(h, i - 1, i);
  }

#define setData(h, i, msg) (h)[(i)].data = (msg)

  // row 1

  newNode(h, n);
  linkUD(h, 3, n);
  setData(h, n, "r1");
  n++;

  newNode(h, n);
  linkUD(h, 5, n);
  linkLR(h, n - 1, n);
  setData(h, n, "r1");
  n++;

  newNode(h, n);
  linkUD(h, 6, n);
  linkLR(h, n - 1, n);
  setData(h, n, "r1");
  n++;

  // row 2
  newNode(h, n);
  linkUD(h, 1, n);
  setData(h, n, "r2");
  n++;

  newNode(h, n);
  linkUD(h, 4, n);
  linkLR(h, n - 1, n);
  setData(h, n, "r2");
  n++;

  newNode(h, n);
  linkUD(h, 7, n);
  linkLR(h, n - 1, n);
  setData(h, n, "r2");
  n++;

  // row 3
  newNode(h, n);
  linkUD(h, 2, n);
  setData(h, n, "r3");
  n++;

  newNode(h, n);
  linkUD(h, 3, n);
  linkLR(h, n - 1, n);
  setData(h, n, "r3");
  n++;

  newNode(h, n);
  linkUD(h, 6, n);
  linkLR(h, n - 1, n);
  setData(h, n, "r3");
  n++;

  // row 4
  newNode(h, n);
  linkUD(h, 1, n);
  setData(h, n, "r4");
  n++;

  newNode(h, n);
  linkUD(h, 4, n);
  linkLR(h, n - 1, n);
  setData(h, n, "r4");
  n++;

  // row 5
  newNode(h, n);
  linkUD(h, 2, n);
  setData(h, n, "r5");
  n++;

  newNode(h, n);
  linkUD(h, 7, n);
  linkLR(h, n - 1, n);
  setData(h, n, "r5");
  n++;

  // row 6
  newNode(h, n);
  linkUD(h, 4, n);
  setData(h, n, "r6");
  n++;

  newNode(h, n);
  linkUD(h, 5, n);
  linkLR(h, n - 1, n);
  setData(h, n, "r6");
  n++;

  newNode(h, n);
  linkUD(h, 7, n);
  linkLR(h, n - 1, n);
  setData(h, n, "r6");
  n++;

  vecSetSize(h, n);
  print(h, n, 7);

  vec_t(int) sols;
  vecReserve(sols, 8);
  search(h, 0, sols);

  vecFree(sols);

  printf("hello size %lu cap %lu\n", vecSize(h), vecCap(h));
  vecFree(h);
}
