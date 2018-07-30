#include "chayai.h"  // first include!

#include <assert.h>

#include "faragon-libsrt/src/svector.h"

volatile long sum_length = 0;

BENCHMARK(faragon, libsrt, 10, 1) {
    sv_t *vec = sv_alloc(sizeof(int), 0, NULL);

    for(int i = 0; i < 20000; i+=2) {
        size_t n = sv_push(&vec, &i);
        assert(n == 1);
    }

    for(int i = 0; i < 20000; i++) {
        size_t s = sv_find(vec, 0, &i);
        assert(i % 2 == 0 ? s >= 0 : s == S_NPOS);
    }

    sv_free(&vec);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(faragon, libsrt); // library for writing fast and safe C code

    RUN_BENCHMARKS(argc, argv);

    return 0;
}