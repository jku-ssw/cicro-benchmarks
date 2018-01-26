#include "harness.h"

#include "richgel999-miniz/miniz.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

int __attribute__ ((noinline)) test_harness(void) {
    size_t cmp_len = 0;

    const char *p = LOREM_IPSUM1000;
    size_t uncomp_len = strlen(p);

    for(int i = 0; i < 10; i++) {
        void *pComp_data = tdefl_compress_mem_to_heap(p, uncomp_len, &cmp_len, TDEFL_WRITE_ZLIB_HEADER);
        if (!pComp_data) {
            free(pComp_data);
            return EXIT_FAILURE;
        }

        size_t decomp_len = 0;
        void *pDecomp_data = tinfl_decompress_mem_to_heap(pComp_data, cmp_len, &decomp_len,
                                                          TINFL_FLAG_PARSE_ZLIB_HEADER);

        free(pComp_data);
        free(pDecomp_data);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="richgel999-miniz",
        .description="Single C source file zlib-replacement",
        .test_harness=*test_harness,
        .expected_runtime=450L
    };
    return _execute_harness(argc, argv, harness);
}