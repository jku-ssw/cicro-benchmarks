#ifndef CHAYAI_JSON_OUTPUTTER_H
#define CHAYAI_JSON_OUTPUTTER_H

#include "chayai_outputter.h"

#ifdef __cplusplus
extern "C" {
#endif


/// Json outputter.
/// Prints the result to standard output using JSON format.
void chayai_json_outputter_init(CHayaiOutputter* outputter);

#ifdef __cplusplus
}
#endif


#endif // CHAYAI_JSON_OUTPUTTER_H
