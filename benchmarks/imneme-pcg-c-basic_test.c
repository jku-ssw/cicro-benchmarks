#include "chayai.h"  // first include!

#include <assert.h>

#include "imneme-pcg-c-basic/pcg_basic.h"

BENCHMARK(imneme, pcg_random, 100, 10) {
    pcg32_random_t rng;

    pcg32_srandom_r(&rng, 42u, 54u);

    volatile uint32_t r;

    for(int i = 0; i < 50000; i++) {
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

    assert(r == 3911378973);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(imneme, pcg_random); // PCG random number generator

    RUN_BENCHMARKS(argc, argv);

    return 0;
}