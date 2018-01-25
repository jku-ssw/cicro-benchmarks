#include "harness.h"

#include <bson.h>
#include <bcon.h>

int __attribute__ ((noinline)) test_harness(void) {
    // From: examples/bcon-speed.c
    int bcon;
    bson_t bson, foo, bar, baz;
    bson_init (&bson);

    for(int i = 0; i < 200000; i ++) {
        BCON_APPEND(&bson,
                    "foo",
                    "{",
                    "bar",
                    "{",
                    "baz",
                    "[",
                    BCON_INT32(1),
                    BCON_INT32(2),
                    BCON_INT32(3),
                    "]",
                    "}",
                    "}");
        bson_reinit(&bson);

        bson_append_document_begin(&bson, "foo", -1, &foo);
        bson_append_document_begin(&foo, "bar", -1, &bar);
        bson_append_array_begin(&bar, "baz", -1, &baz);
        bson_append_int32(&baz, "0", -1, 1);
        bson_append_int32(&baz, "1", -1, 2);
        bson_append_int32(&baz, "2", -1, 3);
        bson_append_array_end(&bar, &baz);
        bson_append_document_end(&foo, &bar);
        bson_append_document_end(&bson, &foo);

        bson_reinit(&bson);
    }

    bson_destroy (&bson);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="mongodb-libbson",
        .description="building, parsing, and iterating BSON documents",
        .test_harness=*test_harness,
        .expected_runtime=300L
    };
    return _execute_harness(argc, argv, harness);
}