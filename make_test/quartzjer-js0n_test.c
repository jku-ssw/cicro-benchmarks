#include <stdlib.h>

#include "harness.h"

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

int __attribute__ ((noinline)) test_harness(void) {
    size_t len;

    for(int i = 0; i < 200000; i++) {
        int rc = js0n("blabla", 1, JSON, strlen(JSON), &len);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="quartzjer-js0n",
        .description="Zero-Footprint JSON Parser",
        .test_harness=*test_harness,
        .expected_runtime=470L
    };
    return _execute_harness(argc, argv, harness);
}
