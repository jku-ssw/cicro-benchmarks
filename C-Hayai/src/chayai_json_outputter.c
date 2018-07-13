#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "chayai_json_outputter.h"

#ifdef USE_PAPI
#include <papi.h>
#endif

#define OUTPUT_STREAM stdout

static int numOfBenchmarks = 0; // required for correct json output
static int curBenchmarkNum = 0;

static void chayai_json_outputter_begin(unsigned int bechmarksCount);
static void chayai_json_outputter_end(unsigned int bechmarksCount);
static void chayai_json_outputter_begin_benchmark(
    const char* fixtureName,
    const char* benchmarkName,
    unsigned int warmupRunsCount,
    unsigned int runsCount,
    unsigned int iterationsCount);
static void chayai_json_outputter_end_benchmark(
    const char* fixtureName,
    const char* benchmarkName,
    const CHayaiBenchmarkResult* result);

void chayai_json_outputter_init(CHayaiOutputter* outputter)
{
    assert(outputter != NULL);
    outputter->begin = &chayai_json_outputter_begin;
    outputter->end = &chayai_json_outputter_end;
    outputter->beginBenchmark = &chayai_json_outputter_begin_benchmark;
    outputter->endBenchmark = &chayai_json_outputter_end_benchmark;
}


static void chayai_json_outputter_begin(unsigned int bechmarksCount)
{
    fputs("{", OUTPUT_STREAM);
    fputs("\"format_version\":1,", OUTPUT_STREAM);
    fputs("\"benchmarks\":[", OUTPUT_STREAM);

    numOfBenchmarks = bechmarksCount;
    curBenchmarkNum = 0;
}



static void chayai_json_outputter_end(unsigned int bechmarksCount)
{
    fputs("]", OUTPUT_STREAM);  // end benchmarks
    fputs("}", OUTPUT_STREAM); // end outer dict

    fflush(OUTPUT_STREAM);
}


static void chayai_json_outputter_begin_benchmark(
    const char* fixtureName,
    const char* benchmarkName,
    unsigned int warmupRunsCount,
    unsigned int runsCount,
    unsigned int iterationsCount)
{
}


static void chayai_json_outputter_end_benchmark(
    const char* fixtureName,
    const char* benchmarkName,
    const CHayaiBenchmarkResult* result)
{
    curBenchmarkNum ++;

    // start benchmark dict
    fputs("{", OUTPUT_STREAM);

    // basic information
    fprintf(OUTPUT_STREAM, "\"fixture\":\"%s\",", fixtureName);  // TODO: escaping
    fprintf(OUTPUT_STREAM, "\"name\":\"%s\",",benchmarkName);  // TODO: escaping, remove args
    fprintf(OUTPUT_STREAM, "\"iterations_per_run\":%d,",result->iterations);
    fprintf(OUTPUT_STREAM, "\"disabled\":false,"); // TODO: not supported at the moment

    // single runs
    fputs("\"runs\":[", OUTPUT_STREAM);
    for(unsigned int i=0; i < result->runs; i++) {
        const double runTimeSingleRunUs = result->singleRuns[i].time / 1e3;
        fprintf(OUTPUT_STREAM, "{\"duration\":%f", runTimeSingleRunUs);

#ifdef USE_PAPI
        if(result->singleRuns[i].papiEventSet != PAPI_NULL) {
            int numberOfEvents = 128;
            int events[128]; // should be easily big enough
            PAPI_list_events(result->singleRuns[i].papiEventSet, events, &numberOfEvents);

            for(int k = 0; k < numberOfEvents; k++) {
                PAPI_event_info_t info;
                PAPI_get_event_info(events[k], &info);
                fprintf(OUTPUT_STREAM, ", \"%s\":%lld", info.symbol, result->singleRuns[i].papiCounters[k]);
            }
        }
#endif

        fprintf(OUTPUT_STREAM, "}");
        if(i+1 < result->runs) {
            fputs(",", OUTPUT_STREAM);
        }
    }
    fputs("],", OUTPUT_STREAM);

    // statistical data
    const double runTimeAvgUs = result->timeTotal / (1e3 * result->runs);

    double varianceUs = 0;
    for(unsigned int i=0; i < result->runs; i++) {
        const double runTimeSingleRunUs = result->singleRuns[i].time / 1e3;
        varianceUs +=  (runTimeSingleRunUs - runTimeAvgUs) * (runTimeSingleRunUs - runTimeAvgUs);
    }
    varianceUs /= result->runs;
    const double standardDerivationUs = sqrt(varianceUs);

    fprintf(OUTPUT_STREAM, "\"mean\":%f,", runTimeAvgUs);
    fprintf(OUTPUT_STREAM, "\"std_dev\":%f", standardDerivationUs);

    // end benchmark dict
    fputs("}", OUTPUT_STREAM);
    if(curBenchmarkNum < numOfBenchmarks) {
        fputs(",", OUTPUT_STREAM);
    }

    fflush(OUTPUT_STREAM);
}