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

#ifndef DEEPLEARNDATA_H
#define DEEPLEARNDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include "globals.h"
#include "deeplearn.h"
#include "deeplearn_images.h"

int deeplearndata_add(deeplearndata ** datalist,
                      int data_samples[],
                      float inputs[],
                      char ** inputs_text,
                      float outputs[],
                      int no_of_input_fields,
                      int no_of_outputs,
                      float input_range_min[],
                      float input_range_max[],
                      float output_range_min[],
                      float output_range_max[]);
int deeplearndata_index_data(
        deeplearndata * list,
        int samples,
        deeplearndata *** indexed_list,
        int* indexed_samples);
int deeplearndata_index_meta(
        deeplearndata_meta * list,
        int samples,
        deeplearndata_meta *** indexed_list,
        int* indexed_samples);
deeplearndata * deeplearndata_get(deeplearn * learner, int index);
deeplearndata * deeplearndata_get_training(deeplearn * learner, int index);
deeplearndata * deeplearndata_get_training_labeled(deeplearn * learner,
                                                   int index);
deeplearndata * deeplearndata_get_test(deeplearn * learner, int index);
int deeplearndata_add_training_sample(deeplearn * learner,
                                      deeplearndata * sample);
int deeplearndata_add_labeled_training_sample(deeplearn * learner,
                                              deeplearndata * sample);
int deeplearndata_add_test_sample(deeplearn * learner, deeplearndata * sample);
int deeplearndata_create_datasets(deeplearn * learner,
                                  int test_data_percentage);
int deeplearndata_training(deeplearn * learner);
float deeplearndata_get_performance(deeplearn * learner);
int deeplearndata_get_field_length(deeplearndata * data, int field_index);
int deeplearndata_update_field_lengths(int no_of_input_fields,
                                       int field_length[],
                                       deeplearndata * data);
int deeplearndata_read_csv(char * filename,
                           deeplearn * learner,
                           int no_of_hiddens, int hidden_layers,
                           int no_of_outputs, int * output_field_index,
                           int output_classes,
                           float error_threshold[],
                           unsigned int * random_seed);
#endif
