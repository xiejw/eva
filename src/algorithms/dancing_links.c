// #include "dancing_links.h"

#include "adt/vec.h"
#include "stdio.h"

typedef struct {
  int id;
  int L;
  int R;
  int U;
  int D;
  int C;
} dl_node_t;

#define dlSetHeader(h, id, c) (h)[(id)].C = (c)

void newNode(vec_t(dl_node_t) h, int id) {
  assert(vecSize(h) >= id + 1);
  dl_node_t* p = &h[id];
  p->id        = id;
  p->L         = id;
  p->R         = id;
  p->U         = id;
  p->D         = id;
  p->C         = -1;  // unset.
}

void linkLR(vec_t(dl_node_t) h, int end, int id) {
  dl_node_t* p = &h[id];
  p->L         = end;
  p->R         = h[end].R;
  h[end].R     = id;
  h[p->R].L    = id;
}
void print(vec_t(dl_node_t) h, int n) {
  for (int i = 0; i < n; i++) {
    dl_node_t* p = &h[i];
    printf("%2d |  L: %2d R: %2d D: %2d U: %2d C: %2d\n", i, p->L, p->R, p->U,
           p->D, p->C);
  }
}

int main() {
  // use set size and reserve to allocate space.
  // pushBack is expensive.
  vec_t(dl_node_t) h = vecNew();
  vecReserve(h, 24 + 1);

  vecSetSize(h, 7 + 1);
  newNode(h, 0);
  for (int i = 1; i <= 7; i++) {
    newNode(h, i);
    linkLR(h, i - 1, i);
    dlSetHeader(h, i, i);
  }

  print(h, 8);

  printf("hello size %lu cap %lu\n", vecSize(h), vecCap(h));
  vecFree(h);
}
