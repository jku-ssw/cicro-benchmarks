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

#include "deeplearn.h"

/**
 * @brief Returns a training error threshold for the given layer
 * @param learner Deep learner object
 * @param index Layer index
 * @returns Training error threshold (percentage value)
 */
float deeplearn_get_error_threshold(deeplearn * learner, int index)
{
    return(learner->error_threshold[index]);
}

/**
 * @brief Sets a training error threshold
 * @param learner Deep learner object
 * @param index Layer index
 * @param value Threshold value as a percentage
 */
void deeplearn_set_error_threshold(deeplearn * learner, int index, float value)
{
    learner->error_threshold[index] = value;
}

/**
 * @brief Initialise a deep learner
 * @param learner Deep learner object
 * @param no_of_inputs The number of input fields
 * @param no_of_hiddens The number of hidden units within each layer
 * @param hidden_layers The number of hidden layers
 * @param no_of_outputs The number of output units
 * @param error_threshold Minimum training error for each hidden layer plus
 *        the output layer
 * @param random_seed Random number generator seed
 */
int deeplearn_init(deeplearn * learner,
                   int no_of_inputs,
                   int no_of_hiddens,
                   int hidden_layers,
                   int no_of_outputs,
                   float error_threshold[],
                   unsigned int * random_seed)
{
    /* no training/test data yet */
    learner->data = 0;
    learner->data_samples = 0;
    learner->indexed_data = 0;
    learner->indexed_data_samples = 0;

    learner->training_data = 0;
    learner->training_data_samples = 0;
    learner->indexed_training_data = 0;
    learner->indexed_training_data_samples = 0;

    learner->training_data_labeled = 0;
    learner->training_data_labeled_samples = 0;
    learner->indexed_training_data_labeled = 0;
    learner->indexed_training_data_labeled_samples = 0;

    learner->test_data = 0;
    learner->test_data_samples = 0;
    learner->indexed_test_data = 0;
    learner->indexed_test_data_samples = 0;

    learner->no_of_input_fields = 0;
    learner->field_length = 0;

    deeplearn_history_init(&learner->history, "training.png",
                           "Training History",
                           "Time Step", "Training Error %");
    deeplearn_history_init(&learner->gradients_std, "weight_gradients_std.png",
                           "Weight Gradient Standard Deviation",
                           "Time Step", "Weight Gradient Std. Dev.");
    deeplearn_history_init(&learner->gradients_mean, "weight_gradients_mean.png",
                           "Average Weight Gradient",
                           "Time Step", "Weight Gradient mean");

    FLOATALLOC(learner->input_range_min, no_of_inputs);
    if (!learner->input_range_min)
        return -1;

    FLOATALLOC(learner->input_range_max, no_of_inputs);
    if (!learner->input_range_max) {
        free(learner->input_range_min);
        return -2;
    }

    FLOATALLOC(learner->output_range_min, no_of_outputs);
    if (!learner->output_range_min) {
        free(learner->input_range_max);
        free(learner->input_range_min);
        return -3;
    }

    FLOATALLOC(learner->output_range_max, no_of_outputs);
    if (!learner->output_range_max) {
        free(learner->output_range_min);
        free(learner->input_range_max);
        free(learner->input_range_min);
        return -4;
    }

    COUNTDOWN(i, no_of_inputs) {
        learner->input_range_min[i] = 99999;
        learner->input_range_max[i] = -99999;
    }

    COUNTDOWN(i, no_of_outputs) {
        learner->output_range_min[i] = 99999;
        learner->output_range_max[i] = -99999;
    }

    /* has not been trained */
    learner->training_complete = 0;

    /* create the error thresholds for each layer */
    FLOATALLOC(learner->error_threshold, hidden_layers+1);
    if (!learner->error_threshold) {
        free(learner->output_range_max);
        free(learner->output_range_min);
        free(learner->input_range_max);
        free(learner->input_range_min);
        return -5;
    }

    memcpy((void*)learner->error_threshold,
           (void*)error_threshold,
           (hidden_layers+1)*sizeof(float));

    /* set the current layer being trained */
    learner->current_hidden_layer = 0;

    /* create the network */
    learner->net = (bp*)malloc(sizeof(bp));
    if (!learner->net) {
        free(learner->error_threshold);
        free(learner->output_range_max);
        free(learner->output_range_min);
        free(learner->input_range_max);
        free(learner->input_range_min);
        return -6;
    }

    /* initialise the network */
    if (bp_init(learner->net,
                no_of_inputs, no_of_hiddens,
                hidden_layers, no_of_outputs,
                random_seed) != 0) {
        free(learner->net);
        free(learner->error_threshold);
        free(learner->output_range_max);
        free(learner->output_range_min);
        free(learner->input_range_max);
        free(learner->input_range_min);
        return -7;
    }

    /* create the autocoder */
    learner->autocoder = (ac**)malloc(sizeof(ac*)*hidden_layers);
    if (!learner->autocoder) {
        free(learner->net);
        free(learner->error_threshold);
        free(learner->output_range_max);
        free(learner->output_range_min);
        free(learner->input_range_max);
        free(learner->input_range_min);
        return -8;
    }

    COUNTUP(i, hidden_layers) {
        learner->autocoder[i] = (ac*)malloc(sizeof(ac));
        if (!learner->autocoder[i]) {
            COUNTDOWN(j, i) {
                free(learner->autocoder[j]);
            }
            free(learner->autocoder);
            free(learner->net);
            free(learner->error_threshold);
            free(learner->output_range_max);
            free(learner->output_range_min);
            free(learner->input_range_max);
            free(learner->input_range_min);
            return -9;
        }

        if (i == 0) {
            /* if this is the first hidden layer then number of inputs
               for the autocoder is the same as the number of
               neural net input units */
            if (autocoder_init(learner->autocoder[i], no_of_inputs,
                               HIDDENS_IN_LAYER(learner->net,i),
                               learner->net->random_seed) != 0) {
                COUNTDOWN(j, i) {
                    free(learner->autocoder[j]);
                }
                free(learner->autocoder);
                free(learner->net);
                free(learner->error_threshold);
                free(learner->output_range_max);
                free(learner->output_range_min);
                free(learner->input_range_max);
                free(learner->input_range_min);
                return -10;
            }
        }
        else {
            if (autocoder_init(learner->autocoder[i],
                               HIDDENS_IN_LAYER(learner->net,i-1),
                               HIDDENS_IN_LAYER(learner->net,i),
                               learner->net->random_seed) != 0) {
                COUNTDOWN(j, i) {
                    free(learner->autocoder[j]);
                }
                free(learner->autocoder);
                free(learner->net);
                free(learner->error_threshold);
                free(learner->output_range_max);
                free(learner->output_range_min);
                free(learner->input_range_max);
                free(learner->input_range_min);
                return -11;
            }
        }

    }

    learner->backprop_error = DEEPLEARN_UNKNOWN_ERROR;
    return 0;
}

/**
 * @brief Sets the title for graphs
 * @param learner Deep learner object
 * @param title Title to use
 */
void deeplearn_set_title(deeplearn * learner, char title[])
{
    sprintf(learner->history.title, "%s", title);
    sprintf(learner->gradients_std.title, "%s", title);
    sprintf(learner->gradients_mean.title, "%s", title);
}

/**
 * @brief Feeds the input values through the network towards the outputs
 * @param learner Deep learner object
 */
void deeplearn_feed_forward(deeplearn * learner)
{
    bp_feed_forward(learner->net, 0);
}

/**
 * @brief Returns true if currently training the final layer
 * @param learner Deep learner object
 * @returns True if training the last layer
 */
int deeplearn_training_last_layer(deeplearn * learner)
{
    return (learner->current_hidden_layer >=
            learner->net->hidden_layers);
}

/**
 * @brief Copy autocoder hidden units to a hidden layer of the deep learner
 * @param learner Deep learner object
 * @param hidden_layer Index of the layer to be copied
 */
void copy_autocoder_to_hidden_layer(deeplearn * learner, int hidden_layer)
{
    ac * autocoder = learner->autocoder[hidden_layer];
    /* for each unit on the hidden layer */
    COUNTDOWN(i, HIDDENS_IN_LAYER(learner->net,hidden_layer)) {
        bp_neuron * nrn  = learner->net->hiddens[hidden_layer][i];
        nrn->bias = autocoder->bias[i];
        memcpy((void*)nrn->weights,
               &autocoder->weights[i*autocoder->no_of_inputs],
               autocoder->no_of_inputs*sizeof(float));
    }
}

/**
 * @brief Trains the autocoder for a given hidden layer
 * @param net Backprop object
 * @param autocoder Autocoder object
 * @param current_layer Index of the current hidden layer
 */
void deeplearn_pretrain(bp * net, ac * autocoder, int current_layer)
{
    bp_feed_forward_layers(net, current_layer);
    if (current_layer > 0) {
        /* copy the hidden unit values to the inputs
           of the autocoder */
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,current_layer-1)) {
            float hidden_value = bp_get_hidden(net, current_layer-1, i);
            autocoder_set_input(autocoder, i, hidden_value);
        }
    }
    else {
        /* copy the input unit values to the inputs
           of the autocoder */
        COUNTDOWN(i, net->no_of_inputs)
            autocoder_set_input(autocoder, i, bp_get_input(net, i));
    }
    autocoder_update(autocoder);
}

/**
 * @brief Calculate weight gradient standard deviations for each layer
 * @param learner Deep learner object
 */
static int deeplearn_update_weight_gradients(deeplearn * learner)
{
    float weight_gradients[HISTORY_DIMENSIONS];

    if (learner->gradients_std.ctr+1 < learner->gradients_std.step)
       return 0;

    if (learner->net->hidden_layers >= HISTORY_DIMENSIONS)
        return 0;

    COUNTDOWN(layer_index, learner->net->hidden_layers) {
        weight_gradients[layer_index] =
            bp_weight_gradient_std(learner->net, layer_index);
    }

    deeplearn_history_update_from_array(&learner->gradients_std,
                                        weight_gradients,
                                        PLOT_RUNNING_AVERAGE);

    COUNTDOWN(layer_index, learner->net->hidden_layers) {
        weight_gradients[layer_index] =
            bp_weight_gradient_mean(learner->net, layer_index);
    }

    deeplearn_history_update_from_array(&learner->gradients_mean,
                                        weight_gradients,
                                        PLOT_RUNNING_AVERAGE);

    return 1;
}

/**
 * @brief Performs training initially using autocoders
 *        for each hidden
 *        layer and eventually for the entire network.
 * @param learner Deep learner object
 */
void deeplearn_update(deeplearn * learner)
{
    float minimum_error_percent = 0;
    int current_layer = learner->current_hidden_layer;

    /* only continue if training is not complete */
    if (learner->training_complete == 1)
        return;

    /* get the maximum backprop error after which a layer
       will be considered to have been trained */
    minimum_error_percent =
        learner->error_threshold[current_layer];

    /* If there is only a single hidden layer */
    if ((current_layer == 0) &&
        (learner->net->hidden_layers == 1)) {
        current_layer = 1;
        learner->current_hidden_layer = current_layer;
    }

    /* pretraining of autocoders */
    if (current_layer < learner->net->hidden_layers) {

        /* train the autocoder for this layer */
        deeplearn_pretrain(learner->net,
                           learner->autocoder[current_layer],
                           current_layer);

        /* update the backprop error value from the autocoder */
        learner->backprop_error =
            learner->autocoder[current_layer]->backprop_error_percent;

        /* If below the error threshold.
           Only do this after a minimum number of itterations
           in order to allow the running average to stabilise */
        if ((learner->backprop_error != DEEPLEARN_UNKNOWN_ERROR) &&
            (learner->backprop_error < minimum_error_percent) &&
            (learner->autocoder[current_layer]->itterations > 100)) {

            copy_autocoder_to_hidden_layer(learner, current_layer);

            /* advance to the next hidden layer */
            learner->current_hidden_layer++;

            /* reset the error value */
            learner->backprop_error = DEEPLEARN_UNKNOWN_ERROR;
        }
    }
    else {
        bp_update(learner->net,0);

        /* update the backprop error value */
        learner->backprop_error = learner->net->backprop_error_percent;

        /* set the training completed flag */
        if (learner->backprop_error < minimum_error_percent)
            learner->training_complete = 1;

        /* record the history of error values */
        deeplearn_update_weight_gradients(learner);
    }

    /* record the history of error values */
    deeplearn_history_update(&learner->history, learner->backprop_error);

    /* increment the number of itterations */
    if (learner->net->itterations < UINT_MAX)
        learner->net->itterations++;
}

/**
 * @brief Perform continuous unsupervised learning
 * @param learner deep learner object
 */
void deeplearn_update_continuous(deeplearn * learner)
{
    learner->backprop_error = 0;

    COUNTUP(i, learner->net->hidden_layers) {
        /* train the autocoder for this layer */
        deeplearn_pretrain(learner->net, learner->autocoder[i], i);

        /* update the backprop error value */
        learner->backprop_error += learner->autocoder[i]->backprop_error_percent;

        copy_autocoder_to_hidden_layer(learner, i);
    }

    learner->backprop_error /= learner->net->hidden_layers;

    /* record the history of error values */
    deeplearn_history_update(&learner->history, learner->backprop_error);
}

/**
 * @brief Deallocates memory for the given deep learner object
 * @param learner Deep learner object
 */
void deeplearn_free(deeplearn * learner)
{
    /* clear any data */
    deeplearndata * sample = learner->data;
    deeplearndata * prev_sample;

    free(learner->input_range_min);
    free(learner->input_range_max);
    free(learner->output_range_min);
    free(learner->output_range_max);
    if (learner->field_length != 0)
        free(learner->field_length);
    if (learner->indexed_data != 0)
        free(learner->indexed_data);
    if (learner->indexed_training_data != 0)
        free(learner->indexed_training_data);
    if (learner->indexed_training_data_labeled != 0)
        free(learner->indexed_training_data_labeled);
    if (learner->indexed_test_data != 0)
        free(learner->indexed_test_data);

    while (sample != 0) {
        prev_sample = sample;
        sample = (deeplearndata *)sample->next;
        if (prev_sample->inputs_text != 0) {
            /* clear any input text strings */
            COUNTDOWN(i, learner->no_of_input_fields) {
                if (prev_sample->inputs_text[i] != 0)
                    free(prev_sample->inputs_text[i]);
            }
            free(prev_sample->inputs_text);
        }
        /* clear numerical fields */
        free(prev_sample->inputs);
        free(prev_sample->outputs);
        free(prev_sample);
    }

    /* free training samples */
    deeplearndata_meta * training_sample = learner->training_data;
    deeplearndata_meta * prev_training_sample;
    while (training_sample != 0) {
        prev_training_sample = training_sample;
        training_sample = (deeplearndata_meta *)training_sample->next;
        free(prev_training_sample);
    }

    /* free labeled training samples */
    deeplearndata_meta * training_sample_labeled =
        learner->training_data_labeled;
    deeplearndata_meta * prev_training_sample_labeled;
    while (training_sample_labeled != 0) {
        prev_training_sample_labeled = training_sample_labeled;
        training_sample_labeled =
            (deeplearndata_meta *)training_sample_labeled->next;
        free(prev_training_sample_labeled);
    }

    /* free test samples */
    deeplearndata_meta * test_sample = learner->test_data;
    deeplearndata_meta * prev_test_sample;
    while (test_sample != 0) {
        prev_test_sample = test_sample;
        test_sample = (deeplearndata_meta *)test_sample->next;
        free(prev_test_sample);
    }

    /* free the error thresholds */
    free(learner->error_threshold);

    /* free the autocoder */
    COUNTDOWN(i, learner->net->hidden_layers) {
        autocoder_free(learner->autocoder[i]);
        free(learner->autocoder[i]);
        learner->autocoder[i] = 0;
    }
    free(learner->autocoder);

    /* free the learner */
    bp_free(learner->net);
    free(learner->net);
}

/**
 * @brief Sets the value of an input to the network
 * @param learner Deep learner object
 * @param index Index number of the input unit
 * @param value Value to set the input unit to in the range 0.0 to 1.0
 */
void deeplearn_set_input(deeplearn * learner, int index, float value)
{
    bp_set_input(learner->net, index, value);
}

/**
 * @brief Sets the inputs to a text string
 *        Note that this sets the entire inputs, not a field
 * @param learner Deep learner object
 * @param text The text string
 */
void deeplearn_set_input_text(deeplearn * learner, char * text)
{
    bp_set_input_text(learner->net, text);
}

/**
 * @brief Sets inputs from the given data sample.
 *        The sample can contain arbitrary floating point values, so these
 *        need to be normalised into a NEURON_LOW -> NEURON_HIGH range
 * @param learner Deep learner object
 */
void deeplearn_set_inputs(deeplearn * learner, deeplearndata * sample)
{
    float value, range, normalised;
    int pos = 0;

    COUNTUP(i, learner->no_of_input_fields) {
        if (learner->field_length[i] > 0) {
            /* text value */
            enc_text_to_binary(sample->inputs_text[i],
                               learner->net->inputs,
                               learner->net->no_of_inputs,
                               pos, learner->field_length[i]/CHAR_BITS);
            pos += learner->field_length[i];
        }
        else {
            /* numerical */
            value = sample->inputs[i];
            range = learner->input_range_max[i] - learner->input_range_min[i];
            if (range > 0) {
                normalised =
                    (((value - learner->input_range_min[i])/range)*
                     NEURON_RANGE) + NEURON_LOW;
                deeplearn_set_input(learner, pos, normalised);
            }
            pos++;
        }
    }
}

/**
 * @brief Sets a numeric value for the given input field
 * @param learner Deep learner object
 * @param fieldindex Index number of the input field.
 *        This is not necessarily the same as the input index
 * @param value Value to set the input unit to in the range 0.0 to 1.0
 * @returns zero on success
 */
int deeplearn_set_input_field(deeplearn * learner, int fieldindex, float value)
{
    int pos=0;

    /* No fields are defined
       Assume you meant fieldindex to be the input index */
    if (learner->no_of_input_fields == 0) {
        bp_set_input(learner->net, fieldindex, value);
        return -1;
    }

    /* no field length array defined */
    if (learner->field_length == 0)
        return -2;

    /* this is a text field */
    if (learner->field_length[fieldindex] > 0)
        return -3;

    /* get the offset (first input unit index) of the input field */
    COUNTUP(i, fieldindex) {
        if (learner->field_length[i] == 0)
            pos++;
        else
            pos += learner->field_length[i];
    }

    /* set the value */
    bp_set_input(learner->net, pos, value);
    return 0;
}

/**
 * @brief Sets a text value for the given input field
 * @param learner Deep learner object
 * @param fieldindex Index number of the input field.
 *        This is not necessarily the same as the input index
 * @param text Text value for the field
 * @returns zero on success
 */
int deeplearn_set_input_field_text(deeplearn * learner, int fieldindex,
                                   char * text)
{
    int pos=0;

    /* No fields are defined */
    if (learner->no_of_input_fields == 0)
        return -1;

    /* no field length array defined */
    if (learner->field_length == 0)
        return -2;

    /* this is a text field */
    if (learner->field_length[fieldindex] == 0)
        return -3;

    /* get the offset (first input unit index) of the input field */
    COUNTUP(i, fieldindex) {
        if (learner->field_length[i] == 0)
            pos++;
        else
            pos += learner->field_length[i];
    }

    /* set the value */
    enc_text_to_binary(text,
                       learner->net->inputs,
                       learner->net->no_of_inputs, pos,
                       learner->field_length[fieldindex]/CHAR_BITS);
    return 0;
}

/**
 * @brief Sets the value of an output unit
 * @param learner Deep learner object
 * @param index Index number of the output unit
 * @param value Value to set the output unit to in the range 0.0 to 1.0
 */
void deeplearn_set_output(deeplearn * learner, int index, float value)
{
    bp_set_output(learner->net, index, value);
}

/**
 * @brief Sets outputs from the given data sample.
 *        The sample can contain arbitrary floating point values, so these
 *        need to be normalised into a NEURON_LOW -> NEURON_HIGH range
 * @param learner Deep learner object
 * @param sample The data sample from which to obtain the output values
 */
void deeplearn_set_outputs(deeplearn * learner, deeplearndata * sample)
{
    COUNTDOWN(i, learner->net->no_of_outputs) {
        float value = sample->outputs[i];
        float range =
            learner->output_range_max[i] - learner->output_range_min[i];
        if (range > 0) {
            float normalised =
                (((value - learner->output_range_min[i])/range)*
                 NEURON_RANGE) + NEURON_LOW;
            deeplearn_set_output(learner, i, normalised);
        }
    }
}

/**
 * @brief Returns the values of outputs within their normal range
 * @param learner Deep learner object
 * @param outputs The returned output values
 */
void deeplearn_get_outputs(deeplearn * learner, float outputs[])
{
    COUNTDOWN(i, learner->net->no_of_outputs) {
        float value = deeplearn_get_output(learner, i);
        float range =
            learner->output_range_max[i] - learner->output_range_min[i];
        if (range > 0)
            outputs[i] =
                (((value - NEURON_LOW)/NEURON_RANGE)*range) +
                learner->output_range_min[i];
    }
}

/**
 * @brief Returns the value of an output unit
 * @param learner Deep learner object
 * @param index Index number of the output unit
 * @return Value of the output unit to in the range 0.0 to 1.0
 */
float deeplearn_get_output(deeplearn * learner, int index)
{
    return bp_get_output(learner->net, index);
}

/**
 * @brief Returns the desired output value
 * @param learner Deep learner object
 * @param index Index number of the output unit
 * @return Desired value of the output unit to in the range 0.0 to 1.0
 */
float deeplearn_get_desired(deeplearn * learner, int index)
{
    return bp_get_desired(learner->net, index);
}

/**
 * @brief Gets the output class as an integer value
 * @param learner Deep learner object
 * @return output class
 */
int deeplearn_get_class(deeplearn * learner)
{
    int class = -9999;
    float max = -1;

    COUNTDOWN(i, learner->net->no_of_outputs) {
        if (bp_get_output(learner->net, i) > max) {
            max = bp_get_output(learner->net, i);
            class = i;
        }
    }
    return class;
}

/**
 * @brief Sets the output class
 * @param learner Deep learner object
 * @param class The class number
 */
void deeplearn_set_class(deeplearn * learner, int class)
{
    COUNTDOWN(i, learner->net->no_of_outputs) {
        if (i != class)
            bp_set_output(learner->net, i, NEURON_LOW);
        else
            bp_set_output(learner->net, i, NEURON_HIGH);
    }
}

/**
 * @brief Saves the given deep learner object to a file
 * @param fp File pointer
 * @param learner Deep learner object
 * @return zero value on success
 */
int deeplearn_save(FILE * fp, deeplearn * learner)
{
    if (INTWRITE(learner->training_complete) == 0)
        return -1;

    if (INTWRITE(learner->current_hidden_layer) == 0)
        return -3;

    if (FLOATWRITE(learner->backprop_error) == 0)
        return -4;

    if (INTWRITE(learner->no_of_input_fields) == 0)
        return -5;

    if (learner->no_of_input_fields > 0) {
        if (INTWRITEARRAY(learner->field_length,
                          learner->no_of_input_fields) == 0)
            return -6;
    }

    if (bp_save(fp, learner->net) != 0)
        return -7;

    COUNTUP(i, learner->net->hidden_layers) {
        if (autocoder_save(fp, learner->autocoder[i]) != 0)
            return -8;
    }

    /* save error thresholds */
    if (FLOATWRITEARRAY(learner->error_threshold,
                        learner->net->hidden_layers+1) == 0) {
        return -9;
    }

    /* save ranges */
    if (FLOATWRITEARRAY(learner->input_range_min,
                        learner->net->no_of_inputs) == 0)
        return -10;

    if (FLOATWRITEARRAY(learner->input_range_max,
                        learner->net->no_of_inputs) == 0)
        return -11;

    if (FLOATWRITEARRAY(learner->output_range_min,
                        learner->net->no_of_outputs) == 0)
        return -12;

    if (FLOATWRITEARRAY(learner->output_range_max,
                        learner->net->no_of_outputs) == 0)
        return -13;

    if (fwrite(&learner->history, sizeof(deeplearn_history), 1, fp) == 0)
        return -14;

    if (fwrite(&learner->gradients_std, sizeof(deeplearn_history), 1, fp) == 0)
        return -15;

    if (fwrite(&learner->gradients_mean, sizeof(deeplearn_history), 1, fp) == 0)
        return -16;

    return 0;
}

/**
 * @brief Loads a deep learner object from file
 * @param fp File pointer
 * @param learner Deep learner object
 * @return zero value on success
 */
int deeplearn_load(FILE * fp, deeplearn * learner)
{
    /* no training/test data yet */
    learner->data = 0;
    learner->data_samples = 0;
    learner->training_data = 0;
    learner->training_data_samples = 0;
    learner->training_data_labeled = 0;
    learner->training_data_labeled_samples = 0;
    learner->test_data = 0;
    learner->test_data_samples = 0;

    if (INTREAD(learner->training_complete) == 0)
        return -1;

    if (INTREAD(learner->current_hidden_layer) == 0)
        return -3;

    if (FLOATREAD(learner->backprop_error) == 0)
        return -4;

    if (INTREAD(learner->no_of_input_fields) == 0)
        return -5;

    learner->field_length = 0;
    if (learner->no_of_input_fields > 0) {
        INTALLOC(learner->field_length,
                 learner->no_of_input_fields);
        if (INTREADARRAY(learner->field_length,
                         learner->no_of_input_fields) == 0)
            return -6;
    }

    learner->net = (bp*)malloc(sizeof(bp));

    if (bp_load(fp, learner->net) != 0)
        return -7;

    learner->autocoder = (ac**)malloc(sizeof(ac*)*learner->net->hidden_layers);
    if (!learner->autocoder)
        return -8;

    COUNTUP(i, learner->net->hidden_layers) {
        learner->autocoder[i] = (ac*)malloc(sizeof(ac));
        if (autocoder_load(fp, learner->autocoder[i], 1) != 0)
            return -9;
    }

    /* load error thresholds */
    FLOATALLOC(learner->error_threshold, learner->net->hidden_layers+1);
    if (FLOATREADARRAY(learner->error_threshold,
                       learner->net->hidden_layers+1) == 0)
        return -10;

    /* load ranges */
    FLOATALLOC(learner->input_range_min, learner->net->no_of_inputs);
    if (!learner->input_range_min)
        return -15;

    FLOATALLOC(learner->input_range_max, learner->net->no_of_inputs);
    if (!learner->input_range_max)
        return -16;

    FLOATALLOC(learner->output_range_min, learner->net->no_of_outputs);
    if (!learner->output_range_min)
        return -17;

    FLOATALLOC(learner->output_range_max, learner->net->no_of_outputs);
    if (!learner->output_range_max)
        return -18;

    if (FLOATREADARRAY(learner->input_range_min,
                       learner->net->no_of_inputs) == 0)
        return -19;

    if (FLOATREADARRAY(learner->input_range_max,
                       learner->net->no_of_inputs) == 0)
        return -20;

    if (FLOATREADARRAY(learner->output_range_min,
                       learner->net->no_of_outputs) == 0)
        return -21;

    if (FLOATREADARRAY(learner->output_range_max,
                       learner->net->no_of_outputs) == 0)
        return -22;

    if (fread(&learner->history, sizeof(deeplearn_history), 1, fp) == 0)
        return -23;

    if (fread(&learner->gradients_std, sizeof(deeplearn_history), 1, fp) == 0)
        return -24;

    if (fread(&learner->gradients_mean, sizeof(deeplearn_history), 1, fp) == 0)
        return -25;

    return 0;
}

/**
 * @brief Compares two deep learners and returns a greater
 *        than zero value if they are the same
 * @param learner1 First deep learner object
 * @param learner2 Second deep learner object
 * @return Greater than zero if the two learners are the same
 */
int deeplearn_compare(deeplearn * learner1,
                      deeplearn * learner2)
{
    int retval;

    if (learner1->current_hidden_layer !=
        learner2->current_hidden_layer)
        return -1;

    if (learner1->backprop_error != learner2->backprop_error)
        return -2;

    retval = bp_compare(learner1->net,learner2->net);
    if (retval < 1) return -3;
    if (learner1->history.index !=
        learner2->history.index)
        return -5;

    if (learner1->history.ctr !=
        learner2->history.ctr)
        return -6;

    if (learner1->history.step !=
        learner2->history.step)
        return -7;

    COUNTDOWN(i, learner1->history.index) {
        if (learner1->history.history[i][0] !=
            learner2->history.history[i][0])
            return -8;
    }

    if (learner1->history.itterations !=
        learner2->history.itterations)
        return -9;

    COUNTDOWN(i, learner1->net->hidden_layers+1) {
        if (learner1->error_threshold[i] !=
            learner2->error_threshold[i])
            return -10;
    }

    COUNTDOWN(i, learner1->net->no_of_inputs) {
        if (learner1->input_range_min[i] !=
            learner2->input_range_min[i])
            return -11;

        if (learner1->input_range_max[i] !=
            learner2->input_range_max[i])
            return -12;
    }

    COUNTDOWN(i, learner1->net->no_of_outputs) {
        if (learner1->output_range_min[i] !=
            learner2->output_range_min[i])
            return -13;

        if (learner1->output_range_max[i] !=
            learner2->output_range_max[i])
            return -14;
    }

    if (learner1->no_of_input_fields !=
        learner2->no_of_input_fields)
        return -15;

    return 1;
}

/**
 * @brief Plots the training error for the given learner
 * @param learner Deep learner object
 * @param image_width Width of the image in pixels
 * @param image_height Height of the image in pixels
 * @return zero on success
 */
int deeplearn_plot_history(deeplearn * learner,
                           int image_width, int image_height)
{
    return deeplearn_history_plot(&learner->history,
                                  image_width, image_height);
}

/**
 * @brief Plot the weight gradients for the given learner
 * @param gradient_type The type of gradient to be plotted
 * @param learner Deep learner object
 * @param image_width Width of the image in pixels
 * @param image_height Height of the image in pixels
 * @return zero on success
 */
int deeplearn_plot_gradients(int gradient_type,
                             deeplearn * learner,
                             int image_width, int image_height)
{
    if (gradient_type == GRADIENT_STANDARD_DEVIATION)
        return deeplearn_history_plot(&learner->gradients_std,
                                      image_width, image_height);
    return deeplearn_history_plot(&learner->gradients_mean,
                                  image_width, image_height);
}

/**
 * @brief Uses phosphene to plot the weight magnitude histogram
 * @param learner Deep learner object
 * @param buckets Number of histogram buckets
 * @param max_magnitude The maximum weight magnitude
 * @param img_width Width of the image in pixels
 * @param img_height Height of the image in pixels
 * @return zero on success
 */
int deeplearn_plot_weight_magnitude(deeplearn * learner,
                                    int buckets,
                                    float max_magnitude,
                                    int img_width, int img_height)
{
    scope s;
    double max_voltage = 1000;
    double min_voltage = 0;
    unsigned int grid_horizontal = 20;
    unsigned int grid_vertical = 16;
    unsigned char * img;
    unsigned int * histogram = NULL;


    UCHARALLOC(img, img_width*img_height*3);
    if (!img)
        return 1;

    UINTALLOC(histogram, buckets);
    if (!histogram) {
        free(img);
        return 2;
    }
    bp_weight_histogram(learner->net,
                        histogram, buckets,
                        max_magnitude);

    create_scope(&s, 1);
    s.offset_ms = 0;
    s.marker_position = 0;
    s.time_ms = buckets;
    s.step_ms = 1;
    s.horizontal_multiplier = 1;

    COUNTUP(p, buckets) {
        scope_update(&s, 0, histogram[p],
                     min_voltage, max_voltage,
                     p, 0);
    }

    scope_draw_graph(&s, PHOSPHENE_DRAW_ALL, 3, 100,
                     grid_horizontal, grid_vertical,
                     img, img_width, img_height,
                     PHOSPHENE_SHAPE_RECTANGULAR,
                     "Weight Magnitude",
                     "Frequency", "Magnitude", 25, 4);

    phosphene_write_png_file("weight_magnitude.png",
                             img_width, img_height, 24, img);

    free(img);
    free(histogram);

    return 0;
}


/**
 * @brief Updates the input units from a patch within a larger image
 * @param learner Deep learner object
 * @param img Image buffer (1 byte per pixel)
 * @param image_width Width of the image in pixels
 * @param image_height Height of the image in pixels
 * @param tx Top left x coordinate of the patch
 * @param ty Top left y coordinate of the patch
 */
int deeplearn_inputs_from_image_patch(deeplearn * learner,
                                      unsigned char * img,
                                      int image_width, int image_height,
                                      int tx, int ty)
{
    return bp_inputs_from_image_patch(learner->net,
                                      img, image_width, image_height,
                                      tx, ty);
}

/**
 * @brief Updates the input units from an image
 * @param learner Deep learner object
 * @param img Image buffer (1 byte per pixel)
 * @param image_width Width of the image in pixels
 * @param image_height Height of the image in pixels
 */
void deeplearn_inputs_from_image(deeplearn * learner,
                                 unsigned char * img,
                                 int image_width, int image_height)
{
    bp_inputs_from_image(learner->net, img, image_width, image_height);
}

/**
 * @brief Updates the input units from an image
 * @param learner Deep learner object
 * @param conv Convolutional network instance
 * @returns zero on success
 */
int deeplearn_inputs_from_convnet(deeplearn * learner, deeplearn_conv * conv)
{
    return bp_inputs_from_convnet(learner->net, conv);
}

/**
 * @brief Sets the learning rate
 * @param learner Deep learner object
 * @param rate the learning rate in the range 0.0 to 1.0
 */
void deeplearn_set_learning_rate(deeplearn * learner, float rate)
{
    learner->net->learning_rate = rate;

    COUNTDOWN(i, learner->net->hidden_layers)
        learner->autocoder[i]->learning_rate = rate;
}

/**
 * @brief Sets the percentage of units which drop out during training
 * @param learner Deep learner object
 * @param dropout_percent Percentage of units which drop out in the range 0 to 100
 */
void deeplearn_set_dropouts(deeplearn * learner, float dropout_percent)
{
    learner->net->dropout_percent = dropout_percent;

    COUNTDOWN(i, learner->net->hidden_layers)
        learner->autocoder[i]->dropout_percent = dropout_percent;
}

/**
 * @brief Exports a trained network as a standalone C program
 * @param learner Deep learner object
 * @param export_type The flavor of C
 * @param filename The C source file to be produced
 * @returns zero on success
 */
static int deeplearn_export_c_base(deeplearn * learner, int export_type,
                                   char * filename)
{
    FILE * fp;
    int no_of_weights;

    fp = fopen(filename,"w");
    if (!fp)
        return -1;

    if (export_type == EXPORT_C99) {
        fprintf(fp,"%s\n", "#include <stdio.h>");
        fprintf(fp,"%s\n", "#include <stdlib.h>");

        if (learner->no_of_input_fields > 0)
            fprintf(fp,"%s\n", "#include <string.h>");

        fprintf(fp,"%s\n\n", "#include <math.h>");
    }

#if ACTIVATION_FUNCTION == AF_SIGMOID
    fprintf(fp,"%s\n\n", "#define AF(adder) (1.0f / (1.0f + exp(-(adder))))");
#elif ACTIVATION_FUNCTION == AF_TANH
    fprintf(fp,"%s\n\n", "#define AF(adder) " \
            "((((2.0f / (1.0f + exp(-(2*adder)))) - 1.0f)*0.5f)+0.5f)");
#elif ACTIVATION_FUNCTION == AF_LINEAR
    fprintf(fp,"%s\n\n", "#define AF(adder) " \
            "((adder) < 1.0f ? ((adder) > -1.0f ? " \
            "(((adder)*0.5f)+0.5f) : 0.0f) : 1.0f)");
#endif

    if (learner->no_of_input_fields > 0)
        fprintf(fp, "const int no_of_input_fields = %d;\n",
                learner->no_of_input_fields);

    fprintf(fp, "const int no_of_inputs = %d;\n",
            learner->net->no_of_inputs);
    fprintf(fp, "const int no_of_hiddens = %d;\n",
            learner->net->no_of_hiddens);
    fprintf(fp, "const int no_of_outputs = %d;\n",
            learner->net->no_of_outputs);
    fprintf(fp, "const int hidden_layers = %d;\n\n",
            learner->net->hidden_layers);

    /* field lengths */
    if (learner->field_length != 0) {
        if (learner->no_of_input_fields > 0) {
            fprintf(fp, "%s", "int field_length[] = {\n");
            fprintf(fp, "%s", "  ");
            COUNTUP(i, learner->no_of_input_fields) {
                fprintf(fp, "%d", learner->field_length[i]);
                if (i < learner->no_of_input_fields-1)
                    fprintf(fp, ",");
            }
            fprintf(fp, "%s", "\n};\n\n");
        }
    }

    /* ranges */
    fprintf(fp, "%s", "float input_range_min[] = {\n");
    fprintf(fp, "%s", "  ");
    COUNTUP(i, learner->net->no_of_inputs) {
        fprintf(fp, "%.10f", learner->input_range_min[i]);
        if (i < learner->net->no_of_inputs-1)
            fprintf(fp, ",");
    }
    fprintf(fp, "%s", "\n};\n\n");
    fprintf(fp, "%s", "float input_range_max[] = {\n");
    fprintf(fp, "%s", "  ");
    COUNTUP(i, learner->net->no_of_inputs) {
        fprintf(fp, "%.10f", learner->input_range_max[i]);
        if (i < learner->net->no_of_inputs-1)
            fprintf(fp, ",");
    }
    fprintf(fp, "%s", "\n};\n\n");
    fprintf(fp, "%s", "float output_range_min[] = {\n");
    fprintf(fp, "%s", "  ");
    COUNTUP(i, learner->net->no_of_outputs) {
        fprintf(fp, "%.10f", learner->output_range_min[i]);
        if (i < learner->net->no_of_outputs-1)
            fprintf(fp, ",");
    }
    fprintf(fp, "%s", "\n};\n\n");
    fprintf(fp, "%s", "float output_range_max[] = {\n");
    fprintf(fp, "%s", "  ");
    COUNTUP(i, learner->net->no_of_outputs) {
        fprintf(fp, "%.10f", learner->output_range_max[i]);
        if (i < learner->net->no_of_outputs-1)
            fprintf(fp, ",");
    }
    fprintf(fp, "%s", "\n};\n\n");

    /* hidden unit weights */
    COUNTUP(i, learner->net->hidden_layers) {
        fprintf(fp,
                "float hidden_layer_%d_weights[] = {\n  ", i);

        if (i == 0)
            no_of_weights = learner->net->no_of_inputs;
        else
            no_of_weights = HIDDENS_IN_LAYER(learner->net, i-1);

        COUNTUP(j, HIDDENS_IN_LAYER(learner->net, i)) {
            COUNTUP(k, no_of_weights) {
                float w = learner->net->hiddens[i][j]->weights[k];
                if (w != 0)
                    fprintf(fp, "%.10f", w);
                else
                    fprintf(fp, "%d", 0);
                if (!((j == HIDDENS_IN_LAYER(learner->net, i)-1) &&
                      (k == no_of_weights-1)))
                    fprintf(fp, ",");
            }
        }
        fprintf(fp, "%s", "\n};\n\n");
    }

    /* hidden unit biases */
    COUNTUP(i, learner->net->hidden_layers) {
        fprintf(fp,
                "float hidden_layer_%d_bias[] = {\n  ", i);
        COUNTUP(j, HIDDENS_IN_LAYER(learner->net, i)) {
            fprintf(fp,"%.10f",learner->net->hiddens[i][j]->bias);
            if (j < HIDDENS_IN_LAYER(learner->net, i)-1)
                fprintf(fp, ",");
        }
        fprintf(fp, "\n};\n\n");
    }

    /* output unit weights */
    fprintf(fp, "%s",
            "float output_layer_weights[] = {\n  ");
    COUNTUP(i, learner->net->no_of_outputs) {
        COUNTUP(j, HIDDENS_IN_LAYER(learner->net,
                                       learner->net->hidden_layers-1)) {
            float w = learner->net->outputs[i]->weights[j];
            if (w != 0)
                fprintf(fp, "%.10f", w);
            else
                fprintf(fp, "%d", 0);
            if (!((i == learner->net->no_of_outputs-1) &&
                  (j == HIDDENS_IN_LAYER(learner->net,
                                            learner->net->hidden_layers-1)-1)))
                fprintf(fp, ",");
        }
    }
    fprintf(fp, "%s", "\n};\n\n");

    /* output unit biases */
    fprintf(fp, "%s",
            "float output_layer_bias[] = {\n  ");
    COUNTUP(i, learner->net->no_of_outputs) {
        fprintf(fp, "%.10f",
                learner->net->outputs[i]->bias);
        if (i < learner->net->no_of_outputs-1)
            fprintf(fp, ",");
    }
    fprintf(fp, "%s", "\n};\n\n");
    fprintf(fp, "float inputs[%d];\n",learner->net->no_of_inputs);
    fprintf(fp, "float network_inputs[%d];\n",learner->net->no_of_inputs);
    fprintf(fp, "float prev_hiddens[%d];\n",learner->net->no_of_hiddens);
    fprintf(fp, "float hiddens[%d];\n",learner->net->no_of_hiddens);
    fprintf(fp, "float outputs[%d];\n\n",learner->net->no_of_outputs);

    if (learner->no_of_input_fields > 0) {
        fprintf(fp, "%s", "/* Encode some text into the input units */\n");
        fprintf(fp, "%s", "void encode_text(char * text,\n");
        fprintf(fp, "%s",
                "                 float * inputs, int no_of_inputs,\n");
        fprintf(fp, "%s",
                "                 int offset, int max_field_length_chars)\n");

        fprintf(fp, "%s",
                "{\n  int pos = offset, i, bit, max_chars = strlen(text);\n\n");

        fprintf(fp, "  if (max_chars > (no_of_inputs-offset)/%d) {\n",
                (int)CHAR_BITS);
        fprintf(fp, "    max_chars = ((no_of_inputs-offset)/%d);\n",
                (int)CHAR_BITS);
        fprintf(fp, "%s", "  }\n");
        fprintf(fp, "%s", "  if (max_chars > max_field_length_chars) {\n");
        fprintf(fp, "%s", "    max_chars = max_field_length_chars;\n");
        fprintf(fp, "%s", "  }\n\n");

        fprintf(fp, "%s", "  /* for each character in the string */\n");
        fprintf(fp, "%s", "  for (i = 0; i < max_chars; i++) {\n");
        fprintf(fp, "%s", "    /* set the bits for this character */\n");
        fprintf(fp, "    for (bit = 0; bit < %d; bit++, pos++) {\n",
                (int)CHAR_BITS);
        fprintf(fp, "%s", "      if (text[i] & (1<<bit)) {\n");
        fprintf(fp,       "        inputs[pos] = %.2f;\n", NEURON_HIGH);
        fprintf(fp, "%s", "      }\n");
        fprintf(fp, "%s", "      else {\n");
        fprintf(fp,       "        inputs[pos] = %.2f;\n", NEURON_LOW);
        fprintf(fp, "%s", "      }\n");
        fprintf(fp, "%s", "    }\n");
        fprintf(fp, "%s", "  }\n");
        fprintf(fp, "%s",
                "  /* set the remaining inputs within the field to neutral */\n");
        fprintf(fp, "%s", "  while (i < max_field_length_chars) {\n");
        fprintf(fp,       "    for (bit = 0; bit < %d; bit++) {\n",
                (int)CHAR_BITS);
        fprintf(fp, "%s", "      if (pos >= no_of_inputs) {\n");
        fprintf(fp, "%s", "        i = max_field_length_chars;\n");
        fprintf(fp, "%s", "        break;\n");
        fprintf(fp, "%s", "      }\n");
        fprintf(fp,       "      inputs[pos++] = %.1f;\n", NEURON_UNKNOWN);
        fprintf(fp, "%s", "    }\n");
        fprintf(fp, "%s", "    i++;\n");
        fprintf(fp, "%s", "  }\n");
        fprintf(fp, "%s", "}\n\n");
    }

    if (export_type == EXPORT_C99)
        fprintf(fp, "%s", "int main(int argc, char* argv[])\n");
    else {
        fprintf(fp, "%s", "void setup()\n");
        fprintf(fp, "%s", "{\n");
        fprintf(fp, "%s", "  Serial.begin(115200);\n");
        fprintf(fp, "%s", "}\n\n");
        fprintf(fp, "%s", "void loop()\n");
    }

    fprintf(fp, "%s", "{\n");

    if (learner->no_of_input_fields == 0)
        fprintf(fp, "%s", "  int i,j;\n");
    else
        fprintf(fp, "%s", "  int i,j,pos;\n");

    fprintf(fp, "%s", "  float sum;\n\n");

    if (export_type == EXPORT_C99) {
        if (learner->no_of_input_fields == 0)
            fprintf(fp, "  if (argc < %d) return -1;\n\n",
                    learner->net->no_of_inputs);
        else
            fprintf(fp, "  if (argc < %d) return -1;\n\n",
                    learner->no_of_input_fields);
        fprintf(fp, "%s",
                "  /* Obtain input values from command arguments */\n");
        fprintf(fp, "%s", "  for (i = 1; i < argc; i++) {\n");
        fprintf(fp, "%s", "    if (i > no_of_inputs) return -2;\n");
        fprintf(fp, "%s", "    inputs[i-1] = atof(argv[i]);\n");
        fprintf(fp, "%s", "  }\n\n");
    }
    else {
        fprintf(fp, "%s", "  /* Change the read pin numbers as needed */\n");
        COUNTUP(i, learner->net->no_of_inputs)
            fprintf(fp, "  inputs[%d] = analogRead(%d);\n", i, i);
        fprintf(fp, "%s", "\n");
    }

    if (learner->no_of_input_fields == 0) {
        fprintf(fp,
                "  /* Normalise inputs into a %.2f - %.2f range */\n",
                NEURON_LOW, NEURON_HIGH);
        fprintf(fp, "%s", "  for (i = 0; i < no_of_inputs; i++) {\n");
        fprintf(fp, "    network_inputs[i] = %.2f + ((inputs[i] - " \
                "input_range_min[i])*%.2f/(input_range_max[i] - " \
                "input_range_min[i]));\n",
                NEURON_LOW, NEURON_RANGE);
        fprintf(fp,
                "    if (network_inputs[i] < %.2f) " \
                "network_inputs[i] = %.2f;\n",
                NEURON_LOW, NEURON_LOW);
        fprintf(fp,
                "    if (network_inputs[i] > %.2f) " \
                "network_inputs[i] = %.2f;\n",
                NEURON_HIGH, NEURON_HIGH);
        fprintf(fp, "%s", "  }\n\n");
    }
    else {
        fprintf(fp, "%s", "  pos = 0;\n");
        fprintf(fp, "%s", "  for (i = 0; i < no_of_input_fields; i++) {\n");
        fprintf(fp, "%s", "    if (field_length[i] == 0) {\n");
        fprintf(fp,
                "      /* Normalise numeric inputs into a " \
                "%.2f - %.2f range */\n",
                NEURON_LOW, NEURON_HIGH);
        fprintf(fp, "      network_inputs[pos] = %.2f + " \
                "((inputs[i] - input_range_min[i])*%.2f/" \
                "(input_range_max[i] - input_range_min[i]));\n",
                NEURON_LOW, NEURON_RANGE);
        fprintf(fp,
                "      if (network_inputs[pos] < %.2f) " \
                "network_inputs[pos] = %.2f;\n",
                NEURON_LOW, NEURON_LOW);
        fprintf(fp, "      if (network_inputs[pos] > %.2f) " \
                "network_inputs[pos] = %.2f;\n",
                NEURON_HIGH, NEURON_HIGH);
        fprintf(fp, "%s", "      pos++;\n");
        fprintf(fp, "%s", "    }\n");
        fprintf(fp, "%s", "    else {\n");
        fprintf(fp, "%s", "      /* text value */\n");
        if (export_type == EXPORT_C99)
            fprintf(fp, "%s", "      encode_text(argv[i+1], " \
                    "network_inputs, no_of_inputs,\n");
        else
            fprintf(fp, "%s", "      encode_text(\"\", " \
                    "network_inputs, no_of_inputs,\n");

        fprintf(fp,       "                  pos, field_length[i]/%d);\n",
                (int)CHAR_BITS);
        fprintf(fp, "%s", "      pos += field_length[i];\n");
        fprintf(fp, "%s", "    }\n");
        fprintf(fp, "%s", "  }\n\n");
    }

    fprintf(fp, "%s", "  /* Hidden layer 0 */\n");
    fprintf(fp, "%s", "  for (i = 0; i < no_of_hiddens; i++) {\n");
    fprintf(fp, "%s", "    sum = hidden_layer_0_bias[i];\n");
    fprintf(fp, "%s", "    for (j = 0; j < no_of_inputs; j++) {\n");
    fprintf(fp, "%s", "      sum += hidden_layer_0_weights[i*no_of_inputs+j]*" \
            "network_inputs[j];\n");
    fprintf(fp, "%s", "    }\n");
    fprintf(fp, "%s", "    hiddens[i] = AF(sum);\n");
    fprintf(fp, "%s", "  }\n");
    fprintf(fp, "%s", "  for (i = 0; i < no_of_hiddens; i++) {\n");
    fprintf(fp, "%s", "    prev_hiddens[i] = hiddens[i];\n");
    fprintf(fp, "%s", "  }\n\n");
    for (int i = 1; i < learner->net->hidden_layers; i++) {
        fprintf(fp, "  /* Hidden layer %d */\n", i);
        fprintf(fp, "  for (i = 0; i < %d; i++) {\n",
                HIDDENS_IN_LAYER(learner->net,i));
        fprintf(fp, "    sum = hidden_layer_%d_bias[i];\n",i);
        fprintf(fp, "    for (j = 0; j < %d; j++) {\n",
                HIDDENS_IN_LAYER(learner->net,i-1));
        fprintf(fp, "      sum += " \
                "hidden_layer_%d_weights[i*%d+j]*prev_hiddens[j];\n",
                i,HIDDENS_IN_LAYER(learner->net,i-1));
        fprintf(fp, "%s", "    }\n");
        fprintf(fp, "%s", "    hiddens[i] = AF(sum);\n");
        fprintf(fp, "%s", "  }\n");
        fprintf(fp, "  for (i = 0; i < %d; i++) {\n",
                HIDDENS_IN_LAYER(learner->net,i));
        fprintf(fp, "%s", "    prev_hiddens[i] = hiddens[i];\n");
        fprintf(fp, "%s", "  }\n\n");
    }
    fprintf(fp, "%s", "  /* Output layer */\n");
    fprintf(fp, "%s", "  for (i = 0; i < no_of_outputs; i++) {\n");
    fprintf(fp, "%s", "    sum = output_layer_bias[i];\n");
    fprintf(fp, "    for (j = 0; j < %d; j++) {\n",
            HIDDENS_IN_LAYER(learner->net,learner->net->hidden_layers-1));
    fprintf(fp, "      sum += output_layer_weights[i*%d+j]*prev_hiddens[j];\n",
            HIDDENS_IN_LAYER(learner->net,learner->net->hidden_layers-1));
    fprintf(fp, "%s", "    }\n");
    fprintf(fp, "%s", "    outputs[i] = AF(sum);\n");
    fprintf(fp, "%s", "  }\n\n");

    fprintf(fp, "%s", "  for (i = 0; i < no_of_outputs; i++) {\n");
    fprintf(fp, "    /* Convert outputs from %.2f - %.2f " \
            "back to their original range */\n",
            NEURON_LOW, NEURON_HIGH);
    fprintf(fp, "    outputs[i] = output_range_min[i] + " \
            "((outputs[i]-%.2f)*(output_range_max[i] - " \
            "output_range_min[i])/%.2f);\n",
            NEURON_LOW, NEURON_RANGE);

    if (export_type == EXPORT_C99) {
        fprintf(fp, "%s", "    /* Send the outputs to stdout */\n");
        fprintf(fp, "%s", "    printf(\"%.10f\",outputs[i]);\n");
        fprintf(fp, "%s", "    if (i < no_of_outputs-1) {\n");
        fprintf(fp, "%s", "      printf(\" \");\n");
        fprintf(fp, "%s", "    }\n");
        fprintf(fp, "%s", "  }\n\n");
        fprintf(fp, "%s", "  printf(\"\\n\");");
        fprintf(fp, "%s", "\n");
        fprintf(fp, "%s", "  return 0;\n");
    }
    else {
        fprintf(fp, "%s", "    /* Do something with the outputs here */\n");
        fprintf(fp, "%s", "    Serial.print(outputs[i]);\n");
        fprintf(fp, "%s", "    if (i < no_of_outputs-1) {\n");
        fprintf(fp, "%s", "      Serial.print(\" \");\n");
        fprintf(fp, "%s", "    }\n");
        fprintf(fp, "%s", "  }\n\n");
        fprintf(fp, "%s", "  Serial.println(\"\");");
        fprintf(fp, "%s", "\n");
    }

    fprintf(fp, "%s", "}\n");
    fclose(fp);
    return 0;
}

/**
 * @brief Exports a trained network as a standalone C program
 * @param learner Deep learner object
 * @param filename The C source file to be produced
 * @returns zero on success
 */
static int deeplearn_export_c(deeplearn * learner, char * filename)
{
    return deeplearn_export_c_base(learner, EXPORT_C99, filename);
}

/**
 * @brief Exports a trained network as a standalone Arduino C program
 * @param learner Deep learner object
 * @param filename The Arduino C source file to be produced
 * @returns zero on success
 */
static int deeplearn_export_arduino(deeplearn * learner, char * filename)
{
    return deeplearn_export_c_base(learner, EXPORT_ARDUINO, filename);
}

/**
 * @brief Exports a trained network as a standalone python class
 * @param learner Deep learner object
 * @param filename The python source file to be produced
 * @returns zero on success
 */
static int deeplearn_export_python(deeplearn * learner, char * filename)
{
    FILE * fp;
    int no_of_weights;

    fp = fopen(filename,"w");
    if (!fp)
        return -1;

    fprintf(fp, "%s", "#!/usr/bin/python3\n\n");
    fprintf(fp, "%s", "import sys\n");
    fprintf(fp, "%s", "import math\n\n");

    fprintf(fp,"%s\n\n","class NeuralNet:");

    if (learner->no_of_input_fields > 0)
        fprintf(fp, "  no_of_input_fields = %d\n", learner->no_of_input_fields);

    fprintf(fp, "  no_of_inputs = %d\n",
            learner->net->no_of_inputs);
    fprintf(fp, "  no_of_hiddens = %d\n",
            learner->net->no_of_hiddens);
    fprintf(fp, "  no_of_outputs = %d\n",
            learner->net->no_of_outputs);
    fprintf(fp, "  hidden_layers = %d\n\n",
            learner->net->hidden_layers);

    /* field lengths */
    if (learner->field_length != 0) {
        if (learner->no_of_input_fields > 0) {
            fprintf(fp, "%s", "  field_length = [");
            COUNTUP(i, learner->no_of_input_fields) {
                fprintf(fp, "%d", learner->field_length[i]);
                if (i < learner->no_of_input_fields-1)
                    fprintf(fp, ",");
            }
            fprintf(fp, "%s", "]\n\n");
        }
    }

    /* ranges */
    fprintf(fp, "%s", "  input_range_min = [");
    COUNTUP(i, learner->net->no_of_inputs) {
        fprintf(fp, "%.10f", learner->input_range_min[i]);
        if (i < learner->net->no_of_inputs-1)
            fprintf(fp, ",");
    }
    fprintf(fp, "%s", "]\n\n");
    fprintf(fp, "%s", "  input_range_max = [");
    COUNTUP(i, learner->net->no_of_inputs) {
        fprintf(fp, "%.10f", learner->input_range_max[i]);
        if (i < learner->net->no_of_inputs-1)
            fprintf(fp, ",");
    }
    fprintf(fp, "%s", "]\n\n");
    fprintf(fp, "%s", "  output_range_min = [");
    COUNTUP(i, learner->net->no_of_outputs) {
        fprintf(fp, "%.10f", learner->output_range_min[i]);
        if (i < learner->net->no_of_outputs-1)
            fprintf(fp, ",");
    }
    fprintf(fp, "%s", "]\n\n");
    fprintf(fp, "%s", "  output_range_max = [");
    COUNTUP(i, learner->net->no_of_outputs) {
        fprintf(fp, "%.10f", learner->output_range_max[i]);
        if (i < learner->net->no_of_outputs-1)
            fprintf(fp, ",");
    }
    fprintf(fp, "]\n\n");

    /* hidden unit weights */
    COUNTUP(i, learner->net->hidden_layers) {
        fprintf(fp,
                "  hidden_layer_%d_weights = [", i);

        if (i == 0)
            no_of_weights = learner->net->no_of_inputs;
        else
            no_of_weights = HIDDENS_IN_LAYER(learner->net, i-1);

        COUNTUP(j, HIDDENS_IN_LAYER(learner->net, i)) {
            COUNTUP(k, no_of_weights) {
                float w = learner->net->hiddens[i][j]->weights[k];
                if (w != 0)
                    fprintf(fp, "%.10f", w);
                else
                    fprintf(fp, "%d", 0);
                if (!((j == HIDDENS_IN_LAYER(learner->net, i)-1) &&
                      (k == no_of_weights-1)))
                    fprintf(fp, ",");
            }
        }
        fprintf(fp, "]\n\n");
    }

    /* hidden unit biases */
    COUNTUP(i, learner->net->hidden_layers) {
        fprintf(fp,
                "  hidden_layer_%d_bias = [", i);
        COUNTUP(j, HIDDENS_IN_LAYER(learner->net, i)) {
            fprintf(fp,"%.10f",learner->net->hiddens[i][j]->bias);
            if (j < HIDDENS_IN_LAYER(learner->net, i)-1)
                fprintf(fp, ",");
        }
        fprintf(fp, "]\n\n");
    }

    /* output unit weights */
    fprintf(fp,
            "  output_layer_weights = [");
    COUNTUP(i, learner->net->no_of_outputs) {
        COUNTUP(j, HIDDENS_IN_LAYER(learner->net,
                                       learner->net->hidden_layers-1)) {
            float w = learner->net->outputs[i]->weights[j];
            if (w != 0)
                fprintf(fp, "%.10f", w);
            else
                fprintf(fp, "%d", 0);
            if (!((i == learner->net->no_of_outputs-1) &&
                  (j == HIDDENS_IN_LAYER(learner->net,
                                            learner->net->hidden_layers-1)-1)))
                fprintf(fp, ",");
        }
    }
    fprintf(fp, "]\n\n");

    /* output unit biases */
    fprintf(fp, "%s",
            "  output_layer_bias = [");
    COUNTUP(i, learner->net->no_of_outputs) {
        fprintf(fp, "%.10f",
                learner->net->outputs[i]->bias);
        if (i < learner->net->no_of_outputs-1)
            fprintf(fp, ",");
    }
    fprintf(fp, "%s", "]\n\n\n");

    fprintf(fp, "%s", "  # Activation function\n");

#if ACTIVATION_FUNCTION == AF_SIGMOID
    fprintf(fp, "%s", "  def af(this, adder):\n");
    fprintf(fp, "%s", "      return 1.0 / (1.0 + math.exp(-adder))\n\n");
#elif ACTIVATION_FUNCTION == AF_TANH
    fprintf(fp, "%s", "  def af(this, adder):\n");
    fprintf(fp, "%s", "      return ((((2.0 / (1.0 + math.exp(-(2*adder)))) " \
            "- 1.0)*0.5)+0.5)\n\n");
#elif ACTIVATION_FUNCTION == AF_LINEAR
    fprintf(fp, "%s", "  def af(this, adder):\n");
    fprintf(fp, "%s", "      if adder < 1.0:\n");
    fprintf(fp, "%s", "          if adder > -1.0:\n");
    fprintf(fp, "%s", "              return (adder*0.5) + 0.5\n");
    fprintf(fp, "%s", "          else:\n");
    fprintf(fp, "%s", "              return 0.0\n");
    fprintf(fp, "%s", "      else:\n");
    fprintf(fp, "%s", "          return 1.0\n");
#endif

    if (learner->no_of_input_fields > 0) {
        fprintf(fp, "%s", "  # Encode some text into the input units\n");
        fprintf(fp, "%s", "  def encode_text(this, text,inputs,no_of_inputs,");
        fprintf(fp, "%s", "  offset, max_field_length_chars):\n");

        fprintf(fp, "%s", "    pos = offset\n");
        fprintf(fp, "%s", "    max_chars = len(text)\n\n");

        fprintf(fp, "    if max_chars > (no_of_inputs-offset)/%d:\n",
                (int)CHAR_BITS);
        fprintf(fp, "      max_chars = ((no_of_inputs-offset)/%d)\n\n",
                (int)CHAR_BITS);

        fprintf(fp, "%s", "    if max_chars > max_field_length_chars:\n");
        fprintf(fp, "%s", "      max_chars = max_field_length_chars\n\n");

        fprintf(fp, "%s", "    # for each character in the string\n");
        fprintf(fp, "%s", "    for i in range(max_chars):\n");
        fprintf(fp, "%s", "      # set the bits for this character\n");
        fprintf(fp,       "      for bit in range(%d):\n", (int)CHAR_BITS);
        fprintf(fp, "%s", "        if ord(text[i]) & (1 << bit):\n");
        fprintf(fp,       "          inputs.append(%.2f)\n", NEURON_HIGH);
        fprintf(fp, "%s", "        else:\n");
        fprintf(fp,       "          inputs.append(%.2f)\n", NEURON_LOW);
        fprintf(fp, "%s", "        pos = pos + 1\n\n");

        fprintf(fp, "%s",
                "    # set the remaining inputs within the field " \
                "to neutral\n");
        fprintf(fp, "%s", "    i = max_chars\n");
        fprintf(fp, "%s", "    while i < max_field_length_chars:\n");
        fprintf(fp,       "      for bit in range (%d):\n", (int)CHAR_BITS);
        fprintf(fp, "%s", "        if pos >= no_of_inputs:\n");
        fprintf(fp, "%s", "          i = max_field_length_chars\n");
        fprintf(fp, "%s", "          break\n");
        fprintf(fp,       "        inputs.append(%.1f)\n", NEURON_UNKNOWN);
        fprintf(fp, "%s", "        pos = pos + 1\n");
        fprintf(fp, "%s", "      i = i + 1\n\n");
    }

    fprintf(fp, "%s", "  def update(this, inputs):\n");

    if (learner->no_of_input_fields > 0)
        fprintf(fp, "%s", "    pos = 0\n");

    fprintf(fp, "%s", "    network_inputs = []\n");
    fprintf(fp, "%s", "    prev_hiddens = []\n");
    fprintf(fp, "%s", "    hiddens = []\n");
    fprintf(fp, "%s", "    outputs = []\n\n");

    if (learner->no_of_input_fields == 0)
        fprintf(fp, "    if len(inputs) < %d:\n", learner->net->no_of_inputs);
    else
        fprintf(fp, "    if len(inputs) < %d:\n", learner->no_of_input_fields);

    fprintf(fp, "%s", "        return []\n\n");

    if (learner->no_of_input_fields == 0) {
        fprintf(fp, "    # Normalise inputs into a %.2f - %.2f range\n",
                NEURON_LOW, NEURON_HIGH);
        fprintf(fp, "%s", "    for i in range (this.no_of_inputs):\n");
        fprintf(fp,
                "      network_inputs.append(%.2f + ((float(inputs[i]) - " \
                "this.input_range_min[i])*%.2f/" \
                "(this.input_range_max[i] - this.input_range_min[i])))\n",
                NEURON_LOW, NEURON_RANGE);
        fprintf(fp,       "      if network_inputs[i] < %.2f:\n", NEURON_LOW);
        fprintf(fp,       "        network_inputs[i] = %.2f\n", NEURON_LOW);
        fprintf(fp,       "      if network_inputs[i] > %.2f:\n", NEURON_HIGH);
        fprintf(fp,       "        network_inputs[i] = %.2f\n", NEURON_HIGH);
    }
    else {
        fprintf(fp, "%s", "    for i in range(this.no_of_input_fields):\n");
        fprintf(fp, "%s", "      if this.field_length[i] == 0:\n");
        fprintf(fp,
                "        # Normalise numeric inputs into a " \
                "%.2f - %.2f range\n",
                NEURON_LOW, NEURON_HIGH);
        fprintf(fp,       "        network_inputs.append(%.2f + " \
                "((float(inputs[i]) - this.input_range_min[i])*%.2f/" \
                "(this.input_range_max[i] - this.input_range_min[i])))\n",
                NEURON_LOW, NEURON_RANGE);
        fprintf(fp,       "        if network_inputs[pos] < %.2f:\n", NEURON_LOW);
        fprintf(fp,       "          network_inputs[pos] = %.2f\n", NEURON_LOW);
        fprintf(fp,       "        if network_inputs[pos] > %.2f:\n", NEURON_HIGH);
        fprintf(fp,       "          network_inputs[pos] = %.2f\n\n", NEURON_HIGH);
        fprintf(fp, "%s", "        pos = pos + 1\n");
        fprintf(fp, "%s", "      else:\n");
        fprintf(fp, "%s", "        # text value\n");
        fprintf(fp, "%s", "        this.encode_text(inputs[i], " \
                "network_inputs, this.no_of_inputs,");
        fprintf(fp,       "pos, this.field_length[i]/%d)\n", (int)CHAR_BITS);
        fprintf(fp, "%s", "        pos = pos + this.field_length[i]\n\n");
    }

    fprintf(fp, "%s", "    # Hidden layer 0\n");
    fprintf(fp, "%s", "    for i in range(this.no_of_hiddens):\n");
    fprintf(fp, "%s", "      adder = this.hidden_layer_0_bias[i]\n");
    fprintf(fp, "%s", "      for j in range(this.no_of_inputs):\n");
    fprintf(fp, "%s", "        adder = adder + " \
            "this.hidden_layer_0_weights[i*this.no_of_inputs+j]*" \
            "network_inputs[j]\n");
    fprintf(fp, "%s", "      hiddens.append(this.af(adder))\n");
    fprintf(fp, "%s", "    for i in range(this.no_of_hiddens):\n");
    fprintf(fp, "%s", "      prev_hiddens.append(hiddens[i])\n\n");
    for (int i = 1; i < learner->net->hidden_layers; i++) {
        fprintf(fp, "    # Hidden layer %d\n", i);
        fprintf(fp, "    for i in range(%d):\n",
                HIDDENS_IN_LAYER(learner->net,i));
        fprintf(fp, "      adder = this.hidden_layer_%d_bias[i]\n",i);
        fprintf(fp, "      for j in range(%d):\n",
                HIDDENS_IN_LAYER(learner->net,i-1));
        fprintf(fp, "        adder = adder + " \
                "this.hidden_layer_%d_weights[i*%d+j]*prev_hiddens[j]\n",
                i,HIDDENS_IN_LAYER(learner->net,i-1));
        fprintf(fp, "%s", "      hiddens[i] = this.af(adder)\n");
        fprintf(fp, "    for i in range(%d):\n",
                HIDDENS_IN_LAYER(learner->net,i));
        fprintf(fp, "%s", "      prev_hiddens[i] = hiddens[i]\n\n");
    }
    fprintf(fp, "%s", "    # Output layer\n");
    fprintf(fp, "%s", "    for i in range(this.no_of_outputs):\n");
    fprintf(fp, "%s", "      adder = this.output_layer_bias[i]\n");
    fprintf(fp, "      for j in range(%d):\n",
            HIDDENS_IN_LAYER(learner->net,learner->net->hidden_layers-1));
    fprintf(fp, "        adder = adder + " \
            "this.output_layer_weights[i*%d+j]*prev_hiddens[j]\n",
            HIDDENS_IN_LAYER(learner->net,learner->net->hidden_layers-1));
    fprintf(fp, "%s", "      outputs.append(this.af(adder))\n\n");
    fprintf(fp,
            "    # Convert outputs from %.2f - %.2f " \
            "back to their original range\n",
            NEURON_LOW, NEURON_HIGH);
    fprintf(fp, "%s", "    for i in range(this.no_of_outputs):\n");
    fprintf(fp,       "      outputs[i] = this.output_range_min[i] + " \
            "((outputs[i]-%.2f)*(this.output_range_max[i] - " \
            "this.output_range_min[i])/%.2f)\n\n",
            NEURON_LOW, NEURON_RANGE);

    fprintf(fp, "%s", "    # Return the output unit values as a list\n");
    fprintf(fp, "%s", "    return outputs\n\n\n");

    fprintf(fp, "%s", "# Create an instance of the class\n");
    fprintf(fp, "%s", "net = NeuralNet()\n\n");
    fprintf(fp, "%s", "# Use the commandline arguments as input values\n");
    fprintf(fp, "%s", "print(net.update(sys.argv[1:]))\n");
    fclose(fp);
    return 0;
}

/**
 * @brief Exports a trained network as a standalone program
 *        file types supported are .c and .py
 * @param learner Deep learner object
 * @param filename The source file to be produced
 * @returns zero on success
 */
int deeplearn_export(deeplearn * learner, char * filename)
{
    int length = strlen(filename);

    if (length > 3)
        if (string_ends_with_extension(filename, "py"))
            return deeplearn_export_python(learner, filename);

    if ((strstr(filename,"sketch") != NULL) ||
        (strstr(filename,"arduino") != NULL))
        return deeplearn_export_arduino(learner, filename);

    return deeplearn_export_c(learner, filename);
}

/**
 * @brief Sets small weights to zero
 * @param learner deeplearn object
 * @param threshold Pruning threshold in the range 0.0 -> 1.0
 * @returns The percent of weights pruned
 */
int deeplearn_prune_weights(deeplearn * learner, float threshold)
{
    return bp_prune_weights(learner->net, threshold);
}

/*
 * @brief Sets the pruning parameters used during training
 * @param learner deeplearn object
 * @param cycle The number of itterations after which pruning happens
 * @param rate Pruning rate in the range 0.0 -> 1.0
 */
void deeplearn_set_pruning(deeplearn * learner, unsigned int cycle, float rate)
{
    learner->net->pruning_cycle = cycle;
    learner->net->pruning_rate = rate;
}
