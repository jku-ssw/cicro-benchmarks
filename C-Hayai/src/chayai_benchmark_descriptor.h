#ifndef CHAYAI_BENCHMARKER_DATA_H
#define CHAYAI_BENCHMARKER_DATA_H

#include <stdint.h>

#include "chayai_benchmark_result.h"

typedef struct CHayaiBenchmarkDescriptor
{
    void (*runFunction)(struct CHayaiBenchmarkSingleRunResult *result);
    const char* fixtureName;
    const char* benchmarkName;
    unsigned int runs;
    unsigned int iterations;
    unsigned int warmup;

    struct CHayaiBenchmarkDescriptor* next;
} CHayaiBenchmarkDescriptor;



#endif // CHAYAI_BENCHMARKER_DATA_H
