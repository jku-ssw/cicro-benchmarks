/*
  Copyright (C) 2015-2017  Bob Mottram <bob@freedombone.net>

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

#include "tests_conv.h"

static void test_conv_init()
{
    int no_of_layers = 3;
    int image_width = 256;
    int image_height = 256;
    int image_depth = 3;
    int no_of_features = 10*10;
    int feature_width = 8;
    int final_image_width = 64;
    int final_image_height = 64;
    deeplearn_conv conv;

    printf("test_conv_init...");

    assert(conv_init(no_of_layers,
                     image_width, image_height, image_depth,
                     no_of_features, feature_width,
                     final_image_width, final_image_height,
                     &conv) == 0);
    conv_free(&conv);

    printf("Ok\n");
}

static void test_conv_learn()
{
    int no_of_layers = 3;
    unsigned char * img = NULL;
    unsigned int image_width = 0;
    unsigned int image_height = 0;
    int image_depth = 3;
    int no_of_features = 4*4;
    int feature_width = 8;
    int final_image_width = 64;
    int final_image_height = 64;
    deeplearn_conv conv;
    unsigned int bitsperpixel = 0;
    unsigned int random_seed = 123;
    int downsampled_width=128;
    int downsampled_height=128;
    unsigned char downsampled[128*128];
    unsigned int layer_itterations = 5;

    printf("test_conv_learn...");

    assert(deeplearn_read_png_file((char*)"Lenna.png",
                                   &image_width, &image_height,
                                   &bitsperpixel, &img)==0);
    assert(img != NULL);
    assert(image_width == 512);
    assert(image_height == 512);
    assert(bitsperpixel == 24);

    assert(image_resize(img, (int)image_width, (int)image_height,
                        (int)(bitsperpixel/8),
                        &downsampled[0],
                        downsampled_width, downsampled_height, 1)==0);
    image_depth = 1;
    image_width = (unsigned int)downsampled_width;
    image_height = (unsigned int)downsampled_height;

    assert(conv_init(no_of_layers,
                     (int)image_width, (int)image_height, image_depth,
                     no_of_features, feature_width,
                     final_image_width, final_image_height,
                     &conv) == 0);

    float matching_score = 0;
    while (conv.current_layer == 0) {
        matching_score =
            conv_learn(downsampled, &conv, 100, layer_itterations, &random_seed);
        assert(matching_score > 0);
    }
    assert(matching_score < 3.5f);

    conv_clear(&conv);
    conv_feed_forward(downsampled, &conv, 1);

    /* check that the convolution values in the second layer are reasonable */
    float layer_sum = 0;
    COUNTDOWN(i, conv.layer[1].width*conv.layer[1].height*conv.layer[1].depth) {
        assert(conv.layer[1].layer[i] >= 0.0f);
        assert(conv.layer[1].layer[i] <= 1.0f);
        layer_sum += conv.layer[1].layer[i];
    }
    layer_sum /= conv.layer[1].width*conv.layer[1].height*conv.layer[1].depth;
    assert(layer_sum > 0.0f);
    assert(layer_sum <= 1.0f);

    /* check that error continues to decrease on the second layer */
    while (conv.current_layer == 1) {
        matching_score =
            conv_learn(downsampled, &conv, 100, layer_itterations, &random_seed);
        assert(matching_score > 0);
        printf(".");
        fflush(stdout);
    }
    assert(matching_score < 8.0f);

    conv_clear(&conv);
    conv_feed_forward(downsampled, &conv, 2);

    /* check that the convolution values in the second layer are reasonable */
    layer_sum = 0;
    COUNTDOWN(i, conv.layer[2].width*conv.layer[2].height*conv.layer[2].depth) {
        assert(conv.layer[2].layer[i] >= 0.0f);
        assert(conv.layer[2].layer[i] <= 1.0f);
        layer_sum += conv.layer[2].layer[i];
    }
    layer_sum /= conv.layer[2].width*conv.layer[2].height*conv.layer[2].depth;
    assert(layer_sum > 0.0f);
    assert(layer_sum <= 1.0f);

    /* check that error continues to decrease on the second layer */
    while (conv.current_layer == 2) {
        matching_score =
            conv_learn(downsampled, &conv, 100, layer_itterations, &random_seed);
        assert(matching_score > 0);
        printf(".");
        fflush(stdout);
    }
    assert(matching_score < 7.0f);

    /* clear outputs */
    FLOATCLEAR(&conv.outputs[0], conv.no_of_outputs);

    /* check that the outputs are all zero */
    float outputs_sum = 0;
    COUNTDOWN(i, conv.no_of_outputs) {
        assert(conv.outputs[i] == 0.0f);
    }

    /* feed forward through all layers */
    conv_feed_forward(&downsampled[0], &conv, no_of_layers);

    /* check that there are some non-zero outputs */
    outputs_sum = 0;
    COUNTDOWN(i, conv.no_of_outputs) {
        assert(conv.outputs[i] >= 0.0f);
        assert(conv.outputs[i] <= 1.0f);
        outputs_sum += conv.outputs[i];
    }
    outputs_sum /= conv.no_of_outputs;
    assert(outputs_sum > 0.01f);
    assert(outputs_sum <= 1.0f);

    /* save a graph */
    sprintf(&conv.history.filename[0],"%stemp_graph.png", DEEPLEARN_TEMP_DIRECTORY);
    sprintf(&conv.history.title[0], "%s", "Feature Learning");
    conv_plot_history(&conv, 1024, 480);

    free(img);
    conv_free(&conv);

    printf("Ok\n");
}

static void test_reconstruction_from_features()
{
    unsigned char * img, * img_reconstructed;
    unsigned int img_width = 0;
    unsigned int img_height = 0;
    int img_depth = 3;
    unsigned int bitsperpixel = 0;

    deeplearn_conv convnet;
    int i;
    int no_of_layers = 3;
    int final_image_width, final_image_height;
    int no_of_features = 5*5;
    int feature_width = 6;
    unsigned int random_seed = 123;
    int layer_itterations = 30;
    int percent_similarity, diff = 0;

    printf("test_reconstruction_from_features...");

    assert(deeplearn_read_png_file((char*)"Lenna.png",
                                   &img_width, &img_height,
                                   &bitsperpixel, &img) == 0);

    img_depth = (int)bitsperpixel/8;
    final_image_width = (int)img_width/16;
    final_image_height = (int)img_height/16;

    img_reconstructed =
        (unsigned char *)malloc(img_width*img_height*img_depth*
                                sizeof(unsigned char));
    if (!img_reconstructed)
        return;

    assert(conv_init(no_of_layers,
                     (int)img_width, (int)img_height, img_depth,
                     no_of_features, feature_width,
                     final_image_width, final_image_height,
                     &convnet) == 0);

    for (i = 0; i < layer_itterations; i++) {
        conv_learn(img, &convnet, 500, layer_itterations, &random_seed);
        printf(".");
        fflush(stdout);
    }

    conv_feed_forward(img, &convnet, 1);
    printf(".");
    fflush(stdout);

    memset((void*)img_reconstructed, '\0',
           (int)img_width*(int)img_height*img_depth*sizeof(unsigned char));
    conv_feed_backwards(img_reconstructed, &convnet, 0);
    printf(".");
    fflush(stdout);

    /* difference between source image and reconstructed image */
    for (i = 0; i < (int)img_width*(int)img_height*img_depth; i++)
        diff += abs(img[i] - img_reconstructed[i]);

    percent_similarity =
        100 - ((diff / ((int)img_width*(int)img_height*img_depth))*100/255);

    if (percent_similarity < 65)
        printf("\nPercent similarity: %d%%\n", percent_similarity);

    assert(percent_similarity >= 65);

    conv_free(&convnet);
    free(img);
    free(img_reconstructed);

    printf("Ok\n");
}

int run_tests_conv()
{
    printf("\nRunning convolution tests\n");

    test_conv_init();
    test_conv_learn();
    test_reconstruction_from_features();

    printf("All convolution tests completed\n");
    return 0;
}
