#include <stdlib.h>

#include "harness.h"

#include "heman.h"

int __attribute__ ((noinline)) test_harness(void) {
    heman_image* elevation = heman_generate_island_heightmap(512, 512, rand());

    heman_image* occ = heman_lighting_compute_occlusion(elevation);

    heman_image_destroy(occ);
    heman_image_destroy(elevation);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="prideout-heman",
        .description="C99 heightmap utilities",
        .test_harness=*test_harness,
        .expected_runtime=500L
    };
    return _execute_harness(argc, argv, harness);
}
