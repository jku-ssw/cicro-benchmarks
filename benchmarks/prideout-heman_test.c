#include <stdlib.h>

#include "chayai.h"

#include "heman.h"

BENCHMARK(prideout, heman, 10, 1) {
    heman_image* elevation = heman_generate_island_heightmap(512, 512, rand());

    heman_image* occ = heman_lighting_compute_occlusion(elevation);

    heman_image_destroy(occ);
    heman_image_destroy(elevation);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(prideout, heman); // C99 heightmap utilities

    RUN_BENCHMARKS(argc, argv);

    return 0;
}