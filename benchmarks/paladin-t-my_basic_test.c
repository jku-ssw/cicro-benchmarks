#include "chayai.h"  // first include!

#include <stddef.h>
#include <assert.h>

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
	int ret;
	struct mb_interpreter_t* bas = NULL;

	ret = mb_init();
	assert(ret == MB_FUNC_OK);

	ret = mb_open(&bas);
	assert(ret == MB_FUNC_OK);

	ret = mb_load_string(bas, EXAMPLE_CODE, true);
	assert(ret == MB_FUNC_OK);

	ret = mb_run(bas, true);
	assert(ret == MB_FUNC_OK);

	ret = mb_close(&bas);
	assert(ret == MB_FUNC_OK);

	ret = mb_dispose();
	assert(ret == MB_FUNC_OK);
}

int main(int argc, char** argv) {

	REGISTER_BENCHMARK(paladin, my_basic); // Lightweight BASIC interpreter

	RUN_BENCHMARKS(argc, argv);

	return 0;
}