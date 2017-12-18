#include <string.h>
#include <stdlib.h>

#include "harness.h"

#include "josephg-librope/rope.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

volatile uint8_t *str;

int __attribute__ ((noinline)) test_harness(void) {
    rope *r = rope_new();

    rope_insert(r, 0, (uint8_t*)LOREM_IPSUM1000);

    for(int i = 0; i < 50000; i++) {
        rope_insert(r, 100+139*i+(997*i)%569, (uint8_t*)LOREM_IPSUM); // interleaved insertion of substrings
        rope_del(r, 100+139*i+(997*i)%569+20, (i*139)%137+1);
    }

    str = rope_create_cstr(r);
    str[4] = 'A';
    free((void*)str);

    rope_free(r);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="josephg-librope",
        .description="rope library for C",
        .test_harness=*test_harness,
        .expected_runtime=500L
    };
    return _execute_harness(argc, argv, harness);
}
