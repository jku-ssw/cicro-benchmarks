#include <stdlib.h>

#include "chayai.h"

#include "yosefk-checkedthreads/include/checkedthreads.h"


#define N 100
#define SCALE 3

void index_callback(int index, void* context) {
    int* array = (int*)context;
    array[index] += index*SCALE;
}

BENCHMARK(yosefk, checkedthreads, 10, 1) {
    int array[N]={0};

    ct_init(0);
    ct_for(N, index_callback, array, 0);

    ct_fini();
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(yosefk, checkedthreads); // automated load balancing

    RUN_BENCHMARKS(argc, argv);

    return 0;
}