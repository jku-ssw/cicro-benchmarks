#include "chayai.h"  // first include!

#include <stddef.h>
#include <assert.h>

#include "codeplea-tinyexpr/tinyexpr.h"

BENCHMARK(codeplea, tinyexpr, 100, 1) {
    double x = 3, y = 4, a = 2, b = 10, c = -1;
    te_variable vars[] = {{"x", &x}, {"y", &y}, {"a", &a}, {"b", &b}, {"c", &c}};

    for(int i = 0; i < 10000; i++) {
        /* Compile the expression with variables. */
        int err = 0;
        te_expr *expr = te_compile("sqrt(x^2+y^2)+a+b+c*(sqrt(x^2+y^2)+a+b+c*(sqrt(x^2+y^2)+a+b+c))", vars, 5, &err);
        assert(expr != NULL && err == 0);

        a = i;
        const double h1 = te_eval(expr);
        x = h1;

        te_free(expr);
    }
    assert((long)x == 50085007);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(codeplea, tinyexpr); // recursive descent expression parser

    RUN_BENCHMARKS(argc, argv);

    return 0;
}