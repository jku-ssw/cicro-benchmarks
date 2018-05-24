/*
  libdeep - a library for deep learning
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

#ifndef DEEPLEARN_CONV_H
#define DEEPLEARN_CONV_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include "globals.h"
#include "deeplearn_random.h"
#include "backprop_neuron.h"
#include "encoding.h"
#include "backprop.h"
#include "autocoder.h"
#include "deeplearn_features.h"
#include "deeplearn_history.h"

#define PREPROCESS_MAX_LAYERS 100
#define POOLING_FACTOR        2

/* minimum deconvolved value for gap filling */
#define MIN_DECONVOLVED       0.01f

typedef struct {
    int width, height, depth;
    float * layer;
    int no_of_features, feature_width;
    float * feature;
    unsigned int ctr;
    int pooling_factor;
} deeplearn_conv_layer;

typedef struct {
    int no_of_layers;

    /* array storing layers */
    deeplearn_conv_layer layer[PREPROCESS_MAX_LAYERS];

    /* the amount of noise to add to inputs during training
       in the range 0.0 -> 1.0 */
    float noise;
    unsigned int random_seed;

    /* the outputs at the end of the process */
    int outputs_width;
    int no_of_outputs;
    float * outputs;

    float learning_rate;

    /* current layer for which features are being learned */
    int current_layer;

    /* a training flag which can be toggled to enable
       adding noise to the input layer */
    unsigned char training;

    deeplearn_history history;
} deeplearn_conv;

int conv_init(int no_of_layers,
              int image_width, int image_height, int image_depth,
              int no_of_features, int feature_width,
              int final_image_width, int final_image_height,
              deeplearn_conv * conv);

void conv_feed_forward(unsigned char * img, deeplearn_conv * conv, int layer);

float conv_learn(unsigned char * img,
                 deeplearn_conv * conv,
                 int samples, int layer_itterations,
                 unsigned int * random_seed);

void conv_free(deeplearn_conv * conv);

int conv_plot_history(deeplearn_conv * conv,
                      int img_width, int img_height);
int conv_save(FILE * fp, deeplearn_conv * conv);
int conv_load(FILE * fp, deeplearn_conv * conv);

void convolve_image(float img[],
                    int img_width, int img_height, int img_depth,
                    int feature_width, int no_of_features,
                    int pooling_factor,
                    float feature[],
                    float layer[], int layer_width);
void deconvolve_image(float img[],
                      int img_width, int img_height, int img_depth,
                      int feature_width, int no_of_features,
                      float feature[],
                      float layer[], int layer_width);

int conv_draw_features(unsigned char img[],
                       int img_width, int img_height, int img_depth,
                       int layer,
                       deeplearn_conv * conv);
void convolve_image_mono(float img[],
                         int img_width, int img_height,
                         int feature_width, int no_of_features,
                         int pooling_factor,
                         float feature[],
                         float layer[], int layer_width);
float conv_get_output(deeplearn_conv * conv, int index);
float conv_get_error(deeplearn_conv * conv);
int bp_inputs_from_convnet(bp * net, deeplearn_conv * conv);
void conv_clear(deeplearn_conv * conv);
void conv_feed_backwards(unsigned char img[], deeplearn_conv * conv, int layer);

#endif
