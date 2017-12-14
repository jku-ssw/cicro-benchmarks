#include <string.h>

#include "harness.h"

#include "zhemao-libds/vector.h"

int __attribute__ ((noinline)) test_harness(void) {
    for(int i =0 ; i < 4096; i++) {
        vector_p vec = create_vector();
        vector_p subvec;
        int x;
        char *str = "hello, world";
        char *str2 = "goodbye, world";

        for (x = 0; x < 1024; x++) {
            vector_add(vec, (void *) str, strlen(str) + 1);
        }

        vector_remove(vec, 4);

        subvec = subvector(vec, 1, 1000);

        destroy_vector(subvec);
        destroy_vector(vec);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="zhemao-libds",
        .description="Simple, memory-safe data-structures in C",
        .test_harness=*test_harness,
        .expected_runtime=400L
    };
    return _execute_harness(argc, argv, harness);
}
