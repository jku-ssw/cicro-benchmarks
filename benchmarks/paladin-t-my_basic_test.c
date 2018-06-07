#include <stdlib.h>

#include "chayai.h"

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

BENCHMARK(paladin, my_basic, 10, 1) {
    struct mb_interpreter_t* bas = NULL;
	mb_init();
	mb_open(&bas);
	mb_load_string(bas, EXAMPLE_CODE, true);
	mb_run(bas, true);
	mb_close(&bas);
	mb_dispose();
}

int main(int argc, char** argv) {

	REGISTER_BENCHMARK(paladin, my_basic); // Lightweight BASIC interpreter

	RUN_BENCHMARKS(argc, argv);

	return 0;
}