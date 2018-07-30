#include "chayai.h"  // first include!

#include <assert.h>

#include "heman.h"

BENCHMARK(prideout, heman, 10, 1) {
    heman_image* elevation = heman_generate_island_heightmap(512, 512, 0);

    heman_image* occ = heman_lighting_compute_occlusion(elevation);

    int width, height, nbands;
    heman_image_info(occ, &width, &height, &nbands);

    assert(width == 512 && height == 512 && nbands == 1);

    heman_image_destroy(occ);
    heman_image_destroy(elevation);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(prideout, heman); // C99 heightmap utilities

    RUN_BENCHMARKS(argc, argv);

    return 0;
}