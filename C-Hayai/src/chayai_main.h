#ifndef CHAYAI_MAIN_H
#define CHAYAI_MAIN_H

#include "chayai_outputter.h"

#define MAX_NUM_PAPI_EVENTS 32  // way to much, and therefore should always be big enough for now

typedef struct CHayaiArguments {
    void (*outputterInit)(CHayaiOutputter* outputter);
    unsigned int warmupIterations;
    unsigned int benchmarkIterations;
    int papiEventSet; // this variable always exists so benchmarks using it do not depend on the USE_PAPI definition to be present
} CHayaiArguments;

CHayaiArguments chayai_main_parse_args(int argv, char** argc);

void chayai_main_cleanup_arguments(CHayaiArguments *arguments);

#endif // CHAYAI_MAIN_H
