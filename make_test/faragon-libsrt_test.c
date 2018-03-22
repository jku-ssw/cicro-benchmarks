#include <stdlib.h>

#include "chayai.h"

#include "faragon-libsrt/src/svector.h"

volatile long sum_length = 0;

BENCHMARK(faragon, libsrt, 100, 100) {

    sv_t *vec = sv_alloc(sizeof(int), 0, NULL);

    for(int i = 0; i < 200; i+=2) {
        sv_push(&vec, &i);
    }
    for(int i = 0; i < 200; i++) {
        sv_find(vec, 0, &i);
    }

    sv_free(&vec);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(faragon, libsrt); // library for writing fast and safe C code

    RUN_BENCHMARKS(argc, argv);

    return 0;
}