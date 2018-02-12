#include <stdlib.h>

#include "harness.h"

#include "faragon-libsrt/src/svector.h"

volatile long sum_length = 0;

int __attribute__ ((noinline)) test_harness(void) {

    sv_t *vec = sv_alloc(sizeof(int), 0, NULL);

    for(int i = 0; i < 20000; i+=2) {
        sv_push(&vec, &i);
    }
    for(int i = 0; i < 20000; i++) {
        sv_find(vec, 0, &i);
    }

    sv_free(&vec);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="faragon-libsrt",
        .description=" library for writing fast and safe C code",
        .test_harness=*test_harness,
        .expected_runtime=550L
    };
    return _execute_harness(argc, argv, harness);
}
