#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>

#include "chayai_benchmark_descriptor.h"
#include "chayai_benchmark_pp.h"
#include "chayai_benchmark_result.h"
#include "chayai_console_outputter.h"

#include "chayai_benchmarker.h"


static CHayaiBenchmarkDescriptor* firstBenchmarkDescriptor = NULL;
static CHayaiBenchmarkDescriptor* lastBenchmarkDescriptor;
static unsigned int nbBenchmarksRegistered = 0;
static CHayaiOutputter* currentOutputter = NULL;

void chayai_register_benchmark(CHayaiBenchmarkDescriptor* descriptor)
{
    assert(descriptor != NULL);
    
    if (firstBenchmarkDescriptor == NULL) {
        firstBenchmarkDescriptor = descriptor;
    } else {
        lastBenchmarkDescriptor->next = descriptor;
    }
    lastBenchmarkDescriptor = descriptor;
    nbBenchmarksRegistered++;
}

void chayai_set_outputter(CHayaiOutputter* outputter)
{
    currentOutputter = outputter;
}

void chayai_set_warmup_iterations(unsigned int warmup)
{
    CHayaiBenchmarkDescriptor* currentBenchmarkDescriptor = firstBenchmarkDescriptor;

    while (currentBenchmarkDescriptor != NULL) {
        currentBenchmarkDescriptor->warmup = warmup;
        currentBenchmarkDescriptor = currentBenchmarkDescriptor->next;
    }
}

void chayai_run_benchmarks()
{
    CHayaiBenchmarkResult result;
    int64_t timeRun;

    if (currentOutputter == NULL) return;
    
    CHayaiBenchmarkDescriptor* currentBenchmarkDescriptor = firstBenchmarkDescriptor;
    
    currentOutputter->begin(nbBenchmarksRegistered);
    
    while (currentBenchmarkDescriptor != NULL) {
        result.warmupRuns = currentBenchmarkDescriptor->warmup;
        result.runs = currentBenchmarkDescriptor->runs;
        result.iterations = currentBenchmarkDescriptor->iterations;
        result.timeTotal = 0;
        result.timeRunMin = INT64_MAX;
        result.timeRunMax = INT64_MIN;
        result.singleRuns = malloc(result.runs * sizeof(int64_t));

        currentOutputter->beginBenchmark(
            currentBenchmarkDescriptor->fixtureName,
            currentBenchmarkDescriptor->benchmarkName,
            currentBenchmarkDescriptor->warmup,
            currentBenchmarkDescriptor->runs,
            currentBenchmarkDescriptor->iterations);

        for(unsigned int i=0; i < result.warmupRuns; i++) {
            currentBenchmarkDescriptor->runFunction();
        }

        for(unsigned int i=0; i < result.runs; i++) {
            timeRun = currentBenchmarkDescriptor->runFunction();
            result.timeTotal += timeRun;
            result.singleRuns[i] = timeRun;
            if (result.timeRunMin > timeRun) result.timeRunMin = timeRun;
            if (result.timeRunMax < timeRun) result.timeRunMax = timeRun;
        }
        
        
        currentOutputter->endBenchmark(
            currentBenchmarkDescriptor->fixtureName,
            currentBenchmarkDescriptor->benchmarkName,
            &result);

        free(result.singleRuns);

        currentBenchmarkDescriptor = currentBenchmarkDescriptor->next;
    }
    
    currentOutputter->end(nbBenchmarksRegistered);
}
