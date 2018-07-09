#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "chayai.h"

#include "jtsiomb-kdtree/kdtree.h"

BENCHMARK(jtsiomb, kdtree, 100, 1) {
    void *kd = kd_create(3);

    for(int i=0; i<10000; i++) {
        float x, y, z;
        x = (i*479)%487;
        y = (i*433)%439;
        z = (i*401)%409;

        int ret = kd_insert3(kd, x, y, z, 0);
        assert(ret == 0);
    }

    for(int i=0; i<20000; i+= 10) {
        float x, y, z;
        x = (i * 479) % 487 + (i % 11);
        y = (i * 433) % 439 - (i % 7);
        z = (i * 401) % 409 + (i % 5);
        void *set = kd_nearest_range3(kd, x, y, z, 40);
        assert(set != NULL);
        kd_res_free(set);
    }

    kd_free(kd);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(jtsiomb, kdtree); // A simple C library for working with KD-Trees

    RUN_BENCHMARKS(argc, argv);

    return 0;
}