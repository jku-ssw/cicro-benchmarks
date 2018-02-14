#include <stdlib.h>

#include "harness.h"

#include "paladin-t-my_basic/core/my_basic.h"

// sample/sample02.bas
#define EXAMPLE_CODE \
"e = 1000                                               \n"\
"for n = 3 to e                                         \n"\
"   m = 2                                               \n"\
"   isp = 1                                             \n"\
"   while m < n                                         \n"\
"       if n mod m = 0 then isp = 0 else m = m + 1      \n"\
"       if isp = 0 then exit                            \n"\
"   wend                                                \n"\
"next n                                                 \n"

int __attribute__ ((noinline)) test_harness(void) {
    struct mb_interpreter_t* bas = NULL;
	mb_init();
	mb_open(&bas);
	mb_load_string(bas, EXAMPLE_CODE, true);
	mb_run(bas, true);
	mb_close(&bas);
	mb_dispose();

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="paladin-t-my_basic",
        .description="Lightweight BASIC interpreter",
        .test_harness=*test_harness,
        .expected_runtime=450L
    };
    return _execute_harness(argc, argv, harness);
}