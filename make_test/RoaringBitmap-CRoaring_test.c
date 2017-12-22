#include <stdlib.h>

#include "harness.h"

#include "RoaringBitmap-CRoaring/include/roaring/roaring.h"

#define NUMEL 1000000

int __attribute__ ((noinline)) test_harness(void) {
    uint32_t arr1[NUMEL] = {0};
    uint32_t arr2[NUMEL] = {0};

    for(int i = 0; i < NUMEL; i++) {
        arr1[i] = i*2;
        arr2[i] = i*41;
    }

    roaring_bitmap_t *bitmap1 = roaring_bitmap_of_ptr(NUMEL, arr1);
    roaring_bitmap_t *bitmap2 = roaring_bitmap_of_ptr(NUMEL, arr2);


    for(int i = 0; i < 256; i++) {
        roaring_bitmap_t *bitand1 = roaring_bitmap_and(bitmap1, bitmap2);
        roaring_bitmap_free(bitand1);

        roaring_bitmap_t *bitxor1 = roaring_bitmap_xor(bitmap1, bitmap2);
        roaring_bitmap_free(bitxor1);
    }


    roaring_bitmap_free(bitmap2);
    roaring_bitmap_free(bitmap1);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="RoaringBitmap-CRoaring",
        .description="Roaring bitmaps in C",
        .test_harness=*test_harness,
        .expected_runtime=430L
    };
    return _execute_harness(argc, argv, harness);
}
