#include <stdio.h>
#include <stdlib.h>
#include "checkedthreads.h"
#include <unistd.h>

#define N 100
#define SCALE 3

#include "check.h"

void index_callback(int index, void* context) {
    int* array = (int*)context;
    array[index] += index*SCALE;
}

int main() {
    int array[N]={0};

    ct_init(0);
    ct_for(N, index_callback, array, 0);
    print_and_check_results(array);
    ct_fini();
    return 0;
}
