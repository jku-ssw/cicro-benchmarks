/*
 Estimating wine quality
 Copyright (C) 2015,2018  Bob Mottram <bob@freedombone.net>

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
#include "libdeep/deeplearndata.h"

#define TITLE     "Red Wine Quality Training"
#define DATA_FILE "winequality-red.csv"

deeplearn learner;

/**
* @brief Main function
*/
int main(int argc, char* argv[])
{
    int no_of_outputs = 1;
    int output_field_index[] = { 11 };
    float error_threshold_percent[] = { 1.6f, 2.05f, 4.0f, 9.5f };
    unsigned int random_seed = 123;
    int pruned_percent;

    /* load the data */
    printf("Loading data set\n");
    deeplearndata_read_csv(DATA_FILE,
                           &learner,
                           10, 3,
                           no_of_outputs,
                           output_field_index, 0,
                           error_threshold_percent,
                           &random_seed);

    printf("Number of training examples: %d\n",learner.training_data_samples);
    printf("Number of labeled training examples: %d\n",learner.training_data_labeled_samples);
    printf("Number of test examples: %d\n",learner.test_data_samples);
    printf("Number of Inputs: %d\n",learner.net->no_of_inputs);

    /* set learning rate */
    deeplearn_set_learning_rate(&learner, 0.2f);

    /* set percentage of dropouts */
    deeplearn_set_dropouts(&learner, 0.001f);

    learner.history.interval = 900000;

    deeplearn_set_title(&learner, TITLE);

    while (deeplearndata_training(&learner) != 0) {
    }

    printf("Training Completed\n");

    deeplearn_plot_weight_magnitude(&learner, 20, 1.5f, 1000, 1000);

    printf("Test data set performance is %.1f%%\n", deeplearndata_get_performance(&learner));

    pruned_percent = deeplearn_prune_weights(&learner, 0.9f);

    printf("Test data set performance after %d%% pruning is %.1f%%\n", pruned_percent, deeplearndata_get_performance(&learner));

    deeplearn_export(&learner, "export_wine_classifier.c");
    deeplearn_export(&learner, "export_wine_classifier_sketch.c");
    deeplearn_export(&learner, "export_wine_classifier.py");

    deeplearn_free(&learner);

    return 0;
}
