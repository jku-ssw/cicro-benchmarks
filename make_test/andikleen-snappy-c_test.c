#include <string.h>
#include <stdlib.h>

#include "harness.h"

#include "andikleen-snappy-c/snappy.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

int __attribute__ ((noinline)) test_harness(void) {
    struct snappy_env env;
    snappy_init_env(&env);
    int source_len = sizeof(LOREM_IPSUM1000);
    char *source = LOREM_IPSUM1000;

    for(int i = 0; i < 512; i++) {
        // change start of text a little bit so it cannot be optimized that easily
        source++;
        source_len--;

        size_t compressed_max_len = snappy_max_compressed_length(source_len);
        char *compressed = malloc(sizeof(char) * compressed_max_len);

        size_t compressed_len;
        snappy_compress(&env, source, source_len, compressed, &compressed_len);

        size_t uncompressed_len;
        snappy_uncompressed_length(compressed, compressed_len, &uncompressed_len);
        char *uncompressed = malloc(sizeof(char) * uncompressed_len);

        snappy_uncompress(compressed, compressed_max_len, uncompressed);

        free(uncompressed);
        free(compressed);
    }

    snappy_free_env(&env);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="andikleen-snappy-c",
        .description="C port of the snappy compressor",
        .test_harness=*test_harness,
        .expected_runtime=500L
    };
    return _execute_harness(argc, argv, harness);
}
