/*
 Face recognition demo
 Copyright (C) 2013-2017  Bob Mottram <bob@freedombone.net>

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
#include "libdeep/deeplearn_images.h"
#include "libdeep/deepconvnet.h"

#define TITLE "Face Recognition"

/* the dimensions of each face image */
int image_width = 32;
int image_height = 32;

deepconvnet convnet;

deeplearn_conv convolution;
deeplearn learner;

/**
* @brief Train the deep learner
*/
static void facerec_training()
{
    int no_of_convolutions = 3;
    int no_of_deep_layers = 2;
    int max_features_per_convolution = 5*5;
    int final_image_width = 3;
    int no_of_outputs = 5*5;
    int output_classes = 25;
    int feature_width = 5;
    float error_threshold[] = { 5.0, 5.0, 5.0 };
    unsigned int ctr, random_seed = 34217;
    float performance;
    unsigned int layer_itterations = 5000;
    int extra_synthetic_images = 5;

    if (deepconvnet_read_images("images",
                                &convnet,
                                image_width, image_height,
                                no_of_convolutions,
                                max_features_per_convolution,
                                feature_width,
                                final_image_width, final_image_width,
                                layer_itterations,
                                no_of_deep_layers,
                                no_of_outputs,
                                output_classes,
                                error_threshold,
                                &random_seed,
                                extra_synthetic_images) != 0) {
        return;
    }

    printf("Number of images: %d\n", convnet.no_of_images);
    printf("Number of labeled training examples: %d\n",
           convnet.no_of_images*8/10);
    printf("Number of test examples: %d\n",convnet.no_of_images*2/10);

    if (convnet.no_of_images == 0)
        return;

    deepconvnet_set_learning_rate(&convnet, 0.2f);

    deepconvnet_set_dropouts(&convnet, 0.0f);

    convnet.history.interval = 800;

    deeplearn_set_title(&learner, TITLE);

    ctr = 99999;
    while (deepconvnet_training(&convnet) == 0) {
        if (convnet.current_layer == 0) {
            if (ctr > 10000) {
                deepconvnet_plot_features(&convnet, 0,
                                          "features0.png",
                                          640, 640);
                ctr = 0;
            }
            ctr++;
        }
    }

    deepconvnet_plot_features(&convnet, 0,
                              "features0.png",
                              640, 640);

    if (deepconvnet_training(&convnet) < 0) {
        printf("Training error\n");
        return;
    }

    printf("Training Completed\n");
    performance = deepconvnet_get_performance(&convnet);
    if (performance >= 0) {
        printf("Test data set performance is %.2f%%\n",
               performance);
    }
    else {
        printf("Performance measurement error %d\n",
               (int)performance);
    }

    deepconvnet_free(&convnet);
}

/**
* @brief Main function
*/
int main(int argc, char* argv[])
{
    facerec_training();
    return 0;
}
