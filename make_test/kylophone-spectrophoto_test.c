#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "harness.h"

#define STB_IMAGE_IMPLEMENTATION
#include "kylophone-spectrophoto/stb_image.h"

#define SIZE_X 2048
#define SIZE_Y 2048

int __attribute__ ((noinline)) test_harness(void) {

    for(int i = 0; i < 10; i++) {
        int x, y, n;
        unsigned char *img = stbi_load("kylophone-spectrophoto/face.png", &x, &y, &n, 1);

        stbi_image_free(img);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="kylophone-spectrophoto",
        .description="Turns an image into an audio file",
        .test_harness=*test_harness,
        .expected_runtime=400L
    };
    return _execute_harness(argc, argv, harness);
}
