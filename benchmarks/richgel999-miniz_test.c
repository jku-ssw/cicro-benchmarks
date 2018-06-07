#include "chayai.h"

#include "richgel999-miniz/miniz.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

BENCHMARK(richgel999, miniz, 100, 1) {
    size_t cmp_len = 0;

    const char *p = LOREM_IPSUM1000;
    size_t uncomp_len = strlen(p);

    void *pComp_data = tdefl_compress_mem_to_heap(p, uncomp_len, &cmp_len, TDEFL_WRITE_ZLIB_HEADER);
    if (!pComp_data) {
        free(pComp_data);
        abort();
    }

    size_t decomp_len = 0;
    void *pDecomp_data = tinfl_decompress_mem_to_heap(pComp_data, cmp_len, &decomp_len,
                                                      TINFL_FLAG_PARSE_ZLIB_HEADER);

    free(pComp_data);
    free(pDecomp_data);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(richgel999, miniz); // Single C source file zlib-replacement

    RUN_BENCHMARKS(argc, argv);

    return 0;
}