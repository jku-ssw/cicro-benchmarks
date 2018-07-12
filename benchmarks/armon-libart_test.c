#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "chayai.h"

#include "armon-libart/src/art.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10

volatile char* text = LOREM_IPSUM100;
volatile int max_len = sizeof(LOREM_IPSUM100);

BENCHMARK(libart, tree, 100, 10) {
    art_tree t;
    art_tree_init(&t);

    for(int i = 0; i < max_len; i+=100) {
        unsigned char *key = (unsigned char *) text+i;
        art_insert(&t, key, max_len - i, NULL);
    }

    assert(t.size == 888);

    art_tree_destroy(&t);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(libart, tree); // Adaptive Radix Trees

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
