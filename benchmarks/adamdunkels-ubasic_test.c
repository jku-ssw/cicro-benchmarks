#include <assert.h>

#include "chayai.h"

#include "adamdunkels-ubasic/ubasic.h"

static const char program_fibs[] ="\
20 let a = 1\n\
40 let b = 1\n\
60 for i = 0 to 126\n\
80 let b = a + b\n\
100 let a = b - a\n\
rem 120 print a, b\n\
140 next i\n\
160 end\n";

BENCHMARK(ubasic, run, 100, 100) {
    ubasic_init(program_fibs);

    do {
        ubasic_run();
    } while (!ubasic_finished());

    assert(ubasic_get_variable(0) == -59);
    assert(ubasic_get_variable(1) == -30);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(ubasic, run);  // uBASIC interpreter

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
