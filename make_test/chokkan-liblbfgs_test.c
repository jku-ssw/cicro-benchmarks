#include <stdio.h>

#include "harness.h"

#include "lbfgs.h"

#define N 100000

static lbfgsfloatval_t evaluate(
        void *instance,
        const lbfgsfloatval_t *x,
        lbfgsfloatval_t *g,
        const int n,
        const lbfgsfloatval_t step
)
{
    int i;
    lbfgsfloatval_t fx = 0.0;

    for (i = 0;i < n;i += 2) {
        lbfgsfloatval_t t1 = 1.0 - x[i];
        lbfgsfloatval_t t2 = 10.0 * (x[i+1] - x[i] * x[i]);
        g[i+1] = 20.0 * t2;
        g[i] = -2.0 * (x[i] * g[i+1] + t1);
        fx += t1 * t1 + t2 * t2;
    }
    return fx;
}

int __attribute__ ((noinline)) test_harness(void) {
    int i, ret = 0;
    lbfgsfloatval_t fx;
    lbfgsfloatval_t *x = lbfgs_malloc(N);
    lbfgs_parameter_t param;

    for(i = 0; i < 1000; i++) {
        /* Initialize the variables. */
        for (i = 0;i < N;i += 2) {
            x[i] = -1.2;
            x[i+1] = 1.0;
        }

        lbfgs_parameter_init(&param);


        lbfgs(N, x, &fx, evaluate, NULL, NULL, &param);
    }

    lbfgs_free(x);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="chokkan-liblbfgs",
        .description="library of Limited-memory Broyden-Fletcher-Goldfarb-Shanno",
        .test_harness=*test_harness,
        .expected_runtime=440L
    };
    return _execute_harness(argc, argv, harness);
}