#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "adt/vec.h"
#include "algorithms/dancing_links.h"

#define SIZE 9

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef PID
#define PID 1
#endif

static const int PROLBEMS[][SIZE * SIZE] = {
    // From The Art of Computer Programming, Vol 4, Dancing Links.
    {
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
    },
    // The World's Hardest Sudoku.
    // https://gizmodo.com/can-you-solve-the-10-hardest-logic-puzzles-ever-created-1064112665
    {
        // clang-format off
        8, 0, 0,  0, 0, 0,  0, 0, 0,
        0, 0, 3,  6, 0, 0,  0, 0, 0,
        0, 7, 0,  0, 9, 0,  2, 0, 0,

        0, 5, 0,  0, 0, 7,  0, 0, 0,
        0, 0, 0,  0, 4, 5,  7, 0, 0,
        0, 0, 0,  1, 0, 0,  0, 3, 0,

        0, 0, 1,  0, 0, 0,  0, 6, 8,
        0, 0, 8,  5, 0, 0,  0, 1, 0,
        0, 9, 0,  0, 0, 0,  4, 0, 0,
        // clang-format on
    },
};

// -----------------------------------------------------------------------------
// prototypes.
// -----------------------------------------------------------------------------
typedef struct {
        int x;
        int y;
        int k;
} option_t;

static void printProblem(const int *problem);
int         searchOptions(const int *problem, vec_t(option_t) options);
static void getItemId(int i, int j, int k, int *p, int *r, int *c, int *b);

// -----------------------------------------------------------------------------
// main.
// -----------------------------------------------------------------------------
int
main()
{
        // Select and print problem
        const int *problem = PROLBEMS[PID];
        printProblem(problem);

        // Search options.
        vec_t(option_t) options = vecNew();
        vecReserve(options, 9 * 9 * 9);  // at most 9^3 options.
        int options_count = searchOptions(problem, options);

        if (DEBUG) {
                printf("total options count %d\n", options_count);
                printf("top 10 options:\n");
                for (int i = 0; i < 10 && i < options_count; i++) {
                        printf("  x %d, y %d, k %d\n", options[i].x,
                               options[i].y, options[i].k);
                }
        }

        // prepare dancing links table.
        struct dl_table_t *t = dlNew(1 + 4 * options_count + 4 * 81);
        dlAllocateItems(t, /*num_items=*/4 * 81);

        // hide all items which have been filled by the problem already.
        int item_ids[4];
        for (int x = 0; x < SIZE; x++) {
                int offset = x * SIZE;
                for (int y = 0; y < SIZE; y++) {
                        int num = problem[offset + y];
                        if (num == 0) continue;
                        getItemId(x, y, num, /*p=*/item_ids, /*r=*/item_ids + 1,
                                  /*c=*/item_ids + 2,
                                  /*b=*/item_ids + 3);
                        dlCoverCol(t, item_ids[0]);
                        dlCoverCol(t, item_ids[1]);
                        dlCoverCol(t, item_ids[2]);
                        dlCoverCol(t, item_ids[3]);
                }
        }

        // append options to the dancing links table;
        for (int i = 0; i < options_count; i++) {
                option_t *o = &options[i];
                getItemId(o->x, o->y, o->k, /*p=*/item_ids, /*r=*/item_ids + 1,
                          /*c=*/item_ids + 2,
                          /*b=*/item_ids + 3);
                dlAppendOption(t, 4, item_ids, o);
        }

        // search solution.
        vec_t(int) sols = vecNew();
        vecReserve(sols, SIZE * SIZE);

        if (dlSearchSolution(t, sols)) {
                int solution[SIZE * SIZE];
                memcpy(solution, problem, sizeof(int) * SIZE * SIZE);

                printf("Found solution:\n");
                int n = vecSize(sols);
                for (int i = 0; i < n; i++) {
                        option_t *o                  = dlNodeData(t, sols[i]);
                        solution[o->x * SIZE + o->y] = o->k;
                }
                printProblem(solution);

        } else {
                printf("no solution.\n");
        }

        vecFree(options);
        vecFree(sols);
        dlFree(t);
        return 0;
}

// -----------------------------------------------------------------------------
// Helper Methods.
// -----------------------------------------------------------------------------

// Prints the Sudoku Problem on screen.
void
printProblem(const int *problem)
{
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
//
// The argument options must have enough capacity to hold all potential options.
int
searchOptions(const int *problem, vec_t(option_t) options)
{
        int total = 0;

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

                                assert(vecCap(options) >= total + 1);
                                options[total].x = x;
                                options[total].y = y;
                                options[total].k = k;

                                total++;
                        not_a_option:
                                (void)0;
                        }
                }
        }
        vecSetSize(options, total);
        return total;
}

// For a digit k (1-based) in cell (i,j), generats four ids (1-based) for the
// following four items:
//
// - p{i,j} // pos
// - r{i,k} // row with digit
// - c{j,k} // col with digit
// - b{x,k} // box with digit where x = 3 * floor(i/3) + floor(j/3).
void
getItemId(int i, int j, int k, int *p, int *r, int *c, int *b)
{
        int x      = 3 * (i / 3) + (j / 3);
        int offset = 0;

        k = k - 1;  // k is 1 based.

        *p = i * SIZE + j + offset + 1;  // item id is 1 based.
        offset += SIZE * SIZE + 1;

        *r = i * SIZE + k + offset;
        offset += SIZE * SIZE;

        *c = j * SIZE + k + offset;
        offset += SIZE * SIZE;

        *b = x * SIZE + k + offset;
}
