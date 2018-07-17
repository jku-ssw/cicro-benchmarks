#ifndef CHAYAI_PAPI_H
#define CHAYAI_PAPI_H

struct CHayaiBenchmarkSingleRunResult;

void chayai_papi_start(struct CHayaiBenchmarkSingleRunResult *result);

void chayai_papi_read_counters(struct CHayaiBenchmarkSingleRunResult *result);

void chayai_papi_stop_counters(struct CHayaiBenchmarkSingleRunResult *result);

#endif // CHAYAI_PAPI_H
