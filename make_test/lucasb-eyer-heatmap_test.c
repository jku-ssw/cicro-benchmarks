#include <string.h>
#include <stdlib.h>

#include "harness.h"

#include "lucasb-eyer-heatmap/heatmap.h"

static const size_t MAPSIZE = 2048;
static const size_t NPOINTS = 2048;
static const size_t STAMP = 128;

int __attribute__ ((noinline)) test_harness(void) {
    heatmap_stamp_t *stamp = heatmap_stamp_gen(STAMP);
    heatmap_t *hm = heatmap_new(MAPSIZE, MAPSIZE);

    for (int i = 0; i < NPOINTS; i++) {
        heatmap_add_point_with_stamp(hm, (7907*i)%MAPSIZE, (7919*i)%MAPSIZE, stamp);
    }

    unsigned char *imgbuf = malloc(sizeof(unsigned char)*MAPSIZE*MAPSIZE*4);

    heatmap_render_to(hm, heatmap_cs_default, &imgbuf[0]);

    heatmap_render_saturated_to(hm, heatmap_cs_default, 0.5f, &imgbuf[0]);

    free(imgbuf);
    heatmap_free(hm);
    heatmap_stamp_free(stamp);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="lucasb-eyer-heatmap",
        .description="High performance C heatmap generation",
        .test_harness=*test_harness,
        .expected_runtime=560L
    };
    return _execute_harness(argc, argv, harness);
}
