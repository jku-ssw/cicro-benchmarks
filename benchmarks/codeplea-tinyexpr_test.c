#include <stdlib.h>
#include <assert.h>

#include "chayai.h"

#include "codeplea-tinyexpr/tinyexpr.h"

BENCHMARK(codeplea, tinyexpr, 100, 1) {
    double x, y, a, b, c;
    /* Store variable names and pointers. */
    te_variable vars[] = {{"x", &x}, {"y", &y}, {"a", &a}, {"b", &b}, {"c", &c}};

    int err = 0;
    /* Compile the expression with variables. */
    te_expr *expr = te_compile("sqrt(x^2+y^2)+a+b+c*(sqrt(x^2+y^2)+a+b+c*(sqrt(x^2+y^2)+a+b+c))", vars, 5, &err);
    assert(expr != NULL && err == 0);

    x = 3; y = 4; a = 0; b = 0; c = 0;
    for(int i = 0; i < 5000; i++) {
        a = i;
        const double h1 = te_eval(expr);
        x = h1;
    }

    assert((long)x == 12497511);

    te_free(expr);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(codeplea, tinyexpr); // recursive descent expression parser

    RUN_BENCHMARKS(argc, argv);

    return 0;
}