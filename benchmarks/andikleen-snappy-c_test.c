#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "chayai.h"

#include "andikleen-snappy-c/snappy.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

volatile int source_len = sizeof(LOREM_IPSUM1000);
volatile char *source = LOREM_IPSUM1000;

BENCHMARK(snappy_c, compress_uncompress, 100, 100) {
    struct snappy_env env;
    int res = snappy_init_env(&env);
    assert(res == 0);

    size_t compressed_max_len = snappy_max_compressed_length(source_len);
    assert(compressed_max_len == 1034866);
    char *compressed = malloc(sizeof(char) * compressed_max_len);

    size_t compressed_len;
    snappy_compress(&env, (const char *)source, source_len, compressed, &compressed_len);
    assert(compressed_len == 45148);
    assert(compressed[0] == -39 && compressed[256] == 116 && compressed[512] == 3);

    size_t uncompressed_len;
    snappy_uncompressed_length(compressed, compressed_len, &uncompressed_len);
    assert(uncompressed_len == 887001);
    char *uncompressed = malloc(sizeof(char) * uncompressed_len);

    snappy_uncompress(compressed, compressed_max_len, uncompressed);

    free(uncompressed);
    free(compressed);

    snappy_free_env(&env);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(snappy_c, compress_uncompress); // C port of the snappy compressor

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
