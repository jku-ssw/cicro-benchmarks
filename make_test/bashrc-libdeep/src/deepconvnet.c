/*
  libdeep - a library for deep learning
  Copyright (C) 2015-2017  Bob Mottram <bob@freedombone.net>

  A deep convolution network combines a convolutional net as a
  preprocessing stage together with a deep learning system as the
  final stage. The use of convolution greatly reduces the
  dimensionality of the problem for image processing, since the
  neural nets at each convolution layer only handle a small patch
  of the input surface.

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

#include "deepconvnet.h"

/**
 * @brief Initialise a deep convnet
 * @param no_of_convolutions Number of layers in the convolution
 *        preprocessing stage
 * @param no_of_deep_layers Number of layers in the deep learner
 * @param image_width Horizontal resolution of input images
 * @param image_height Vertical resolution of input images
 * @param image_depth Depth of input images in bytes
 * @param max_features The number of features learned at each convolution layer
 * @param feature_width Width of each feature in the input image
 * @param final_image_width Width of the output of the convolutional stage
 * @param final_image_height Height of the output of the convolutional stage
 * @param layer_itterations The number of training itterations for each
 *        convolution layer
 * @param no_of_outputs Number of outputs of the deep learner
 * @param convnet Deep convnet object
 * @param error_threshold Array containing learning thresholds (percent error)
 *        for each layer of both the convolution stage and the deep learner
 * @param random_seed Random number generator seed
 * @return zero on success
 */
int deepconvnet_init(int no_of_convolutions,
                     int no_of_deep_layers,
                     int image_width,
                     int image_height,
                     int image_depth,
                     int max_features,
                     int feature_width,
                     int final_image_width,
                     int final_image_height,
                     unsigned int layer_itterations,
                     int no_of_outputs,
                     deepconvnet * convnet,
                     float error_threshold[],
                     unsigned int * random_seed)
{
    deeplearn_history_init(&convnet->history, "training.png",
                           "Training History",
                           "Time Step", "Training Error %");

    convnet->convolution = (deeplearn_conv*)malloc(sizeof(deeplearn_conv));

    convnet->layer_itterations = layer_itterations;

    if (!convnet->convolution)
        return -1;

    if (conv_init(no_of_convolutions,
                  image_width,
                  image_height,
                  image_depth,
                  max_features, feature_width,
                  final_image_width, final_image_height,
                  convnet->convolution) != 0)
        return -2;

    convnet->learner = (deeplearn*)malloc(sizeof(deeplearn));

    if (!convnet->learner)
        return -3;

    if (deeplearn_init(convnet->learner,
                       convnet->convolution->no_of_outputs,
                       convnet->convolution->no_of_outputs*8/10,
                       no_of_deep_layers,
                       no_of_outputs,
                       &error_threshold[0],
                       random_seed) != 0)
        return -4;

    convnet->training_complete = 0;
    convnet->no_of_images = 0;
    convnet->images = NULL;
    convnet->classifications = NULL;
    convnet->classification_number = NULL;

    /* default history settings */
    convnet->backprop_error = DEEPLEARN_UNKNOWN_ERROR;
    convnet->training_ctr = 0;

    convnet->current_layer = 0;
    convnet->training_set_index = NULL;
    convnet->test_set_index = NULL;
    return 0;
}

/**
 * @brief Is the given convnet still training?
 * @param convnet Deep convnet object
 * @returns True if still training
 */
int deepconvnet_is_training(deepconvnet * convnet)
{
    return ((convnet->convolution->current_layer <
             convnet->convolution->no_of_layers) ||
            (convnet->learner->training_complete != 0));
}

/**
 * @brief Frees memory
 * @param convnet Deep convnet object
 */
void deepconvnet_free(deepconvnet * convnet)
{
    conv_free(convnet->convolution);
    free(convnet->convolution);

    deeplearn_free(convnet->learner);
    free(convnet->learner);

    if (convnet->no_of_images > 0) {
        COUNTDOWN(i, convnet->no_of_images) {
            if (convnet->images[i] != NULL) {
                free(convnet->images[i]);
                convnet->images[i] = 0;
            }
            free(convnet->classifications[i]);
        }
        free(convnet->images);
        free(convnet->classifications);
        free(convnet->classification_number);
        convnet->no_of_images = 0;
    }

    if (convnet->training_set_index != NULL)
        free(convnet->training_set_index);

    if (convnet->test_set_index != NULL)
        free(convnet->test_set_index);
}

/**
 * @brief Update the learning history
 * @param convnet Deep convnet object
 */
static void deepconvnet_update_history(deepconvnet * convnet)
{
    float error_value;

    if (convnet->convolution->current_layer <
        convnet->convolution->no_of_layers)
        error_value = conv_get_error(convnet->convolution);
    else
        error_value = convnet->learner->backprop_error;

    if (error_value == DEEPLEARN_UNKNOWN_ERROR)
        error_value = 0;

    deeplearn_history_update(&convnet->history, error_value);
}

/**
 * @brief Saves the given deep convnet object to a file
 * @param fp File pointer
 * @param convnet Deep convnet object
 * @return zero value on success
 */
int deepconvnet_save(FILE * fp, deepconvnet * convnet)
{
    if (conv_save(fp, convnet->convolution) != 0)
        return -1;

    if (deeplearn_save(fp, convnet->learner) != 0)
        return -2;

    return 0;
}

/**
 * @brief Loads a deep convnet object from file
 * @param fp File pointer
 * @param convnet Deep convnet object
 * @return zero value on success
 */
int deepconvnet_load(FILE * fp, deepconvnet * convnet)
{
    if (conv_load(fp, convnet->convolution) != 0)
        return -1;

    if (deeplearn_load(fp, convnet->learner) != 0)
        return -2;

    return 0;
}

/**
 * @brief Sets the input values to the outputs of convolutional net.
 *        This provides the glue sticking the preprocessing convolutional
 *        stage together with the final deep learning stage.
 * @param learner Deep learner object
 * @param conv Convolution object
 * @return zero on success
 */
static int deepconvnet_set_inputs_conv(deeplearn * learner,
                                       deeplearn_conv * conv)
{
    if (learner->net->no_of_inputs != conv->no_of_outputs)
        return -1;

    COUNTDOWN(i, learner->net->no_of_inputs)
        deeplearn_set_input(learner, i, conv_get_output(conv, i));

    return 0;
}

/**
 * @brief Updates the current training error
 * @param convnet Deep convnet object
 */
static void deepconvnet_update_training_error(deepconvnet * convnet)
{
    /* update the backprop error */
    if (convnet->current_layer <
        convnet->convolution->no_of_layers)
        convnet->backprop_error = conv_get_error(convnet->convolution);
    else
        convnet->backprop_error = convnet->learner->backprop_error;
}

/**
 * @brief Updates the index of the current layer being trained
 * @param convnet Deep convnet object
 */
static void deepconvnet_update_current_layer(deepconvnet * convnet)
{
    /* set the current layer */
    convnet->current_layer =
        convnet->convolution->current_layer +
        convnet->learner->current_hidden_layer;
}

/**
 * @brief Periodicaly plots the training error in a graph
 * @param convnet Deep convnet object
 */
static void deepconvnet_update_training_plot(deepconvnet * convnet)
{
    /* plot a graph showing training progress */
    if (convnet->history.interval > 0) {
        if (convnet->training_ctr > convnet->history.interval) {
            if (strlen(convnet->history.filename) > 0)
                deepconvnet_plot_history(convnet,
                                         DEEPLEARN_PLOT_WIDTH,
                                         DEEPLEARN_PLOT_HEIGHT);

            convnet->training_ctr = 0;
        }
        convnet->training_ctr++;
    }
}

/**
 * @brief Used during training to update error, current layer, etc
 * @param convnet Deep convnet object
 */
static void deepconvnet_update(deepconvnet * convnet)
{
    deepconvnet_update_training_error(convnet);
    deepconvnet_update_current_layer(convnet);
    deepconvnet_update_history(convnet);
    deepconvnet_update_training_plot(convnet);
    convnet->convolution->training = (convnet->learner->training_complete==0);
    convnet->training_complete = convnet->learner->training_complete;
}

/**
 * @brief Update routine for training the system
 * @param convnet Deep convnet object
 * @param img Array containing input image
 * @param samples The number of samples to take from the image
 *        for convolution network training
 * @param layer_itterations The number of training itterations per layer
 *        for the convolution network
 * @param class_number Desired class number
 * @return Zero on success
 */
int deepconvnet_update_img(deepconvnet * convnet, unsigned char img[],
                           int samples, unsigned int layer_itterations,
                           int class_number)
{
    unsigned int * random_seed = &convnet->learner->net->random_seed;

    if (convnet->convolution->current_layer <
        convnet->convolution->no_of_layers) {
        conv_learn(img, convnet->convolution, samples,
                   layer_itterations, random_seed);
        deepconvnet_update(convnet);
        return 0;
    }

    convnet->convolution->training =
        (convnet->learner->training_complete==0);

    conv_feed_forward(img, convnet->convolution,
                      convnet->convolution->no_of_layers);

    if (deepconvnet_set_inputs_conv(convnet->learner,
                                    convnet->convolution) != 0)
        return -2;

    if (convnet->learner->training_complete == 0) {
        if (deeplearn_training_last_layer(convnet->learner))
            deeplearn_set_class(convnet->learner, class_number);

        deeplearn_update(convnet->learner);
        deepconvnet_update(convnet);
    }
    else {
        deeplearn_feed_forward(convnet->learner);
    }
    return 0;
}

/**
 * @brief Test a convnet with an unknown image
 * @param convnet Deep convnet object
 * @param img Array containing input image
 * @return Zero on success
 */
int deepconvnet_test_img(deepconvnet * convnet, unsigned char img[])
{
    conv_feed_forward(img, convnet->convolution,
                      convnet->convolution->no_of_layers);

    if (deepconvnet_set_inputs_conv(convnet->learner,
                                    convnet->convolution) != 0)
        return -2;

    deeplearn_feed_forward(convnet->learner);

    return 0;
}

/**
 * @brief Sets a desired output value
 * @param convnet Deep convnet object
 * @param index Index of the output unit
 * @param value Value to set in the range 0.0 - 1.0
 */
void deepconvnet_set_output(deepconvnet * convnet, int index, float value)
{
    deeplearn_set_output(convnet->learner, index, value);
}

/**
 * @brief Sets a desired output class
 * @param convnet Deep convnet object
 * @param class_number The class number (output index to be set active)
 */
void deepconvnet_set_class(deepconvnet * convnet, int class_number)
{
    deeplearn_set_class(convnet->learner, class_number);
}

/**
 * @brief Returns the value of an output
 * @param convnet Deep convnet object
 * @param index Index of the output unit
 * @return Value of the output
 */
float deepconvnet_get_output(deepconvnet * convnet, int index)
{
    return deeplearn_get_output(convnet->learner, index);
}

/**
 * @brief Returns the output class
 * @param convnet Deep convnet object
 * @return The class number (output index to be set active)
 */
int deepconvnet_get_class(deepconvnet * convnet)
{
    return deeplearn_get_class(convnet->learner);
}

/**
 * @brief Sets the learning rate
 * @param convnet Deep convnet object
 * @param rate the learning rate in the range 0.0 to 1.0
 */
void deepconvnet_set_learning_rate(deepconvnet * convnet, float rate)
{
    convnet->convolution->learning_rate = rate;
    deeplearn_set_learning_rate(convnet->learner, rate);
}

/**
 * @brief Sets the percentage of units which drop out during training
 * @param convnet Deep convnet object
 * @param dropout_percent Percentage of units which drop out in the range 0 to 100
 */
void deepconvnet_set_dropouts(deepconvnet * convnet, float dropout_percent)
{
    deeplearn_set_dropouts(convnet->learner, dropout_percent);
}

/**
 * @brief Uses gnuplot to plot the training error for the given learner
 * @param convnet Deep convnet object
 * @param image_width Width of the image in pixels
 * @param image_height Height of the image in pixels
 * @return zero on success
 */
int deepconvnet_plot_history(deepconvnet * convnet,
                             int image_width, int image_height)
{
    return deeplearn_history_plot(&convnet->history,
                                  image_width, image_height);
}

/**
 * @brief Performs training
 * @param convnet Deep convnet object
 * @returns zero on success
 */
int deepconvnet_training(deepconvnet * convnet)
{
    if (convnet->learner->training_complete != 0)
        return 1;

    if (convnet->no_of_images == 0)
        return 0;

    if (convnet->classification_number == NULL)
        return -1;

    /* pick an image at random */
    int training_images = convnet->no_of_images*8/10;
    unsigned int * random_seed = &convnet->learner->net->random_seed;
    int index0 =
        rand_num(random_seed)%training_images;
    int index = convnet->training_set_index[index0];
    unsigned char * img = convnet->images[index];
    int samples = 20;

    if (deepconvnet_update_img(convnet, img, samples,
                               convnet->layer_itterations,
                               convnet->classification_number[index]) != 0)
        return -2;

    return 0;
}

/**
 * @brief Returns performance on the test set
 * @param convnet Deep convnet object
 * @return Percentage of correct classifications or a negative number on error
 */
float deepconvnet_get_performance(deepconvnet * convnet)
{
    float error_percent;
    float total_error = 0;
    int test_images = convnet->no_of_images*2/10;

    if (convnet->no_of_images == 0)
        return -1;

    if (convnet->classification_number == NULL)
        return -2;

    COUNTDOWN(i, test_images) {
        int index = convnet->test_set_index[i];

        deeplearn_set_class(convnet->learner,
                            convnet->classification_number[index]);

        if (deepconvnet_test_img(convnet, convnet->images[index]) != 0) {
            printf("deepconvnet_test_img failed\n");
            break;
        }

        COUNTDOWN(i, convnet->learner->net->no_of_outputs) {
            error_percent =
                (deeplearn_get_desired(convnet->learner,i) -
                 deeplearn_get_output(convnet->learner,i)) /
                NEURON_RANGE;

            total_error += error_percent*error_percent;
        }
    }

    return 100 -
        ((float)sqrt(total_error /
                     (convnet->learner->net->no_of_outputs*test_images))*100);
}

/**
 * @brief Creates training and test arrays which contain randomly ordered
 *        indexes to the main images array. This tries to ensure that there
 *        is no bias depending on the sequences of inputs during training.
 * @param convnet Deep convnet object
 */
int deepconvnet_create_training_test_sets(deepconvnet * convnet)
{
    int i, j;

    /* create arrays to store randomly ordered array indexes
       for training and test sets */
    int training_images = convnet->no_of_images*8/10;

    INTALLOC(convnet->training_set_index, training_images+1);
    if (!convnet->training_set_index)
        return -1;

    i = 0;
    while (i < training_images) {
        int index =
            rand_num(&convnet->learner->net->random_seed)%convnet->no_of_images;

        for (j = 0; j < i; j++) {
            if (convnet->training_set_index[j] == index)
                break;
        }

        if (j == i)
            convnet->training_set_index[i++] = index;
    }
    int test_images = 0;

    INTALLOC(convnet->test_set_index,
             convnet->no_of_images - training_images + 1);
    if (!convnet->test_set_index) return -2;

    for (i = 0; i < convnet->no_of_images; i++) {
        for (j = 0; j < training_images; j++) {
            if (convnet->training_set_index[j] == i)
                break;
        }

        if (j == training_images)
            convnet->test_set_index[test_images++] = i;
    }
    return 0;
}


/**
 * @brief Reads images from a given directory and creates a deep convnet
 * @param directory Directory containing png images
 * @param convnet Deep convnet object
 * @param image_width Image width
 * @param image_height Image height
 * @param no_of_convolutions The number of convolution layers
 * @param max_features_per_convolution Number of features learned at
 *        each convolution layer
 * @param feature_width Width of features in the input image
 * @param final_image_width Width of the output of the convolutional network
 * @param final_image_height Height of the output of the convolutional network
 * @param layer_itterations Number of training itterations for each
 *        convolution layer
 * @param no_of_deep_layers Number of layers for the deep learner
 * @param no_of_outputs Number of output units
 * @param output_classes The number of output classes if the output in the
 *        data set is a single integer value
 * @param error_threshold Training error thresholds for each hidden layer
 * @param random_seed Random number seed
 * @param extra_synthetic_images The number of extra synthetic images
 *        to generate for each loaded image
 * @return zero on success
 */
int deepconvnet_read_images(char * directory,
                            deepconvnet * convnet,
                            int image_width, int image_height,
                            int no_of_convolutions,
                            int max_features_per_convolution,
                            int feature_width,
                            int final_image_width,
                            int final_image_height,
                            unsigned int layer_itterations,
                            int no_of_deep_layers,
                            int no_of_outputs,
                            int output_classes,
                            float error_threshold[],
                            unsigned int * random_seed,
                            int extra_synthetic_images)
{
    if (deepconvnet_init(no_of_convolutions,
                         no_of_deep_layers,
                         image_width, image_height, 1,
                         max_features_per_convolution,
                         feature_width,
                         final_image_width,
                         final_image_height,
                         layer_itterations,
                         no_of_outputs,
                         convnet,
                         error_threshold,
                         random_seed) != 0)
        return -1;

    convnet->no_of_images =
        deeplearn_load_training_images(directory, &convnet->images,
                                       &convnet->classifications,
                                       &convnet->classification_number,
                                       image_width, image_height,
                                       extra_synthetic_images);
    if (convnet->no_of_images <= 0)
        return -2;

    if (deepconvnet_create_training_test_sets(convnet) != 0)
        return -3;

    return 0;
}

/**
 * @brief Plots the features learned for a given convolution layer
 * @param convnet Deep convnet object
 * @param layer_index Index number of the convolution layer
 * @param filename Filename to save the image as (must be PNG format)
 * @param img_width Image width
 * @param img_height Image height
 * @return zero on success
 */
int deepconvnet_plot_features(deepconvnet * convnet,
                              int layer_index,
                              char * filename,
                              int img_width, int img_height)
{
    unsigned char * img;

    /* allocate memory for the image */
    UCHARALLOC(img, img_width*img_height*3);
    if (!img)
        return -1;

    int retval = conv_draw_features(img, img_width, img_height, 3,
                                    layer_index, convnet->convolution);
    if (retval != 0)
        return retval;

    deeplearn_write_png_file(filename,
                             (unsigned int)img_width,
                             (unsigned int)img_height,
                             24, img);

    free(img);
    return 0;
}
