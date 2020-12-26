#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE  9
#define DEBUG 1

// -----------------------------------------------------------------------------
// prototypes.
// -----------------------------------------------------------------------------
static void printProblem(int* problem);
static int  searchOptions(int* problem);
// static void getItemId(int i, int j, int k, int* p, int* r, int* c, int* b);

// -----------------------------------------------------------------------------
// main.
// -----------------------------------------------------------------------------
int main() {
  int problem[SIZE * SIZE] = {
      // clang-format off
      0, 0, 3,  0, 1, 0,  0, 0, 0,
      4, 1, 5,  0, 0, 0,  0, 9, 0,
      2, 0, 6,  5, 0, 0,  3, 0, 0,

      5, 0, 0,  0, 8, 0,  0, 0, 9,
      0, 7, 0,  9, 0, 0,  0, 3, 2,
      0, 3, 8,  0, 0, 4,  0, 6, 0,

      0, 0, 0,  2, 6, 0,  4, 0, 3,
      0, 0, 0,  3, 0, 0,  0, 0, 8,
      3, 2, 0,  0, 0, 7,  9, 5, 0,
      // clang-format on
  };

  printProblem(problem);
  int options_count = searchOptions(problem);

  printf("options_count %d", options_count);

  return 0;
}

// -----------------------------------------------------------------------------
// helper methods.
// -----------------------------------------------------------------------------

// Prints the Soduku Problem on screen.
void printProblem(int* problem) {
  // header
  printf("+-----+-----+-----+\n");
  for (int x = 0; x < SIZE; x++) {
    int offset = x * SIZE;
    printf("|");
    for (int y = 0; y < SIZE; y++) {
      int num = problem[offset + y];
      if (num == 0)
        printf(" ");
      else
        printf("%d", problem[offset + y]);

      if ((y + 1) % 3 != 0)
        printf(" ");
      else
        printf("|");
    }
    printf("\n");
    if ((x + 1) % 3 == 0) printf("+-----+-----+-----+\n");
  }
}

#define POS(x, y) ((x)*SIZE + (y))

// Seach all options that on (x,y) the digit k is allowed to be put there.
int searchOptions(int* problem) {
  int total = 0;
  if (DEBUG) printf("total 10 options:\n");

  for (int x = 0; x < SIZE; x++) {
    int offset = x * SIZE;

    for (int y = 0; y < SIZE; y++) {
      if (problem[offset + y] > 0) continue;  // prefilled.

      int box_x = (x / 3) * 3;
      int box_y = (y / 3) * 3;

      for (int k = 1; k <= SIZE; k++) {
        // search row
        for (int j = 0; j < SIZE; j++) {
          if (problem[j + offset] == k) {
            goto not_a_option;
          }
        }
        // search column
        for (int j = 0; j < SIZE; j++) {
          if (problem[POS(j, y)] == k) {
            goto not_a_option;
          }
        }

        // search box. static unroll
        if (problem[POS(box_x, box_y)] == k ||
            problem[POS(box_x, box_y + 1)] == k ||
            problem[POS(box_x, box_y + 2)] == k ||
            problem[POS(box_x + 1, box_y)] == k ||
            problem[POS(box_x + 1, box_y + 1)] == k ||
            problem[POS(box_x + 1, box_y + 2)] == k ||
            problem[POS(box_x + 2, box_y)] == k ||
            problem[POS(box_x + 2, box_y + 1)] == k ||
            problem[POS(box_x + 2, box_y + 2)] == k) {
          goto not_a_option;
        }

        total++;
        if (DEBUG && total < 10) {
          printf("  x %d, y %d, k %d\n", x, y, k);
        }
      not_a_option:
        (void)0;
      }
    }
  }
  if (DEBUG) printf("in total %d options\n", total);
  return total;
}

// // p{i,j}, r{i,k} c{j,k} b{x,k}  x=3 * floor(i/3) + floor(j/3)
// //
// // 1-based
// void getItemId(int i, int j, int k, int* p, int* r, int* c, int* b) {
//   int x      = 3 * (i / 3) + (j / 3);
//   int offset = 0;
//
//   k = k - 1;  // k is 1 based.
//
//   *p = i * SIZE + j + offset + 1;  // item id is 1 based.
//   offset += SIZE * SIZE;
//
//   *r = i * SIZE + k + offset + 1;  // item id is 1 based.
//   offset += SIZE * SIZE;
//
//   *c = j * SIZE + k + offset + 1;  // item id is 1 based.
//   offset += SIZE * SIZE;
//
//   *b = x * SIZE + k + offset + 1;  // item id is 1 based.
// }
