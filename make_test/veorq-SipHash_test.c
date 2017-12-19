#include <string.h>
#include <stdlib.h>

#include "harness.h"

#include "veorq-SipHash/siphash.c"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

volatile uint8_t *str;

int __attribute__ ((noinline)) test_harness(void) {
    volatile uint8_t out[16], k[16] = {0};
    for (int i = 0; i < sizeof(LOREM_IPSUM1000); i += sizeof(LOREM_IPSUM1000)/256) {
        k[0] = i;
        siphash((uint8_t *) LOREM_IPSUM1000, i, (const uint8_t*)k, (uint8_t*)out, sizeof(out));
    }
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="veorq-SipHash",
        .description="High-speed pseudorandom function",
        .test_harness=*test_harness,
        .expected_runtime=380L
    };
    return _execute_harness(argc, argv, harness);
}
