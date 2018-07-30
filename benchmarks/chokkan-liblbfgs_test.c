#include "chayai.h"  // first include!

#include <stddef.h>
#include <assert.h>

#include "lbfgs.h"

#define N 10000

static lbfgsfloatval_t evaluate(
        void *instance,
        const lbfgsfloatval_t *x,
        lbfgsfloatval_t *g,
        const int n,
        const lbfgsfloatval_t step
)
{
    lbfgsfloatval_t fx = 0.0;

    for (int i = 0;i < n;i += 2) {
        lbfgsfloatval_t t1 = 1.0 - x[i];
        lbfgsfloatval_t t2 = 10.0 * (x[i+1] - x[i] * x[i]);
        g[i+1] = 20.0 * t2;
        g[i] = -2.0 * (x[i] * g[i+1] + t1);
        fx += t1 * t1 + t2 * t2;
    }
    return fx;
}

BENCHMARK(liblbfgs, evaluate, 100, 5) {
    lbfgsfloatval_t fx;
    lbfgsfloatval_t *x = lbfgs_malloc(N);
    assert(x !=  NULL);

    lbfgs_parameter_t param;

    /* Initialize the variables. */
    for (int i = 0;i < N;i += 2) {
        x[i] = -1.2;
        x[i+1] = 1.0;
    }

    lbfgs_parameter_init(&param);

    int ret = lbfgs(N, x, &fx, evaluate, NULL, NULL, &param);
    assert(ret == LBFGS_SUCCESS);

    lbfgs_free(x);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(liblbfgs, evaluate); // library of Limited-memory Broyden-Fletcher-Goldfarb-Shanno

    RUN_BENCHMARKS(argc, argv);

    return 0;
}