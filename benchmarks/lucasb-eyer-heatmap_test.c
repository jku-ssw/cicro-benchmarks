#include <stdlib.h>
#include <assert.h>

#include "chayai.h"

#include "lucasb-eyer-heatmap/heatmap.h"

static const size_t MAPSIZE = 2048;
static const size_t NPOINTS = 2048;
static const size_t STAMP = 128;

BENCHMARK(lucasb, heatmap, 10, 1) {
    heatmap_stamp_t *stamp = heatmap_stamp_gen(STAMP);
    heatmap_t *hm = heatmap_new(MAPSIZE, MAPSIZE);

    for (int i = 0; i < NPOINTS; i++) {
        heatmap_add_point_with_stamp(hm, (7907*i)%MAPSIZE, (7919*i)%MAPSIZE, stamp);
    }

    unsigned char *imgbuf = malloc(sizeof(unsigned char)*MAPSIZE*MAPSIZE*4);

    heatmap_render_to(hm, heatmap_cs_default, imgbuf);

    heatmap_render_saturated_to(hm, heatmap_cs_default, 0.5f, imgbuf);

    assert(imgbuf[0] == 0x9E && imgbuf[1] == 0x01 && imgbuf[2] == 0x42); // TODO: could be not exact due to floating point

    free(imgbuf);
    heatmap_free(hm);
    heatmap_stamp_free(stamp);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(lucasb, heatmap); // High performance C heatmap generation

    RUN_BENCHMARKS(argc, argv);

    return 0;
}