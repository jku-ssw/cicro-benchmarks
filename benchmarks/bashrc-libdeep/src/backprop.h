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

#ifndef DEEPLEARN_BACKPROP_H
#define DEEPLEARN_BACKPROP_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include "globals.h"
#include "deeplearn_random.h"
#include "deeplearn_images.h"
#include "backprop_neuron.h"
#include "encoding.h"

/* macro returns the number of hidden units at a given layer index */
#define HIDDENS_IN_LAYER(net, layer)                                    \
    ((layer) == 0 ? (net)->no_of_hiddens :                                \
     ((layer) >= (net)->hidden_layers ? ((net)->hidden_layers-1) :        \
      ((net)->no_of_hiddens -                                             \
       (((net)->no_of_hiddens - (net)->no_of_outputs)*(layer)/(net)->hidden_layers))))

struct backprop {
    int no_of_inputs,no_of_hiddens,no_of_outputs;
    int hidden_layers;
    float dropout_percent;
    bp_neuron ** inputs;
    bp_neuron *** hiddens;
    bp_neuron ** outputs;
    float backprop_error_total;
    float backprop_error, backprop_error_average;
    float backprop_error_percent;
    float learning_rate;
    float noise;
    unsigned int random_seed;
    unsigned int itterations;
    unsigned int pruning_cycle;
    float pruning_rate;
};
typedef struct backprop bp;

int bp_init(bp * net,
            int no_of_inputs,
            int no_of_hiddens,
            int hidden_layers,
            int no_of_outputs,
            unsigned int * random_seed);
void bp_free(bp * net);
void bp_feed_forward(bp * net, int learning);
void bp_feed_forward_layers(bp * net, int layers);
void bp_backprop(bp * net, int current_hidden_layer);
void bp_learn(bp * net, int current_hidden_layer);
void bp_set_input_text(bp * net, char * text);
void bp_set_input(bp * net, int index, float value);
void bp_set_output(bp * net, int index, float value);
float bp_get_hidden(bp * net, int layer, int index);
float bp_get_output(bp * net, int index);
float bp_get_desired(bp * net, int index);
void bp_update(bp * net, int current_hidden_layer);
int bp_save(FILE * fp, bp * net);
int bp_load(FILE * fp, bp * net);
int bp_compare(bp * net1, bp * net2);
int bp_inputs_from_image_patch(bp * net,
                               unsigned char img[],
                               int image_width, int image_height,
                               int tx, int ty);
int bp_inputs_from_image(bp * net,
                         unsigned char img[],
                         int image_width, int image_height);
int bp_plot_weights(bp * net,
                    char * filename,
                    int image_width, int image_height,
                    int input_image_width);
void bp_get_classification_from_filename(char * filename,
                                         char * classification);
int bp_classifications_to_numbers(int no_of_instances,
                                  char ** instance_classification,
                                  int * numbers);
void bp_reproject(bp * net, int layer, int neuron_index);
void bp_normalise_inputs(bp * net);
float bp_get_input(bp * net, int index);
float bp_weight_gradient_mean(bp * net, int layer_index);
float bp_weight_gradient_std(bp * net, int layer_index);
void bp_update_averages(bp * net, float running_average_factor);
void bp_weight_histogram(bp * net,
                         unsigned int histogram[], int buckets,
                         float max_value);
int bp_prune_weights(bp * net, float threshold);

#endif
