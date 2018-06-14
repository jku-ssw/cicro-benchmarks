#ifndef CHAYAI_TESTRESULT_H
#define CHAYAI_TESTRESULT_H

#include <stdint.h>

typedef struct CHayaiBenchmarkSingleRunResult
{
    int64_t time;
#ifdef USE_PAPI
    int papiEventSet; // has to be set before benchmark run
    long long int *papiCounters; // has to be allocated before benchmark run
#endif
} CHayaiBenchmarkSingleRunResult;

typedef struct CHayaiBenchmarkResult
{
    unsigned int warmupRuns;
    unsigned int runs;
    unsigned int iterations;
    int64_t timeTotal;
    int64_t timeRunMin;
    int64_t timeRunMax;
    struct CHayaiBenchmarkSingleRunResult *singleRuns;
} CHayaiBenchmarkResult;


#endif // CHAYAI_TESTRESULT_H
