#include <stdlib.h>

#include "chayai.h"

#include "tinyspline.h"


CTRL_POINTS = 500;
BENCHMARK(msteinbeck, tinyspline, 10, 1) {
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
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(msteinbeck, tinyspline); // ANSI C library for NURBS, B-Splines, and Bézier curves

    RUN_BENCHMARKS(argc, argv);

    return 0;
}