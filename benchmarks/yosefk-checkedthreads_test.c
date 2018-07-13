#include <stdlib.h>
#include <assert.h>

#include "chayai.h"

#include "yosefk-checkedthreads/include/checkedthreads.h"


#define N 500000
#define MIN_PAR 10
#define SCALE 3

typedef struct _context_t {
    int* beg;
    int* end;
} context_t;

int comp(const void *a,const void *b) {
    int *x = (int *) a;
    int *y = (int *) b;
    return *x - *y;
}

void quicksort(void* ctx) {
    int *beg = ((context_t*)ctx)->beg;
    int *end = ((context_t*)ctx)->end;

    if (end-beg >= MIN_PAR) {
        int piv = *beg, l = 1, r = end-beg;
        while (l < r) {
            if (beg[l] <= piv) {
                l++;
            } else {
                int tmp = beg[l];
                beg[l] = beg[--r];
                beg[r] = tmp;
            }
        }
        int tmp = beg[0];
        beg[0] = beg[--l];
        beg[l] = tmp;

        //sort the two parts in parallel:
        context_t q0 = {beg, beg+l};
        context_t q1 = {beg+r, end};
        ct_task tasks[] = {
            {quicksort, &q0},
            {quicksort, &q1},
            {0, 0}
        };
        ct_invoke(tasks, 0);
    }
    else {
        qsort(beg, end-beg, sizeof(int), comp);
    }
}

void index_callback(int index, void* context) {
    int* array = (int*)context;
    array[index] += index*SCALE % 1087;
}

BENCHMARK(yosefk, checkedthreads, 10, 1) {
    ct_env_var env[] = {
        {"CT_SCHED", "shuffle"},
        {0, 0}
    };
    ct_init(env);

    int* array = calloc(N, sizeof(int));

    ct_for(N, index_callback, array, 0);

    assert(array[0] == 0x00 && array[20000] == 0xD7 &&  array[40000] == 0x1AE && array[50000] == 0x439);

    context_t q = {array, array+N};
    quicksort(&q);

    assert(array[0] == 0x00 && array[20000] == 0x2B &&  array[40000] == 0x56 && array[50000] == 0x6C);

    free(array);

    ct_fini();
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(yosefk, checkedthreads); // automated load balancing

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
