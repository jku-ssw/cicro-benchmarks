#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#include "chayai_benchmark_descriptor.h"
#include "chayai_benchmark_pp.h"
#include "chayai_benchmark_result.h"
#include "chayai_console_outputter.h"

#include "chayai_benchmarker.h"

#ifdef USE_PAPI
#include <papi.h>
#endif

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

void chayai_run_benchmarks(CHayaiArguments* arguments)
{
    CHayaiBenchmarkResult result;

#ifdef USE_PAPI
    int papiValuesCount = 0;
    if(arguments->papiEventSet != PAPI_NULL) {
        int retval;
        if((retval = PAPI_list_events(arguments->papiEventSet, NULL, &papiValuesCount)) != PAPI_OK) {
            fprintf(stderr, "PAPI error %d: %s (%s:%d)\n", retval, PAPI_strerror(retval), __FILE__, __LINE__);
            exit(2);
        }
    }
#endif

    if (currentOutputter == NULL) return;
    
    CHayaiBenchmarkDescriptor* currentBenchmarkDescriptor = firstBenchmarkDescriptor;
    
    currentOutputter->begin(nbBenchmarksRegistered);
    
    while (currentBenchmarkDescriptor != NULL) {
        result.warmupRuns = currentBenchmarkDescriptor->warmup;
        result.runs = arguments->benchmarkIterations > 0 ? arguments->benchmarkIterations : currentBenchmarkDescriptor->runs;
        result.iterations = currentBenchmarkDescriptor->iterations;
        result.timeTotal = 0;
        result.timeRunMin = INT64_MAX;
        result.timeRunMax = INT64_MIN;
        result.singleRuns = malloc(result.runs * sizeof(struct CHayaiBenchmarkSingleRunResult));

        currentOutputter->beginBenchmark(
            currentBenchmarkDescriptor->fixtureName,
            currentBenchmarkDescriptor->benchmarkName,
            result.warmupRuns,
            result.runs,
            currentBenchmarkDescriptor->iterations);

        for(unsigned int i=0; i < result.warmupRuns; i++) {
            struct CHayaiBenchmarkSingleRunResult singleRun;
#ifdef USE_PAPI
            singleRun.papiEventSet = arguments->papiEventSet;
            singleRun.papiCounters = papiValuesCount == 0 ? NULL : malloc(papiValuesCount*sizeof(int64_t));
#endif
            currentBenchmarkDescriptor->runFunction(&singleRun);
#ifdef USE_PAPI
            if(singleRun.papiCounters != NULL) {
                free(singleRun.papiCounters);
            }
#endif
        }

        for(unsigned int i=0; i < result.runs; i++) {
            struct CHayaiBenchmarkSingleRunResult singleRun;
#ifdef USE_PAPI
            singleRun.papiEventSet = arguments->papiEventSet;
            singleRun.papiCounters = papiValuesCount == 0 ? NULL : malloc(papiValuesCount*sizeof(int64_t));
            memset(singleRun.papiCounters, 0, papiValuesCount*sizeof(int64_t));
#endif
            currentBenchmarkDescriptor->runFunction(&singleRun);

            result.singleRuns[i] = singleRun;

            result.timeTotal += singleRun.time;
            if (result.timeRunMin > singleRun.time) result.timeRunMin = singleRun.time;
            if (result.timeRunMax < singleRun.time) result.timeRunMax = singleRun.time;
        }

        currentOutputter->endBenchmark(
            currentBenchmarkDescriptor->fixtureName,
            currentBenchmarkDescriptor->benchmarkName,
            &result);

#ifdef USE_PAPI
        for(int i = 0; i < result.runs; i++) {
            if(result.singleRuns[i].papiCounters != NULL) {
                free(result.singleRuns[i].papiCounters);
                result.singleRuns[i].papiCounters = NULL;
            }
        }
#endif
        free(result.singleRuns);
        result.singleRuns = NULL;

        currentBenchmarkDescriptor = currentBenchmarkDescriptor->next;
    }
    
    currentOutputter->end(nbBenchmarksRegistered);
}
