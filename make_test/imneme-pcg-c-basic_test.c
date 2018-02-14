#include "harness.h"

#include "imneme-pcg-c-basic/pcg_basic.h"

int __attribute__ ((noinline)) test_harness(void) {
    pcg32_random_t rng;

    pcg32_srandom_r(&rng, 42u, 54u);

    volatile uint32_t r;

    for(int i = 0; i < 5000000; i++) {
        r = pcg32_random_r(&rng);
        r = pcg32_random_r(&rng);
        r = pcg32_random_r(&rng);
        r = pcg32_random_r(&rng);
        r = pcg32_random_r(&rng);
        r = pcg32_random_r(&rng);
        r = pcg32_random_r(&rng);
        r = pcg32_random_r(&rng);
        r = pcg32_random_r(&rng);
        r = pcg32_random_r(&rng);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="imneme-pcg-c-basic",
        .description="PCG random number generator",
        .test_harness=*test_harness,
        .expected_runtime=270L
    };
    return _execute_harness(argc, argv, harness);
}