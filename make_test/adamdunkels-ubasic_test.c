#include "harness.h"

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

int __attribute__ ((noinline)) test_harness(void) {
    for (int i = 0; i < 1000; i++) {

        ubasic_init(program);

        do {
            ubasic_run();
        } while (!ubasic_finished());
    }
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="adamdunkels-ubasic",
        .description="uBASIC interpreter",
        .test_harness=*test_harness,
        .expected_runtime=300L
    };
    return _execute_harness(argc, argv, harness);
}