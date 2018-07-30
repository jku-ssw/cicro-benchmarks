#include "chayai.h"  // first include!

#include <assert.h>

#include <bson.h>
#include <bcon.h>

BENCHMARK(mongodb, libbson, 100, 10) {
    // From: examples/bcon-speed.c
    int bcon;
    bson_t bson, foo, bar, baz;
    bson_init (&bson);

    for(int i = 0; i < 2000; i ++) {
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

        bson_append_document_begin(&bson, "foo", -1, &foo);
        bson_append_document_begin(&foo, "bar", -1, &bar);
        bson_append_array_begin(&bar, "baz", -1, &baz);
        bson_append_int32(&baz, "0", -1, 1);
        bson_append_int32(&baz, "1", -1, 2);
        bson_append_int32(&baz, "2", -1, 3);
        bson_append_array_end(&bar, &baz);
        bson_append_document_end(&foo, &bar);
        bson_append_document_end(&bson, &foo);
    }

    assert(bson.len == 204005);

    bson_destroy (&bson);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(mongodb, libbson); // building, parsing, and iterating BSON documents

    RUN_BENCHMARKS(argc, argv);

    return 0;
}