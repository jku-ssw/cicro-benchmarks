#include "harness.h"

#include "wbhart-bsdnt/nn.h"
#include "wbhart-bsdnt/test.h"

int __attribute__ ((noinline)) test_harness(void) {
    rand_t state;
    randinit(&state);
    int result = 1;
   nn_t a, q, r1, r2;
   len_t m, n;
   word_t d, ci, r, rem1, rem2;
   preinv1_t inv;

   /* test that a = q * d + r */
   for(int i = 0; i < 100000; i++){
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
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="wbhart-bsdnt",
        .description="a bignum library",
        .test_harness=*test_harness,
        .expected_runtime=250L
    };
    return _execute_harness(argc, argv, harness);
}
