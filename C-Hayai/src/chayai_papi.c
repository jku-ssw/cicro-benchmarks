#include "chayai_benchmark_result.h"
#include "chayai_papi.h"

#ifdef USE_PAPI

#include <papi.h>

void chayai_papi_start(struct CHayaiBenchmarkSingleRunResult *result) {
    if(result->papiEventSet != PAPI_NULL) {
        PAPI_start(result->papiEventSet);
    }
}

void chayai_papi_read_counters(struct CHayaiBenchmarkSingleRunResult *result) {
    if(result->papiEventSet != PAPI_NULL) {
        PAPI_reset(result->papiEventSet);
    }
}

void chayai_papi_stop_counters(struct CHayaiBenchmarkSingleRunResult *result) {
    if(result->papiEventSet != PAPI_NULL) {
        PAPI_stop(result->papiEventSet, result->papiCounters);
    }
}

#else

void chayai_papi_start(struct CHayaiBenchmarkSingleRunResult *result) {
}

void chayai_papi_read_counters(struct CHayaiBenchmarkSingleRunResult *result) {
}

void chayai_papi_stop_counters(struct CHayaiBenchmarkSingleRunResult *result) {
}

#endif
