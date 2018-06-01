#ifndef CHAYAI_TESTRESULT_H
#define CHAYAI_TESTRESULT_H

#include <stdint.h>

typedef struct CHayaiBenchmarkSingleRunResult
{
    int64_t time;
    int64_t instructions;
    int64_t cycles;
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
