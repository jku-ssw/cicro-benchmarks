#include "chayai.h"  // first include!

#include "codeplea-genann/genann.h"

BENCHMARK(codeplea, genann, 10, 1) {
    // based on example1.c

    /* Input and expected out data */
    const double input[8][3] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
    const double output[8] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7};
    int i;

    /* New network with 2 inputs,
     * 10 hidden layer of 50 neurons,
     * and 1 output. */
    genann *ann = genann_init(3, 10, 50, 1);

    /* Train on the four labeled data points many times. */
    for (i = 0; i < 300; ++i) {
        genann_train(ann, input[0], output + 0, 3);
        genann_train(ann, input[1], output + 1, 3);
        genann_train(ann, input[2], output + 2, 3);
        genann_train(ann, input[3], output + 3, 3);
        genann_train(ann, input[4], output + 4, 4);
        genann_train(ann, input[5], output + 5, 5);
        genann_train(ann, input[6], output + 6, 6);
        genann_train(ann, input[7], output + 7, 7);
    }

    genann_free(ann);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(codeplea, genann); // simple neural network library in ANSI C

    RUN_BENCHMARKS(argc, argv);

    return 0;
}