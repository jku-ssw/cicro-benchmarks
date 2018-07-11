#include <assert.h>

#include "chayai.h"

#define TEST
#include "zserge-partcl/tcl.c"

#define TCL_BENCH "set i 0; while {< $i 20000} {if {== [- $i [* [/ $i 2] 2]] 0} {set i [+ $i 2]} {}; set i [+ $i 1]}"

BENCHMARK(zserge, partcl, 10, 1) {
    struct tcl tcl;
    tcl_init(&tcl);

    int ret = tcl_eval(&tcl, TCL_BENCH, sizeof(TCL_BENCH));
    assert(ret == FNORMAL);

    tcl_destroy(&tcl);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(zserge, partcl); // a micro Tcl implementation

    RUN_BENCHMARKS(argc, argv);

    return 0;
}