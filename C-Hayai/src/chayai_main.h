#ifndef CHAYAI_MAIN_H
#define CHAYAI_MAIN_H

#include "chayai_outputter.h"

typedef struct CHayaiArguments {
    void (*outputterInit)(CHayaiOutputter* outputter);
    unsigned int warmupIterations;
} CHayaiArguments;

CHayaiArguments chayai_main_parse_args(int argv, char** argc);

#endif // CHAYAI_MAIN_H
