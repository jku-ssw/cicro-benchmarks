#include <assert.h>

#include "chayai.h"

#include "tinyspline.h"


int CTRL_POINTS = 500;
BENCHMARK(msteinbeck, tinyspline, 10, 1) {
    tsBSpline spline;

    int ret = ts_bspline_new(
            CTRL_POINTS,      /* number of control points */
            3,      /* dimension of each point */
            3,      /* degree of spline */
            TS_CLAMPED, /* used to hit first and last control point */
            &spline /* the spline to setup */
    );
    assert(ret == TS_SUCCESS);

    /* Setup control points. */
    for(int i = 0; i < 3*CTRL_POINTS;) {
        spline.ctrlp[i] = i+1;
        spline.ctrlp[i+1] = (i+2) * -1.0;
        spline.ctrlp[i+2] = 0.0;
        i += 3;
    }

    tsBSpline draw;
    ret = ts_bspline_to_beziers(&spline, &draw);
    assert(ret == TS_SUCCESS);

    ts_bspline_free(&draw);

    size_t k_sum = 0;
    tsDeBoorNet net;
    for(tsReal i = 0; i <= 1; i += 0.00001) {
        ts_bspline_evaluate(&spline, i, &net);
        k_sum += net.k;
        ts_deboornet_free(&net);
    }
    assert(k_sum == 25100751);

    ts_bspline_free(&spline);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(msteinbeck, tinyspline); // ANSI C library for NURBS, B-Splines, and Bézier curves

    RUN_BENCHMARKS(argc, argv);

    return 0;
}