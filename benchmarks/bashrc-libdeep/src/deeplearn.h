/*
 libdeep - a library for deep learning
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

#ifndef DEEPLEARN_H
#define DEEPLEARN_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "globals.h"
#include "backprop.h"
#include "autocoder.h"
#include "encoding.h"
#include "utils.h"
#include "deeplearn_history.h"
#include "deeplearn_conv.h"

/* Enumerate different flavors of C which can be exported
   as a standalone program */
enum {
    EXPORT_C99 = 0,
    EXPORT_ARDUINO
};

/* types of weight gradient to plot */
enum {
    GRADIENT_STANDARD_DEVIATION = 0,
    GRADIENT_MEAN
};

struct deeplearndata {
    float * inputs;
    char ** inputs_text;
    float * outputs;
    unsigned int flags;
    unsigned int labeled;
    struct deeplearndata * prev;
    struct deeplearndata * next;
};
typedef struct deeplearndata deeplearndata;

struct deeplearndata_meta {
    deeplearndata * sample;
    struct deeplearndata_meta * prev;
    struct deeplearndata_meta * next;
};
typedef struct deeplearndata_meta deeplearndata_meta;

struct deepl {
    bp * net;
    ac ** autocoder;
    int current_hidden_layer;
    float backprop_error;
    float * error_threshold;
    int training_complete;
    int no_of_input_fields;
    int * field_length;

    deeplearndata * data;
    int data_samples;
    deeplearndata ** indexed_data;
    int indexed_data_samples;

    deeplearndata_meta * training_data;
    int training_data_samples;
    deeplearndata_meta ** indexed_training_data;
    int indexed_training_data_samples;

    deeplearndata_meta * training_data_labeled;
    int training_data_labeled_samples;
    deeplearndata_meta ** indexed_training_data_labeled;
    int indexed_training_data_labeled_samples;

    deeplearndata_meta * test_data;
    int test_data_samples;
    deeplearndata_meta ** indexed_test_data;
    int indexed_test_data_samples;

    float * input_range_min;
    float * input_range_max;
    float * output_range_min;
    float * output_range_max;

    unsigned int training_ctr;

    deeplearn_history history;
    deeplearn_history gradients_std;
    deeplearn_history gradients_mean;
};
typedef struct deepl deeplearn;

int deeplearn_init(deeplearn * learner,
                   int no_of_inputs,
                   int no_of_hiddens,
                   int hidden_layers,
                   int no_of_outputs,
                   float error_threshold[],
                   unsigned int * random_seed);
void deeplearn_feed_forward(deeplearn * learner);
void deeplearn_update(deeplearn * learner);
void deeplearn_free(deeplearn * learner);
void deeplearn_set_input_text(deeplearn * learner, char * text);
void deeplearn_set_input(deeplearn * learner, int index, float value);
int deeplearn_set_input_field(deeplearn * learner, int fieldindex,
                              float value);
int deeplearn_set_input_field_text(deeplearn * learner, int fieldindex,
                                   char * text);
void deeplearn_set_inputs(deeplearn * learner, deeplearndata * sample);
void deeplearn_set_output(deeplearn * learner, int index, float value);
void deeplearn_set_outputs(deeplearn * learner, deeplearndata * sample);
void deeplearn_get_outputs(deeplearn * learner, float outputs[]);
float deeplearn_get_output(deeplearn * learner, int index);
float deeplearn_get_desired(deeplearn * learner, int index);
int deeplearn_get_class(deeplearn * learner);
void deeplearn_set_class(deeplearn * learner, int class);
int deeplearn_save(FILE * fp, deeplearn * learner);
int deeplearn_load(FILE * fp, deeplearn * learner);
int deeplearn_compare(deeplearn * learner1,
                      deeplearn * learner2);
int deeplearn_plot_history(deeplearn * learner,
                           int image_width, int image_height);
int deeplearn_plot_gradients(int gradient_type, deeplearn * learner,
                             int image_width, int image_height);
int deeplearn_inputs_from_image_patch(deeplearn * learner,
                                      unsigned char * img,
                                      int image_width, int image_height,
                                      int tx, int ty);
void deeplearn_inputs_from_image(deeplearn * learner,
                                 unsigned char * img,
                                 int image_width, int image_height);
void deeplearn_set_learning_rate(deeplearn * learner, float rate);
void deeplearn_set_dropouts(deeplearn * learner, float dropout_percent);
int deeplearn_export(deeplearn * learner, char * filename);
float deeplearn_get_error_threshold(deeplearn * learner, int index);
void deeplearn_set_error_threshold(deeplearn * learner, int index,
                                   float value);
void deeplearn_update_continuous(deeplearn * learner);
int deeplearn_training_last_layer(deeplearn * learner);
void copy_autocoder_to_hidden_layer(deeplearn * learner, int hidden_layer);
void deeplearn_pretrain(bp * net, ac * autocoder, int current_layer);
int deeplearn_inputs_from_convnet(deeplearn * learner, deeplearn_conv * conv);
void deeplearn_set_title(deeplearn * learner, char title[]);
int deeplearn_plot_weight_magnitude(deeplearn * learner,
                                    int buckets,
                                    float max_magnitude,
                                    int img_width, int img_height);
int deeplearn_prune_weights(deeplearn * learner, float threshold);
void deeplearn_set_pruning(deeplearn * learner, unsigned int cycle, float rate);

#endif
