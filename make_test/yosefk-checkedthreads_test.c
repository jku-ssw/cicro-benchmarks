#include <stdlib.h>

#include "harness.h"

#include "yosefk-checkedthreads/include/checkedthreads.h"


#define N 100
#define SCALE 3

void index_callback(int index, void* context) {
    int* array = (int*)context;
    array[index] += index*SCALE;
}

int __attribute__ ((noinline)) test_harness(void) {
    int array[N]={0};

    ct_init(0);
    ct_for(N, index_callback, array, 0);

    ct_fini();

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="yosefk-checkedthreads",
        .description="automated load balancing",
        .test_harness=*test_harness,
        .expected_runtime=380L
    };
    return _execute_harness(argc, argv, harness);
}
