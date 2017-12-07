#include "harness.h"

int __attribute__ ((noinline)) test_harness(void) {
    int x=0;
    for(int i = 0; i < 1000000; i++) {
        x++;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="wbhart-bsdnt",
        .description="a bignum library",
        .test_harness=*test_harness,
        .expected_runtime=100L
    };
    return _execute_harness(argc, argv, harness);
}
