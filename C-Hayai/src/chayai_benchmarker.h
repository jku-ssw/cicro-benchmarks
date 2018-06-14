#ifndef CHAYAI_BENCHMARKER_H
#define CHAYAI_BENCHMARKER_H

#include "chayai_benchmark_descriptor.h"
#include "chayai_benchmark_pp.h"
#include "chayai_outputter.h"
#include "chayai_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Register benchmark, called by macros
void chayai_register_benchmark(CHayaiBenchmarkDescriptor* descriptor);

/// Set outputter
void chayai_set_outputter(CHayaiOutputter* outputter);

/// Set number of warmup iterations
void chayai_set_warmup_iterations(unsigned int warmup);

/// Run all registered benchmark
void chayai_run_benchmarks(CHayaiArguments* arguments);

#ifdef __cplusplus
}
#endif

#endif // CHAYAI_BENCHMARKER_H
