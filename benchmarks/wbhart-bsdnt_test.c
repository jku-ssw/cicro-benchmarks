#include "chayai.h"  // first include!

#include "wbhart-bsdnt/nn.h"
#include "wbhart-bsdnt/test.h"

BENCHMARK(wbhart, bsdnt, 100, 1) {
    rand_t state;
    randinit(&state);
    int result = 1;
    nn_t a, q, r1, r2;
    len_t m, n;
    word_t d, ci, r, rem1, rem2;
    preinv1_t inv;

    /* test that a = q * d + r */
    for(int i = 0; i < 10000; i++){
        randoms_upto(100, ANY, state, &m, NULL);

        randoms_of_len(m, ANY, state, &r1, &a, &q, NULL);

        randoms(NORMALISED, state, &d, NULL);

        inv = precompute_inverse1(d);

        r = nn_divrem1_preinv(q, a, m, d, inv);

        ci = nn_mul1_c(r1, q, m, d, r);

        result = (nn_equal_m(r1, a, m) && ci == 0);
    }
    gc_cleanup(); // Run GC
    randclear(state);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(wbhart, bsdnt); // a bignum library

    RUN_BENCHMARKS(argc, argv);

    return 0;
}