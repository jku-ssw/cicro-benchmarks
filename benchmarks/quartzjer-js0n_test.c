#include <stdlib.h>
#include <string.h>

#include "chayai.h"

#include "quartzjer-js0n/src/js0n.h"

#define JSON ""\
"{                                          "\
"    \"barbar\": [1, 2, 3, 4, 5, 6, 7],     "\
"    \"bar\": {                             "\
"        \"barbar\": [1, 2, 3, 4, 5, 6, 7], "\
"        \"foo\": \"bar\",                  "\
"        \"obj\": {                         "\
"            \"a\": \"b\"                   "\
"        }                                  "\
"    },                                     "\
"    \"foo\": \"bar\",                      "\
"    \"obj\": {                             "\
"        \"a\": \"b\"                       "\
"    },                                     "\
"    \"bob\" : {                            "\
"        \"test\": {                        "\
"            \"blabla\": []                 "\
"        }                                  "\
"    }                                      "\
"}                                          "

BENCHMARK(quartzjer, js0n, 100, 100) {
    size_t len;

    for(int i = 0; i < 200; i++) {
        const char *rc = js0n("blabla", 1, JSON, strlen(JSON), &len);
    }
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(quartzjer, js0n); // Zero-Footprint JSON Parser

    RUN_BENCHMARKS(argc, argv);

    return 0;
}