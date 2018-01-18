#include <stdlib.h>

#include "harness.h"

#include "tinyspline.h"


CTRL_POINTS = 500;
int __attribute__ ((noinline)) test_harness(void) {
    tsBSpline spline;

    ts_bspline_new(
            CTRL_POINTS,      /* number of control points */
            3,      /* dimension of each point */
            3,      /* degree of spline */
            TS_CLAMPED, /* used to hit first and last control point */
            &spline /* the spline to setup */
    );

    /* Setup control points. */
    for(int i = 0; i < 3*CTRL_POINTS;) {
        spline.ctrlp[i++] = i;
        spline.ctrlp[i++] = i*-1.0;
        spline.ctrlp[i++] = 0.0;
    }

    tsBSpline draw;
    ts_bspline_to_beziers(&spline, &draw);
    ts_bspline_free(&draw);

    tsDeBoorNet net;
    for(tsReal i = 0; i <= 1; i += 0.00001) {
        ts_bspline_evaluate(&spline, i, &net);
        ts_deboornet_free(&net);
    }
    ts_bspline_free(&spline);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="msteinbeck-tinyspline",
        .description="ANSI C library for NURBS, B-Splines, and Bézier curves",
        .test_harness=*test_harness,
        .expected_runtime=260L
    };
    return _execute_harness(argc, argv, harness);
}
