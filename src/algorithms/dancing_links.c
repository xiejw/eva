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

  // row 1

  newNode(h, n);
  linkUD(h, 3, n);
  n++;

  newNode(h, n);
  linkUD(h, 5, n);
  linkLR(h, n - 1, n);
  n++;

  newNode(h, n);
  linkUD(h, 6, n);
  linkLR(h, n - 1, n);
  n++;

  // row 2
  newNode(h, n);
  linkUD(h, 1, n);
  n++;

  newNode(h, n);
  linkUD(h, 4, n);
  linkLR(h, n - 1, n);
  n++;

  newNode(h, n);
  linkUD(h, 7, n);
  linkLR(h, n - 1, n);
  n++;

  // row 3
  newNode(h, n);
  linkUD(h, 2, n);
  n++;

  newNode(h, n);
  linkUD(h, 3, n);
  linkLR(h, n - 1, n);
  n++;

  newNode(h, n);
  linkUD(h, 6, n);
  linkLR(h, n - 1, n);
  n++;

  // row 4
  newNode(h, n);
  linkUD(h, 1, n);
  n++;

  newNode(h, n);
  linkUD(h, 4, n);
  linkLR(h, n - 1, n);
  n++;

  // row 5
  newNode(h, n);
  linkUD(h, 2, n);
  n++;

  newNode(h, n);
  linkUD(h, 7, n);
  linkLR(h, n - 1, n);
  n++;

  // row 6
  newNode(h, n);
  linkUD(h, 4, n);
  n++;

  newNode(h, n);
  linkUD(h, 5, n);
  linkLR(h, n - 1, n);
  n++;

  newNode(h, n);
  linkUD(h, 7, n);
  linkLR(h, n - 1, n);
  n++;

  vecSetSize(h, n);
  print(h, n, 7);

  printf("hello size %lu cap %lu\n", vecSize(h), vecCap(h));
  vecFree(h);
}
