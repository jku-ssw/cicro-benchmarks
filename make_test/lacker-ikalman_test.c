#include <stdlib.h>

#include "chayai.h"

#include "lacker-ikalman/gps.h"

BENCHMARK(lacker, ikalman, 10, 100) {
    KalmanFilter f = alloc_filter_velocity2d(1.0);
    for (int i = 0; i < 100; ++i) {
        update_velocity2d(f, i*0.001, i*-0.0001, 1.0);
    }

    double bearing = get_bearing(f);

    free_filter(f);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(lacker, ikalman); // Kalman filter

    RUN_BENCHMARKS(argc, argv);

    return 0;
}