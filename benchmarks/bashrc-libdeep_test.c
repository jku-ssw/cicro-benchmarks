#include "chayai.h"  // first include!

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>

#include "bashrc-libdeep/src/globals.h"
#include "bashrc-libdeep/src/deeplearn.h"
#include "bashrc-libdeep/src/deeplearndata.h"

#define TITLE     "Concrete Slump Training"
#define DATA_FILE "bashrc-libdeep/examples/concreteslump/slump_test.data"

static char *data_file_path = NULL;

BENCHMARK(libdeep, learn, 10, 1) {
    assert(data_file_path != NULL);

    int no_of_outputs = 4;
    int output_field_index[] = { 7,8,9,10 };
    float error_threshold_percent[] = { 10.5f, 10.5f, 10.5f, 40.0f };
    unsigned int random_seed = 123;

    deeplearn learner;
    deeplearndata_read_csv(data_file_path,
                           &learner,
                           4*4, 3,
                           no_of_outputs,
                           output_field_index, 0,
                           error_threshold_percent,
                           &random_seed);

    deeplearn_set_learning_rate(&learner, 0.9f);

    deeplearn_set_pruning(&learner, 10000, 0.9f);

    deeplearn_set_dropouts(&learner, 1.1f);

    learner.history.interval = 500000;

    deeplearn_set_title(&learner, TITLE);

    while (deeplearndata_training(&learner) != 0);

    deeplearn_free(&learner);
}

void benchmark_cleanup (void) {
    unlink("training.png");
    unlink("weight_gradients_std.png");
    unlink("weight_gradients_mean.png");
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(libdeep, learn); // A deep learning library for C/C++

    // get realpath
    data_file_path = malloc(PATH_MAX*sizeof(char));
    assert(data_file_path != NULL);
    char *path = realpath(DATA_FILE, data_file_path); // TODO: support different working directories
    assert(path == data_file_path);

    // we don't want to clutter our directories
    chdir(chayai_util_get_tmp_dir());
    atexit(benchmark_cleanup);

    RUN_BENCHMARKS(argc, argv);

    free(data_file_path);

    return 0;
}
