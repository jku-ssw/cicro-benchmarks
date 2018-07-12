#include "chayai.h"

#include "100-Cranium/src/cranium.h"

BENCHMARK(Cranium, train, 100, 10) {
    int i;
    float **data = (float **) malloc(sizeof(float *) * 4);
    for (i = 0; i < 4; i++) {
        data[i] = (float *) malloc(sizeof(float) * 2);
    }
    data[0][0] = 0;
    data[0][1] = 0;
    data[1][0] = 0;
    data[1][1] = 1;
    data[2][0] = 1;
    data[2][1] = 0;
    data[3][0] = 1;
    data[3][1] = 1;
    DataSet *trainingData = createDataSet(4, 2, data);
    float **classes = (float **) malloc(sizeof(float *) * 4);
    for (i = 0; i < 4; i++) {
        classes[i] = (float *) malloc(sizeof(float) * 2);
        classes[i][0] = (data[i][0] == 0 && data[i][1] == 0) || (data[i][0] == 1 && data[i][1] == 1) ? 1 : 0;
        classes[i][1] = classes[i][0] == 1 ? 0 : 1;
    }
    DataSet *trainingClasses = createDataSet(4, 2, classes);

    size_t hiddenSize2[] = {3};
    void (*hiddenActivations2[])(Matrix *) = {tanH};
    Network *network2 = createNetwork(2, 1, hiddenSize2, hiddenActivations2, 2, softmax);

    batchGradientDescent(network2, trainingData, trainingClasses, CROSS_ENTROPY_LOSS, trainingData->rows, .3, 0,
                         .01, .9, 1000, 1, 1);

    destroyDataSet(trainingData);
    destroyDataSet(trainingClasses);
    destroyNetwork(network2);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(Cranium, train);  // feed-forward artificial neural network

    RUN_BENCHMARKS(argc, argv);

    return 0;
}