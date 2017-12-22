#include <stdlib.h>
#include <string.h>

#include "harness.h"

#include "Themaister-libfmsynth/include/fmsynth.h"

int __attribute__ ((noinline)) test_harness(void) {
    fmsynth_t *fm = fmsynth_new(44100.0f, 64);

    for (unsigned i = 0; i < 8; i++)
    {
        fmsynth_set_parameter(fm, FMSYNTH_PARAM_MOD_TO_CARRIERS0 + i, (i + 1) & 7, 2.0f);
        fmsynth_set_parameter(fm, FMSYNTH_PARAM_FREQ_MOD, i, i + 1.0f);
        fmsynth_set_parameter(fm, FMSYNTH_PARAM_PAN, i, (i & 1) ? -0.5f : +0.5f);
        fmsynth_set_parameter(fm, FMSYNTH_PARAM_CARRIERS, i, 1.0f);
        fmsynth_set_parameter(fm, FMSYNTH_PARAM_LFO_FREQ_MOD_DEPTH, i, 0.15f);
    }

    for (unsigned i = 0; i < 64; i++)
    {
        fmsynth_note_on(fm, i + 20, 127);
    }

    float left[256];
    float right[256];
    for (unsigned i = 0; i < 100; i++)
    {
        memset(left, 0, sizeof(left));
        memset(right, 0, sizeof(right));
        fmsynth_render(fm, left, right, 256);
    }

    fmsynth_free(fm);
    
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="Themaister-libfmsynth",
        .description="FM synthesizer",
        .test_harness=*test_harness,
        .expected_runtime=580L
    };
    return _execute_harness(argc, argv, harness);
}
