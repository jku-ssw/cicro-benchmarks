#include "harness.h"

#include "rxi-vec/src/vec.h"

int intptrcmp(const void *a_, const void *b_) {
    const int *a = a_, *b = b_;
    return *a < *b ? -1 : *a > *b;
}

int __attribute__ ((noinline)) test_harness(void) {
    vec_int_t v;
    vec_init(&v);

    for(int i = 0; i < 10; i++) {
        // insert elements
        for(int i = 0; i < 100000; i++) {
            vec_push(&v, i);
        }

        // delete some elementss
        for(int i = 0; i < 1000; i++) {
            vec_splice(&v, 42 + i*10, 42);
        }

        // sort
        vec_sort(&v, intptrcmp);

        // reverse
        vec_reverse(&v);
    }

    vec_deinit(&v);
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="rxi-vec",
        .description="dynamic array implementation",
        .test_harness=*test_harness,
        .expected_runtime=580L
    };
    return _execute_harness(argc, argv, harness);
}