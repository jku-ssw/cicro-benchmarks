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

#include "deeplearndata.h"

/**
* @brief Adds a training or test sample to the data set
* @param datalist The list to be added to
* @param data_samples The number of data samples in the list
* @param inputs Input data
* @param outputs Output data
* @param no_of_input_fields The number of input fields
* @param no_of_outputs The number of output fields
* @param input_range_min Minimum value for each input field
* @param input_range_max Maximum value for each input field
* @param output_range_min Minimum value for each output field
* @param output_range_max Maximum value for each output field
* @returns 0 on success
*/
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
                      float output_range_max[])
{
    deeplearndata * data;
    data = (deeplearndata*)malloc(sizeof(deeplearndata));
    if (!data)
        return -1;

    /* create arrays to store the data */
    FLOATALLOC(data->inputs, no_of_input_fields);
    if (!data->inputs) {
        free(data);
        return -2;
    }

    data->inputs_text = 0;
    if (inputs_text != 0) {
        CHARPTRALLOC(data->inputs_text, no_of_input_fields);
        if (!data->inputs_text) {
            free(data->inputs);
            free(data);
            return -3;
        }

        COUNTUP(i, no_of_input_fields) {
            data->inputs_text[i] = 0;
            if (inputs_text[i] != 0) {
                /* copy the string */
                CHARALLOC(data->inputs_text[i], strlen(inputs_text[i])+1);
                if (!data->inputs_text[i]) {
                    COUNTDOWN(j, i) {
                        free(data->inputs_text[j]);
                    }
                    free(data->inputs_text);
                    free(data->inputs);
                    free(data);
                    return -4;
                }
                strcpy(data->inputs_text[i], inputs_text[i]);
            }
        }
    }
    FLOATALLOC(data->outputs, no_of_outputs);
    if (!data->outputs) {
        if (inputs_text != 0) {
            COUNTDOWN(i, no_of_input_fields) {
                free(data->inputs_text[i]);
            }
            free(data->inputs_text);
        }
        free(data->inputs);
        free(data);
        return -5;
    }

    /* copy the data */
    memcpy((void*)data->inputs, inputs, no_of_input_fields*sizeof(float));
    memcpy((void*)data->outputs, outputs, no_of_outputs*sizeof(float));

    /* update the data range */
    COUNTUP(i, no_of_input_fields) {
        if (inputs[i] < input_range_min[i])
            input_range_min[i] = inputs[i];

        if (inputs[i] > input_range_max[i])
            input_range_max[i] = inputs[i];
    }

    data->labeled = 1;
    COUNTUP(i, no_of_outputs) {
        if ((int)outputs[i] != DEEPLEARN_UNKNOWN_VALUE) {
            if (outputs[i] < output_range_min[i])
                output_range_min[i] = outputs[i];

            if (outputs[i] > output_range_max[i])
                output_range_max[i] = outputs[i];
        }
        else
            data->labeled = 0;
    }

    data->flags = 0;

    /* change the current head of the list */
    data->prev = 0;
    data->next = 0;
    if (*datalist) {
        (*datalist)->prev = (struct deeplearndata *)data;
        data->next = (struct deeplearndata *)(*datalist);
    }
    *datalist = data;
    *data_samples = *data_samples + 1;

    return 0;
}

/**
* @brief Indexes data for fast read access
* @param meta Pointer to the data list
* @param samples The size of the data list
* @param indexed_list Pointer to the indexed data list
* @param indexed_samples Pointer to the size of the indexed data list
* @returns 0 if data indexed successfully,
*          1 if samples does not match list size
*/
int deeplearndata_index_data(
        deeplearndata * list,
        int samples,
        deeplearndata *** indexed_list,
        int* indexed_samples)
{
    *indexed_list = (deeplearndata**)malloc(samples*sizeof(deeplearndata*));
    deeplearndata* data_head = list;
    *indexed_samples = 0;
    for(int i = 0; i < samples && data_head != 0; ++i) {
        (*indexed_list)[i] = data_head;
        data_head = data_head->next;
        (*indexed_samples)++;
    }
    return *indexed_samples == samples ? 0 : 1;
}

/**
* @brief Indexes metadata for fast read access
* @param meta Pointer to the metadata list
* @param samples The size of the metadata list
* @param indexed_list Pointer to the indexed metadata list
* @param indexed_samples Pointer to the size of the indexed metadata list
* @returns 0 if data indexed successfully,
*          1 if samples does not match list size
*/
int deeplearndata_index_meta(
        deeplearndata_meta * list,
        int samples,
        deeplearndata_meta *** indexed_list,
        int* indexed_samples)
{
    *indexed_list =
        (deeplearndata_meta**)malloc(samples*sizeof(deeplearndata_meta*));
    deeplearndata_meta* data_head = list;
    *indexed_samples = 0;
    for(int i = 0; i < samples && data_head != 0; ++i) {
        (*indexed_list)[i] = data_head;
        data_head = data_head->next;
        (*indexed_samples)++;
    }
    return *indexed_samples == samples ? 0 : 1;
}

/**
* @brief Returns a training data sample
* @param learner Deep learner object
* @returns deeplearndata object
*/
deeplearndata * deeplearndata_get_training(deeplearn * learner, int index)
{
    if ((index < 0) || (index >= learner->indexed_training_data_samples))
        return 0;

    deeplearndata_meta * meta = learner->indexed_training_data[index];
    if (meta == 0)
        return 0;

    return meta->sample;
}

/**
* @brief Returns a labeled training data sample
* @param learner Deep learner object
* @returns deeplearndata object
*/
deeplearndata * deeplearndata_get_training_labeled(deeplearn * learner,
                                                   int index)
{
    if ((index < 0) ||
        (index >= learner->indexed_training_data_labeled_samples))
        return 0;

    deeplearndata_meta * meta = learner->indexed_training_data_labeled[index];
    if (meta == 0)
        return 0;

    return meta->sample;
}

/**
* @brief Returns a test data sample
* @param learner Deep learner object
* @returns deeplearndata object
*/
deeplearndata * deeplearndata_get_test(deeplearn * learner, int index)
{
    if ((index < 0) || (index >= learner->indexed_test_data_samples))
        return 0;

    deeplearndata_meta * meta = learner->indexed_test_data[index];
    if (meta == 0)
        return 0;

    return meta->sample;
}

/**
* @brief Returns a data sample
* @param learner Deep learner object
* @returns deeplearndata object
*/
deeplearndata * deeplearndata_get(deeplearn * learner, int index)
{
    /* range checking of the index */
    if ((index < 0) || (index >= learner->indexed_data_samples))
      return 0;

    deeplearndata * ptr = learner->indexed_data[index];
    return ptr;
}

/**
* @brief Clears the flags on every data sample
* @param learner Deep learner object
*/
static void deeplearndata_clear_flags(deeplearn * learner)
{
    deeplearndata * ptr = learner->data;
    while (ptr != 0) {
        ptr->flags = 0;
        ptr = (deeplearndata*)ptr->next;
    }
}

/**
* @brief Clears existing training and test data sets
* @param learner Deep learner object
*/
static void deeplearndata_free_datasets(deeplearn * learner)
{
    /* free training samples */
    deeplearndata_meta * training_sample = learner->training_data;
    deeplearndata_meta * prev_training_sample;
    while (training_sample != 0) {
        prev_training_sample = training_sample;
        training_sample = (deeplearndata_meta *)training_sample->next;
        free(prev_training_sample);
    }
    learner->training_data = 0;
    learner->training_data_samples = 0;
    free(learner->indexed_training_data);
    learner->indexed_training_data = 0;
    learner->indexed_training_data_samples = 0;

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
    learner->training_data_labeled = 0;
    learner->training_data_labeled_samples = 0;
    free(learner->indexed_training_data_labeled);
    learner->indexed_training_data_labeled = 0;
    learner->indexed_training_data_labeled_samples = 0;

    /* free test samples */
    deeplearndata_meta * test_sample = learner->test_data;
    deeplearndata_meta * prev_test_sample;
    while (test_sample != 0) {
        prev_test_sample = test_sample;
        test_sample = (deeplearndata_meta *)test_sample->next;
        free(prev_test_sample);
    }
    learner->test_data = 0;
    learner->test_data_samples = 0;
    free(learner->indexed_test_data);
    learner->indexed_test_data = 0;
    learner->indexed_test_data_samples = 0;
}

/**
* @brief Adds a sample to the training set
* @param learner Deep learner object
* @param sample The data sample to be added
* @returns zero on success
*/
int deeplearndata_add_training_sample(deeplearn * learner,
                                      deeplearndata * sample)
{
    deeplearndata_meta * data;

    if (sample == 0)
        return -1;

    data = (deeplearndata_meta*)malloc(sizeof(deeplearndata_meta));
    if (!data)
        return -2;

    data->sample = sample;
    data->prev = 0;
    data->next = 0;
    if (learner->training_data) {
        learner->training_data->prev = (struct deeplearndata_meta *)data;
        data->next = (struct deeplearndata_meta *)learner->training_data;
    }
    learner->training_data = data;
    learner->training_data_samples++;
    return 0;
}

/**
* @brief Adds a labeled sample to the training set
* @param learner Deep learner object
* @param sample The data sample to be added
* @returns zero on success
*/
int deeplearndata_add_labeled_training_sample(deeplearn * learner,
                                              deeplearndata * sample)
{
    deeplearndata_meta * data;

    if (sample == 0)
        return -1;

    data = (deeplearndata_meta*)malloc(sizeof(deeplearndata_meta));
    if (!data)
        return -2;

    data->sample = sample;
    data->prev = 0;
    data->next = 0;

    if (learner->training_data_labeled) {
        learner->training_data_labeled->prev =
            (struct deeplearndata_meta *)data;
        data->next =
            (struct deeplearndata_meta *)learner->training_data_labeled;
    }

    learner->training_data_labeled = data;
    learner->training_data_labeled_samples++;
    return 0;
}

/**
* @brief Adds a sample to the test set
* @param learner Deep learner object
* @param sample The data sample to be added
* @returns zero on success
*/
int deeplearndata_add_test_sample(deeplearn * learner, deeplearndata * sample)
{
    deeplearndata_meta * data;

    if (sample == 0)
        return -1;

    data = (deeplearndata_meta*)malloc(sizeof(deeplearndata_meta));
    if (!data)
        return -2;

    data->sample = sample;
    data->prev = 0;
    data->next = 0;

    if (learner->test_data) {
        learner->test_data->prev = (struct deeplearndata_meta *)data;
        data->next = (struct deeplearndata_meta *)learner->test_data;
    }

    learner->test_data = data;
    learner->test_data_samples++;
    return 0;
}

/**
* @brief Creates training and test sets from the data samples
* @param learner Deep learner object
* @param test_data_percentage The percentage of samples to be used for testing
* @returns zero on success
*/
int deeplearndata_create_datasets(deeplearn * learner,
                                  int test_data_percentage)
{
    int training_samples =
        learner->data_samples * (100-test_data_percentage) / 100;
    int index, retval;
    deeplearndata * sample;

    if (learner->data == 0)
        return -1;

    deeplearndata_clear_flags(learner);
    deeplearndata_free_datasets(learner);

    /* create training samples */
    while (learner->training_data_samples < training_samples) {
        index = rand_num(&learner->net->random_seed)%learner->data_samples;
        sample = deeplearndata_get(learner, index);

        if (!sample)
            return -1;

        if (sample->flags == 0) {
            sample->flags = 1;
            retval =
              deeplearndata_add_training_sample(learner, sample);

            if (retval != 0)
                return -200 + retval;

            if (sample->labeled != 0) {
                retval =
                    deeplearndata_add_labeled_training_sample(learner, sample);
                if (retval != 0)
                    return -300 + retval;
            }
        }
    }

    /* create the indexed array for fast access */
    deeplearndata_index_meta(learner->training_data,
                             learner->training_data_samples,
                             &learner->indexed_training_data,
                             &learner->indexed_training_data_samples);
    deeplearndata_index_meta(learner->training_data_labeled,
                             learner->training_data_labeled_samples,
                             &learner->indexed_training_data_labeled,
                             &learner->indexed_training_data_labeled_samples);

    /* create test samples */
    sample = learner->data;
    while (sample != 0) {
        if (sample->flags == 0) {
            if (sample->labeled != 0) {
                retval =
                    deeplearndata_add_test_sample(learner, sample);
                if (retval != 0)
                    return -400 + retval;
            }
            else {
                retval =
                    deeplearndata_add_training_sample(learner, sample);
                if (retval != 0)
                    return -500 + retval;
            }
        }
        sample = (deeplearndata*)sample->next;
    }

    /* create the indexed array for fast access */
    deeplearndata_index_meta(learner->test_data, learner->test_data_samples,
                             &learner->indexed_test_data,
                             &learner->indexed_test_data_samples);

    return 0;
}

/**
* @brief Loads a data set from a csv file and creates a deep learner
* @param filename csv filename
* @param learner Deep learner object
* @param no_of_hiddens The number of hidden units per layer
* @param hidden layers The number of hidden layers
* @param no_of_outputs The number of outputs
* @param output_field_index Field numbers for the outputs within the csv file
* @param output_classes The number of output classes if the output in the
*        data set is a single integer value
* @param error_threshold Training error thresholds for each hidden layer
* @param random_seed Random number seed
* @returns The number of data samples loaded
*/
int deeplearndata_read_csv(char * filename,
                           deeplearn * learner,
                           int no_of_hiddens, int hidden_layers,
                           int no_of_outputs, int * output_field_index,
                           int output_classes,
                           float error_threshold[],
                           unsigned int * random_seed)
{
    int field_number, input_index, ctr, samples_loaded = 0;
    FILE * fp;
    char line[2000],valuestr[DEEPLEARN_MAX_FIELD_LENGTH_CHARS],*retval;
    float value;
    int data_set_index = 0;
    int no_of_inputs = 0;
    int no_of_input_fields = 0;
    float inputs[DEEPLEARN_MAX_CSV_INPUTS];
    char* inputs_text[DEEPLEARN_MAX_CSV_INPUTS];
    float outputs[DEEPLEARN_MAX_CSV_OUTPUTS];
    int fields_per_example = 0;
    int network_outputs = no_of_outputs;
    int is_text, output_ctr;
    deeplearndata * data = 0;
    int data_samples = 0;
    float input_range_min[DEEPLEARN_MAX_CSV_INPUTS];
    float input_range_max[DEEPLEARN_MAX_CSV_INPUTS];
    float output_range_min[DEEPLEARN_MAX_CSV_OUTPUTS];
    float output_range_max[DEEPLEARN_MAX_CSV_OUTPUTS];
    int field_length[DEEPLEARN_MAX_CSV_INPUTS];

    COUNTDOWN(i, DEEPLEARN_MAX_CSV_INPUTS) {
        input_range_min[i] = 9999;
        input_range_max[i] = -9999;
        inputs_text[i] = 0;
    }

    COUNTDOWN(i, DEEPLEARN_MAX_CSV_OUTPUTS) {
        output_range_min[i] = 9999;
        output_range_max[i] = -9999;
    }

    if (output_classes > 0)
        network_outputs = output_classes;

    COUNTDOWN(i, network_outputs)
        outputs[i] = DEEPLEARN_UNKNOWN_VALUE;

    fp = fopen(filename,"r");

    if (!fp)
        return -1;

    while (!feof(fp)) {
        retval = fgets(line, 1999, fp);
        if (!retval)
            continue;

        if (strlen(line)==0)
            continue;

        if ((line[0]=='"') || (line[0]=='#'))
            continue;

        field_number = 0;
        input_index = 0;
        ctr = 0;
        COUNTUP(i, strlen(line)) {
            if (!((line[i] == ',') || (line[i] == ';') ||
                  (i==strlen(line)-1))) {
                /* update the value string */
                if (ctr < DEEPLEARN_MAX_FIELD_LENGTH_CHARS-1)
                    valuestr[ctr++] = line[i];
                continue;
            }

            if (i == strlen(line)-1) {
                if (ctr < DEEPLEARN_MAX_FIELD_LENGTH_CHARS-1)
                    valuestr[ctr++]=line[i];
            }
            valuestr[ctr]=0;
            ctr=0;

            /* get the value from the string */
            value = 0;
            is_text = 0;
            if ((valuestr[0]!='?') && (valuestr[0]!=10)) {
                /* positive numbers*/
                if (((valuestr[0] >= '0') &&
                     (valuestr[0] <= '9')) ||
                    /* negative numbers*/
                    ((valuestr[0] == '-') &&
                     (valuestr[1] >= '0') &&
                     (valuestr[1] <= '9'))) {
                    value = atof(valuestr);
                }
                else {
                    is_text = 1;
                }
            }

            output_ctr = 0;
            COUNTUP(j, no_of_outputs) {
                if (field_number != output_field_index[j]) {
                    output_ctr++;
                    continue;
                }

                if (j < DEEPLEARN_MAX_CSV_OUTPUTS-1) {
                    if (output_classes <= 0) {
                        outputs[j] = value;
                        break;
                    }

                    /* for a class number */
                    COUNTUP(k, network_outputs) {
                        if (k != (int)value)
                            outputs[k] = NEURON_LOW;
                        else
                            outputs[k] = NEURON_HIGH;
                    }

                    break;
                }

                output_ctr++;
            }
            if ((output_ctr == no_of_outputs) &&
                (input_index < DEEPLEARN_MAX_CSV_INPUTS-1)) {
                inputs_text[input_index] = 0;
                if (is_text != 0) {
                    /* allocate some memory for the string */
                    CHARALLOC(inputs_text[input_index],
                              strlen(valuestr)+1);
                    if (!inputs_text[input_index])
                        return -2;

                    /* copy it */
                    strcpy(inputs_text[input_index],
                           (char*)valuestr);
                }
                inputs[input_index++] = value;
            }

            field_number++;
            data_set_index++;
        }

        if (fields_per_example == 0)
            fields_per_example = field_number;

        if (samples_loaded == 0)
            no_of_input_fields = input_index;

        /* add a data sample */
        if (deeplearndata_add(&data,
                              &data_samples,
                              inputs, inputs_text,
                              outputs,
                              no_of_input_fields,
                              network_outputs,
                              input_range_min,
                              input_range_max,
                              output_range_min,
                              output_range_max) != 0) {
            fclose(fp);
            return -3;
        }

        /* free memory for any text strings */
        COUNTUP(i, no_of_input_fields) {
            if (inputs_text[i] != 0) {
                free(inputs_text[i]);
                inputs_text[i] = 0;
            }
        }

        COUNTDOWN(i, network_outputs)
            outputs[i] = DEEPLEARN_UNKNOWN_VALUE;

        samples_loaded++;
    }

    fclose(fp);

    /* calculate field lengths */
    no_of_inputs =
        deeplearndata_update_field_lengths(no_of_input_fields,
                                           field_length, data);

    /* create the deep learner */
    deeplearn_init(learner,
                   no_of_inputs, no_of_hiddens,
                   hidden_layers, network_outputs,
                   error_threshold, random_seed);

    /* set the input fields */
    learner->no_of_input_fields = no_of_input_fields;

    INTALLOC(learner->field_length, no_of_input_fields);
    if (!learner->field_length)
        return -4;

    COUNTDOWN(i, no_of_input_fields) {
        learner->field_length[i] = field_length[i];
        if (field_length[i] > 0) {
            input_range_min[i] = NEURON_LOW;
            input_range_max[i] = NEURON_HIGH;
        }
    }

    /* attach the data samples */
    learner->data = data;
    learner->data_samples = data_samples;

    /* create the indexed array for fast access */
    deeplearndata_index_data(learner->data, learner->data_samples,
                             &learner->indexed_data,
                             &learner->indexed_data_samples);

    /* set the field ranges */
    COUNTDOWN(i, no_of_input_fields) {
        learner->input_range_min[i] = input_range_min[i];
        learner->input_range_max[i] = input_range_max[i];
    }

    COUNTDOWN(i, network_outputs) {
        learner->output_range_min[i] = output_range_min[i];
        learner->output_range_max[i] = output_range_max[i];
    }

    /* create training and test data sets */
    if (deeplearndata_create_datasets(learner, 20) != 0)
        return -5;

    return samples_loaded;
}

/**
 * @brief Update the training history graph
 * @param learner Deep learner object
 */
static void deeplearndata_update_training_history(deeplearn * learner)
{
    /* plot a graph showing training progress */
    if (learner->training_ctr > learner->history.interval) {
        deeplearn_plot_history(learner,
                               DEEPLEARN_PLOT_WIDTH, DEEPLEARN_PLOT_HEIGHT);
        deeplearn_plot_gradients(GRADIENT_STANDARD_DEVIATION, learner,
                                 DEEPLEARN_PLOT_WIDTH, DEEPLEARN_PLOT_HEIGHT);
        deeplearn_plot_gradients(GRADIENT_MEAN, learner,
                                 DEEPLEARN_PLOT_WIDTH, DEEPLEARN_PLOT_HEIGHT);
        learner->training_ctr = 0;
    }
    learner->training_ctr++;
}

/**
* @brief Performs a single training step
* @param learner Deep learner object
* @returns 1=pretraining,2=final training,0=training complete,-1=no training data
*/
int deeplearndata_training(deeplearn * learner)
{
    if (learner->training_data_samples == 0)
        return -1;

    deeplearndata_update_training_history(learner);

    if ((learner->net->hidden_layers > 1) &&
        (learner->current_hidden_layer < learner->net->hidden_layers)) {
        /* index number of a random training sample */
        int index =
            rand_num(&learner->net->random_seed)%
            learner->training_data_samples;
        /* get the sample */
        deeplearndata * sample = deeplearndata_get_training(learner, index);
        deeplearn_set_inputs(learner, sample);
        deeplearn_update(learner);
        return 1;
    }

    if (learner->training_complete == 0) {
        /* index number of a random training sample */
        int index = rand_num(&learner->net->random_seed)%
            learner->training_data_labeled_samples;
        /* get the sample */
        deeplearndata * sample =
            deeplearndata_get_training_labeled(learner, index);
        deeplearn_set_inputs(learner, sample);
        deeplearn_set_outputs(learner, sample);
        deeplearn_update(learner);
        return 2;
    }
    return 0;
}

/**
* @brief Returns the performance on the test data set as a percentage value
* @param learner Deep learner object
* @return Training or test performance on the given data, in the range 0 to 100%
*/
float deeplearndata_get_performance(deeplearn * learner)
{
    int hits=0;
    float error_percent, total_error=0, average_error;
    float * outputs;

    FLOATALLOC(outputs, learner->net->no_of_outputs);
    if (!outputs)
        return -1;

    COUNTUP(index, learner->test_data_samples) {
        deeplearndata * sample = deeplearndata_get_test(learner, index);
        deeplearn_set_inputs(learner, sample);
        deeplearn_feed_forward(learner);
        deeplearn_get_outputs(learner, outputs);

        COUNTUP(i, learner->net->no_of_outputs) {
            if (sample->outputs[i] != 0) {
                error_percent =
                    (sample->outputs[i] - outputs[i]) / sample->outputs[i];
                total_error += error_percent*error_percent;
                hits++;
            }
        }
    }
    if (hits > 0) {
        average_error = (float)sqrt(total_error / hits) * 100;
        if (average_error > 100) average_error = 100;
        free(outputs);
        return 100 - average_error;
    }
    free(outputs);
    return 0;
}

/**
* @brief Returns the maximum field length for a text field
* @param data List of data samples
* @param field_index Index number of the input field
* @returns maximum field length in number of input neurons (bits)
*          Zero length indicates a numeric value
*/
int deeplearndata_get_field_length(deeplearndata * data, int field_index)
{
    int max = 0;

    while (data != 0) {
        if (data->inputs_text != 0) {
            if (data->inputs_text[field_index] != 0) {
                if (strlen(data->inputs_text[field_index])*CHAR_BITS > max)
                    max = strlen(data->inputs_text[field_index])*CHAR_BITS;
            }
        }
        data = (deeplearndata *)data->next;
    }
    return max;
}

/**
* @brief Calculates the field lengths (input neurons per field)
*        Note that a zero field length indicates a numeric value
* @param no_of_input_fields The number of input fields
* @param field_length Array storing the field lengths in input neurons (bits)
* @param data List containing data samples
* @returns The total number of input neurons needed
*/
int deeplearndata_update_field_lengths(int no_of_input_fields,
                                       int field_length[],
                                       deeplearndata * data)
{
    int no_of_inputs = 0, length;

    COUNTUP(i, no_of_input_fields) {
        length = deeplearndata_get_field_length(data, i);
        field_length[i] = length;
        if (length < 1) length = 1;
        no_of_inputs += length;
    }
    return no_of_inputs;
}
