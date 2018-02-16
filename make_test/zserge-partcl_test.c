#include <stdlib.h>

#include "harness.h"

#define TEST
#include "zserge-partcl/tcl.c"

#define TCL_BENCH "set i 0; while {< $i 20000} {if {== [- $i [* [/ $i 2] 2]] 0} {set i [+ $i 2]} {}; set i [+ $i 1]}"

int __attribute__ ((noinline)) test_harness(void) {
    struct tcl tcl;
    tcl_init(&tcl);

    tcl_eval(&tcl, TCL_BENCH, sizeof(TCL_BENCH));

    tcl_destroy(&tcl);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="zserge-partcl",
        .description="a micro Tcl implementation",
        .test_harness=*test_harness,
        .expected_runtime=300L
    };
    return _execute_harness(argc, argv, harness);
}
