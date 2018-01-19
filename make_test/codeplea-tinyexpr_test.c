#include <stdlib.h>

#include "harness.h"

#include "codeplea-tinyexpr/tinyexpr.h"

int __attribute__ ((noinline)) test_harness(void) {
    double x, y, a, b, c;
    /* Store variable names and pointers. */
    te_variable vars[] = {{"x", &x}, {"y", &y}, {"a", &a}, {"b", &b}, {"c", &c}};

    int err;
    /* Compile the expression with variables. */
    te_expr *expr = te_compile("sqrt(x^2+y^2)+a+b+c*(sqrt(x^2+y^2)+a+b+c*(sqrt(x^2+y^2)+a+b+c))", vars, 2, &err);

    x = 3; y = 4; a = 0; b = 0; c = 0;
    for(int i = 0; i < 100000000; i++) {
        a = i;
        const double h1 = te_eval(expr); /* Returns 5. */
        x = h1;
    }

    te_free(expr);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="codeplea-tinyexpr",
        .description="recursive descent expression parser,",
        .test_harness=*test_harness,
        .expected_runtime=340L
    };
    return _execute_harness(argc, argv, harness);
}
