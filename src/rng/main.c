#include <inttypes.h>  // PRIu64
#include <stdio.h>

#include "rng64.h"
#include "rng64normal.h"

#define SIZE 4

int main() {
  rng64t* rng = rng64Create(456L);
  double  arr[SIZE];
  printf("rng int: %" PRIu64 "\n", rng64NextInt64(rng));

  rng64StdNormal(rng, SIZE, arr);
  printf("rng std normal:");
  for (int i = 0; i < SIZE; i++) {
    printf(" %f,", arr[i]);
  }
  printf("\n");
  rng64Free(rng);
  return 0;
}
