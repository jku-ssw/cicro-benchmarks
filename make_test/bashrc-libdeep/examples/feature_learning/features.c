/*
 Feature learning from images demo
 Copyright (C) 2017  Bob Mottram <bob@freedombone.net>

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. Neither the name of the University nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.
 .
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE HOLDERS OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "libdeep/globals.h"
#include "libdeep/deeplearn.h"

static void learn_features_from_image()
{
    unsigned char * img, * img2, * img_features;
    unsigned int img_width = 0;
    unsigned int img_height = 0;
    int img_depth = 3;
    unsigned int bitsperpixel = 0;

    deeplearn_conv convnet;
    int i, layer;
    int no_of_layers = 3;
    int final_image_width, final_image_height;
    int no_of_features = 8*8;
    int feature_width = 8;
    unsigned int random_seed = 123;
    int layer_itterations = 5;

    /* load image from file */
    assert(deeplearn_read_png_file((char*)"../../unittests/Lenna.png",
                                   &img_width, &img_height,
                                   &bitsperpixel, &img2)==0);
    assert(deeplearn_read_png_file((char*)"../../unittests/monalisa.png",
                                   &img_width, &img_height,
                                   &bitsperpixel, &img)==0);

    printf("Image loaded\n");

    img_depth = (int)bitsperpixel/8;
    final_image_width = (int)img_width/16;
    final_image_height = (int)img_height/16;

    img_features =
        (unsigned char *)malloc(img_width*img_height*img_depth*
                                sizeof(unsigned char));
    if (!img_features)
        return;

    assert(conv_init(no_of_layers,
                     (int)img_width, (int)img_height, img_depth,
                     no_of_features, feature_width,
                     final_image_width, final_image_height,
                     &convnet) == 0);

    for (layer = 0; layer < 1; layer++) {
        convnet.current_layer = layer;
        for (i = 0; i < layer_itterations; i++) {
            conv_learn(img, &convnet, 500, layer_itterations, &random_seed);
            printf(".");
            fflush(stdout);
            if ((i > 0) && (i % layer_itterations == 0))
                printf("\n");
        }
    }
    printf("\n");

    printf("Plotting features\n");

    conv_draw_features(img_features, (int)img_width, (int)img_height,
                       img_depth, 0, &convnet);

    deeplearn_write_png_file("features.png",
                             img_width, img_height,
                             bitsperpixel, img_features);

    printf("Plotting Reconstruction 1\n");

    conv_feed_forward(img, &convnet, 1);
    conv_feed_backwards(img, &convnet, 0);

    deeplearn_write_png_file("reconstruction1.png",
                             img_width, img_height,
                             bitsperpixel, img);

    printf("Plotting Reconstruction 2\n");

    conv_feed_forward(img2, &convnet, 1);
    conv_feed_backwards(img2, &convnet, 0);

    deeplearn_write_png_file("reconstruction2.png",
                             img_width, img_height,
                             bitsperpixel, img2);

    conv_free(&convnet);
    free(img);
    free(img2);
    free(img_features);
}

int main(int argc, char* argv[])
{
    learn_features_from_image();
    return 0;
}
