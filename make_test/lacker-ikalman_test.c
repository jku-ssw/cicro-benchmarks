#include <stdlib.h>

#include "harness.h"

#include "lacker-ikalman/gps.h"

int __attribute__ ((noinline)) test_harness(void) {
    KalmanFilter f = alloc_filter_velocity2d(1.0);
    for (int i = 0; i < 100000; ++i) {
        update_velocity2d(f, i*0.001, i*-0.0001, 1.0);
    }

    double bearing = get_bearing(f);

    free_filter(f);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="lacker-ikalman",
        .description="Kalman filter",
        .test_harness=*test_harness,
        .expected_runtime=180L
    };
    return _execute_harness(argc, argv, harness);
}
