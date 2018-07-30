#include "chayai.h"  // first include!

#include <stdlib.h>


#define SORT_NAME sorter
#define SORT_TYPE int64_t

/* You can redefine the comparison operator.
   The default is
#define SORT_CMP(x, y)  ((x) < (y) ? -1 : ((x) == (y) ? 0 : 1))
   but the one below is often faster for integer types.
*/
#define SORT_CMP(x, y) (x - y)

#include "swenson-sort/sort.h"

#define SEED 123
#define SIZE 2048
#define RUNS 64

static void fill(int64_t *arr, const int size) {
    int i;

    for (i = 0; i < size; i++) {
        arr[i] = lrand48();
    }
}

BENCHMARK(swenson, sort, 10, 1) {
    int64_t arr[SIZE];
    int64_t dst[SIZE];
    srand48(SEED);

    for (int i = 0; i < RUNS; i++) {
        fill(arr, SIZE);
        memcpy(dst, arr, sizeof(int64_t) * SIZE);

        sorter_quick_sort(dst, SIZE);
    }

    for (int i = 0; i < RUNS; i++) {
        fill(arr, SIZE);
        memcpy(dst, arr, sizeof(int64_t) * SIZE);

        sorter_selection_sort(dst, SIZE);
    }
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(swenson, sort); // Sorting routine implementations in "template" C

    RUN_BENCHMARKS(argc, argv);

    return 0;
}