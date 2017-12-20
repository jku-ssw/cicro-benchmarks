#include <string.h>
#include <stdlib.h>

#include "harness.h"

#include "jtsiomb-kdtree/kdtree.h"

int __attribute__ ((noinline)) test_harness(void) {
    void *kd = kd_create(3);

    for(int i=0; i<100000; i++) {
        float x, y, z;
        x = (i*479)%487;
        y = (i*433)%439;
        z = (i*401)%409;

        kd_insert3(kd, x, y, z, 0);
    }

    for(int i=0; i<20000; i+= 10) {
        float x, y, z;
        x = (i * 479) % 487 + (i % 11);
        y = (i * 433) % 439 - (i % 7);
        z = (i * 401) % 409 + (i % 5);
        void *set = kd_nearest_range3(kd, x, y, z, 40);
        kd_res_free(set);
    }

    kd_free(kd);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="jtsiomb-kdtree",
        .description="A simple C library for working with KD-Trees",
        .test_harness=*test_harness,
        .expected_runtime=530L
    };
    return _execute_harness(argc, argv, harness);
}
