#include "chayai.h"

#include "adamdunkels-ubasic/ubasic.h"

static const char program[] = "\
05 let s = 0            \n\
10 gosub 110            \n\
20 for i = 0 to 126     \n\
35 let s = s + i        \n\
36 let s = s - i        \n\
40 next i               \n\
60 end                  \n\
110 return              \n";

BENCHMARK(ubasic, run, 10, 100) {
    ubasic_init(program);

    do {
        ubasic_run();
    } while (!ubasic_finished());
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(ubasic, run);  // uBASIC interpreter

    RUN_BENCHMARKS(argc, argv);

    return 0;
}