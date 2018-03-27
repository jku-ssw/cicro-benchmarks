#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "chayai.h"

#define STB_IMAGE_IMPLEMENTATION
#include "kylophone-spectrophoto/stb_image.h"

#define SIZE_X 2048
#define SIZE_Y 2048

BENCHMARK(kylophone, spectrophoto, 100, 1) {
    int x, y, n;
    unsigned char *img = stbi_load("kylophone-spectrophoto/face.png", &x, &y, &n, 1);

    stbi_image_free(img);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(kylophone, spectrophoto); // Turns an image into an audio file

    RUN_BENCHMARKS(argc, argv);

    return 0;
}