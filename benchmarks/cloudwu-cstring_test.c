#include <stdlib.h>
#include <assert.h>

#include "chayai.h"

#include "cloudwu-cstring/cstring.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

BENCHMARK(cloudwu, cstring, 100, 1) {
    CSTRING_BUFFER(a);

    cstring_printf(a, "%s", LOREM_IPSUM1000);

    cstring_printf(a, "very long string %01024d",0);

    CSTRING_BUFFER(b);

    cstring_printf(b, "%s", LOREM_IPSUM1000);
    int ret = cstring_equal(CSTRING(a), CSTRING(b));
    assert(ret == 0);

    uint32_t hash_a = cstring_hash(CSTRING(a));
    assert(hash_a == 0xDB3B5790);

    uint32_t hash_b = cstring_hash(CSTRING(b));
    assert(hash_b == 0xF352F174);

    CSTRING_CLOSE(b);
    CSTRING_CLOSE(a);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(cloudwu, cstring); // A simple C string lib

    RUN_BENCHMARKS(argc, argv);

    return 0;
}