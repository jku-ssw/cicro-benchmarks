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

#include "backprop.h"

/**
* @brief Initialise a backprop neural net
* @param net Backprop neural net object
* @param no_of_inputs The number of input units
* @param no_of_hiddens The number of units in each hidden layer
* @param hidden_layers The number of hidden layers
* @param no_of_outputs The number of output units
* @param random_seed The random number generator seed
* @returns zero on success
*/
int bp_init(bp * net,
            int no_of_inputs,
            int no_of_hiddens,
            int hidden_layers,
            int no_of_outputs,
            unsigned int * random_seed)
{
    bp_neuron * n;

    net->learning_rate = 0.2f;
    net->noise = 0.0f;
    net->random_seed = *random_seed;
    net->backprop_error = DEEPLEARN_UNKNOWN_ERROR;
    net->backprop_error_average = DEEPLEARN_UNKNOWN_ERROR;
    net->backprop_error_total = DEEPLEARN_UNKNOWN_ERROR;
    net->itterations = 0;
    net->pruning_cycle = 0;
    net->pruning_rate = 0.1f;
    net->dropout_percent = 20;

    net->no_of_inputs = no_of_inputs;
    NEURON_ARRAY_ALLOC(net->inputs, no_of_inputs);
    if (!net->inputs)
        return -1;

    net->no_of_hiddens = no_of_hiddens;
    net->no_of_outputs = no_of_outputs;
    net->hidden_layers = hidden_layers;
    NEURON_LAYERS_ALLOC(net->hiddens, hidden_layers);
    if (!net->hiddens)
        return -2;

    COUNTDOWN(l, hidden_layers) {
        NEURON_ARRAY_ALLOC(net->hiddens[l], HIDDENS_IN_LAYER(net,l));
        if (!net->hiddens[l])
            return -3;
    }

    NEURON_ARRAY_ALLOC(net->outputs, no_of_outputs);
    if (!net->outputs)
        return -4;

    /* create inputs */
    COUNTDOWN(i, net->no_of_inputs) {
        NEURONALLOC(net->inputs[i]);
        if (!net->inputs[i])
            return -5;

        if (bp_neuron_init(net->inputs[i], 1, random_seed) != 0)
            return -6;
    }

    /* create hiddens */
    COUNTUP(l, hidden_layers) {
        COUNTUP(i, HIDDENS_IN_LAYER(net,l)) {
            net->hiddens[l][i] = (bp_neuron*)malloc(sizeof(bp_neuron));
            if (!net->hiddens[l][i])
                return -7;

            n = net->hiddens[l][i];
            if (l == 0) {
                if (bp_neuron_init(n, no_of_inputs, random_seed) != 0)
                    return -8;

                /* connect to input layer */
                COUNTDOWN(j, net->no_of_inputs)
                    bp_neuron_add_connection(n, j, net->inputs[j]);
            }
            else {
                if (bp_neuron_init(n, HIDDENS_IN_LAYER(net,l-1),
                                   random_seed) != 0)
                    return -9;

                /* connect to previous hidden layer */
                COUNTDOWN(j, HIDDENS_IN_LAYER(net,l-1))
                    bp_neuron_add_connection(n, j, net->hiddens[l-1][j]);
            }
        }
    }

    /* create outputs */
    COUNTDOWN(i, net->no_of_outputs) {
        NEURONALLOC(net->outputs[i]);
        if (!net->outputs[i])
            return -10;

        n = net->outputs[i];
        if (bp_neuron_init(n, HIDDENS_IN_LAYER(net,hidden_layers-1),
                           random_seed) != 0)
            return -11;

        COUNTDOWN(j, HIDDENS_IN_LAYER(net,hidden_layers-1))
            bp_neuron_add_connection(n, j, net->hiddens[hidden_layers-1][j]);
    }

    return 0;
}

/**
* @brief Deallocate the memory for a backprop neural net object
* @param net Backprop neural net object
*/
void bp_free(bp * net)
{
    COUNTDOWN(i, net->no_of_inputs) {
        bp_neuron_free(net->inputs[i]);
        free(net->inputs[i]);
        net->inputs[i] = 0;
    }
    free(net->inputs);

    COUNTDOWN(l, net->hidden_layers) {
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,l)) {
            bp_neuron_free(net->hiddens[l][i]);
            free(net->hiddens[l][i]);
            net->hiddens[l][i] = 0;
        }
        free(net->hiddens[l]);
        net->hiddens[l] = 0;
    }
    free(net->hiddens);

    COUNTDOWN(i, net->no_of_outputs) {
        bp_neuron_free(net->outputs[i]);
        free(net->outputs[i]);
        net->outputs[i] = 0;
    }
    free(net->outputs);
}

/**
* @brief Propagates the current inputs through the layers of the network
* @param net Backprop neural net object
* @param learning Non-zero if learning
*/
void bp_feed_forward(bp * net, int learning)
{
    unsigned int drop_percent = 0;

    if (learning != 0)
        drop_percent = (unsigned int)(net->dropout_percent*100);

    /* for each hidden layer */
    COUNTUP(l, net->hidden_layers) {
        /* For each unit within the layer */
#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,l))
            bp_neuron_feedForward(net->hiddens[l][i],
                                  net->noise, drop_percent,
                                  &net->random_seed);
    }

    /* for each unit in the output layer */
    COUNTDOWN(i, net->no_of_outputs)
        bp_neuron_feedForward(net->outputs[i], net->noise, drop_percent,
                              &net->random_seed);
}

/**
* @brief Propagates the current inputs through a given number of
*        layers of the network
* @param net Backprop neural net object
* @param layers The number of layers to propagate through
*/
void bp_feed_forward_layers(bp * net, int layers)
{
    unsigned int drop_percent =
        (unsigned int)(net->dropout_percent*100);

    /* for each hidden layer */
    COUNTUP(l, layers) {
        /* if this layer is a hidden layer */
        if (l < net->hidden_layers) {
            /* For each unit within the layer */
#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
            COUNTDOWN(i, HIDDENS_IN_LAYER(net,l))
                bp_neuron_feedForward(net->hiddens[l][i],
                                      net->noise, drop_percent,
                                      &net->random_seed);
        }
        else {
            /* For each unit within the output layer */
#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
            COUNTDOWN(i, net->no_of_outputs)
                bp_neuron_feedForward(net->outputs[i],
                                      net->noise, drop_percent,
                                      &net->random_seed);
        }
    }
}

/**
* @brief back-propogate errors from the output layer towards the input layer
* @param net Backprop neural net object
*/
void bp_backprop(bp * net, int current_hidden_layer)
{
    int neuron_count=0;
    int start_hidden_layer = current_hidden_layer-1;
    float errorPercent=0;

    /* clear all previous backprop errors */
    COUNTDOWN(i, net->no_of_inputs)
        net->inputs[i]->backprop_error = 0;

    /* for every hidden layer */
    if (start_hidden_layer < 0)
        start_hidden_layer = 0;

    FOR(l, start_hidden_layer, net->hidden_layers) {
        /* For each unit within the layer */
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,l))
            net->hiddens[l][i]->backprop_error = 0;
    }

    /* now back-propogate the error from the output units */
    net->backprop_error_total = 0;

    /* for every output unit */
#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
    COUNTDOWN(i, net->no_of_outputs) {
        bp_neuron_backprop(net->outputs[i]);
    }

    COUNTDOWN(i, net->no_of_outputs) {
        /* update the total error which is used to assess
            network performance */
        net->backprop_error_total += net->outputs[i]->backprop_error;
        errorPercent += fabs(net->outputs[i]->backprop_error);
    }
    neuron_count += net->no_of_outputs;

    /* convert summed error to an overall percentage */
    errorPercent = errorPercent * 100 /
        (NEURON_RANGE*net->no_of_outputs);

    /* error on the output units */
    net->backprop_error = fabs(net->backprop_error_total / net->no_of_outputs);

    /* update the running average */
    if (net->backprop_error_average == DEEPLEARN_UNKNOWN_ERROR) {
        net->backprop_error_average = net->backprop_error;
        net->backprop_error_percent = errorPercent;
    }
    else {
        net->backprop_error_average =
            (net->backprop_error_average*0.999f) +
            (net->backprop_error*0.001f);

        net->backprop_error_percent =
            (net->backprop_error_percent*0.999f) +
            (errorPercent*0.001f);
    }

    /* back-propogate through the hidden layers */
    for (int l = net->hidden_layers-1; l >= start_hidden_layer; l--) {
        /* for every unit in the hidden layer */
#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,l)) {
            bp_neuron_backprop(net->hiddens[l][i]);
        }

        COUNTDOWN(i, HIDDENS_IN_LAYER(net,l)) {
            /* update the total error which is used to assess
                network performance */
            net->backprop_error_total += net->hiddens[l][i]->backprop_error;
        }
        neuron_count += HIDDENS_IN_LAYER(net,l);
    }

    /* overall average error */
    net->backprop_error_total =
        fabs(net->backprop_error_total / neuron_count);

    /* increment the number of training itterations */
    if (net->itterations < UINT_MAX)
        net->itterations++;
}

/**
 * @brief Reprojects the input layer from a given hidden layer neuron
 *        This is like feedforward in reverse, and allows you
 *        to visualise what a hidden layer neuron is representing
 * @param layer The hidden layer within which the neuron resides
 * @param neuron_index The hidden layer index of the neuron to be reprojected
 */
void bp_reproject(bp * net, int layer, int neuron_index)
{
    bp_neuron * n;

    /* clear all previous backprop errors */
    COUNTDOWN(i, net->no_of_inputs)
        net->inputs[i]->value_reprojected = 0;

    /* for every hidden layer */
    COUNTDOWN(l, layer) {
        /* For each unit within the layer */
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,l))
            net->hiddens[l][i]->value_reprojected = 0;
    }

    /* set the neuron active */
    n = net->hiddens[layer][neuron_index];
    n->value_reprojected = NEURON_HIGH;

    bp_neuron_reproject(n);
    if (layer > 0) {
        /* apply the sigmoid function in the previous layer,
           as with feedforward */
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,layer-1)) {
            n = net->hiddens[layer-1][i];
            n->value_reprojected =
                AF(n->value_reprojected);
        }
    }

    /* reproject through the hidden layers */
    for (int l = layer-1; l > 0; l--) {
        /* for every unit in the hidden layer */
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,l))
            bp_neuron_reproject(net->hiddens[l][i]);

        /* apply the sigmoid function in the previous layer,
           as with feedforward */
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,l-1)) {
            n = net->hiddens[l-1][i];
            n->value_reprojected =
                AF(n->value_reprojected);
        }
    }
}

/**
* @brief Adjust connection weights and bias values
* @param net Backprop neural net object
* @param current_hidden_layer The hidden layer currently being trained
*/
void bp_learn(bp * net, int current_hidden_layer)
{
    int start_hidden_layer = current_hidden_layer-1;

    /* for each hidden layers */
    if (start_hidden_layer < 0)
        start_hidden_layer = 0;

    FOR(l, start_hidden_layer, net->hidden_layers) {
#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,l))
            bp_neuron_learn(net->hiddens[l][i], net->learning_rate);
    }

#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
    COUNTDOWN(i, net->no_of_outputs)
        bp_neuron_learn(net->outputs[i], net->learning_rate);

    /* perform periodic pruning of weights so that there is a cycle
       of growth and pruning */
    if (net->pruning_cycle != 0) {
        if (net->itterations % net->pruning_cycle == 0) {
            if (net->itterations > 0) {
                bp_prune_weights(net, net->pruning_rate);
            }
        }
    }
}

/**
* @brief Set the value of an input
* @param net Backprop neural net object
* @param index The index number of the input unit
* @param value The value to set the unit to in the range 0.0 to 1.0
*/
void bp_set_input(bp * net, int index, float value)
{
    net->inputs[index]->value = value;
}

/**
* @brief Normalises the input values
* @param net Backprop neural net object
*/
void bp_normalise_inputs(bp * net)
{
    float max = 0,  min = 1;

    COUNTDOWN(i, net->no_of_inputs) {
        if (net->inputs[i]->value > max)
            max = net->inputs[i]->value;

        if (net->inputs[i]->value < min)
            min = net->inputs[i]->value;
    }

    float range = max - min;
    if (range > 0.00001f) {
        COUNTDOWN(i, net->no_of_inputs)
            net->inputs[i]->value =
            NEURON_LOW +
            ((net->inputs[i]->value-min)*NEURON_RANGE/range);
    }
}

/**
* @brief Sets the inputs to a text string
* @param net Backprop neural net object
* @param text The text string
*/
void bp_set_input_text(bp * net, char * text)
{
    enc_text_to_binary(text, net->inputs, net->no_of_inputs, 0, strlen(text));
}

/**
* @brief Set the inputs of the network from a patch within an image.
*        It is assumed that the image is mono (1 byte per pixel)
* @param net Backprop neural net object
* @param img Array storing the image
* @param image_width Width of the image in pixels
* @param image_height Height of the image in pixels
* @param tx Top left x coordinate of the patch within the image
* @param ty Top left y coordinate of the patch within the image
* @returns zero on success
*/
int bp_inputs_from_image_patch(bp * net,
                               unsigned char img[],
                               int image_width, int image_height,
                               int tx, int ty)
{
    int i = 0, idx;

    /* The patch size is calculated from the number of inputs
        of the neural net.  It's assumed to be square. */
    int patch_size = (int)sqrt(net->no_of_inputs);

    /* make sure that the patch fits within the number of inputs */
    if (patch_size*patch_size > net->no_of_inputs)
        return 1;

    /* set the inputs from the patch */
    FOR(py, ty, ty+patch_size) {
        if (py >= image_height) break;
        FOR(px, tx, tx+patch_size) {
            if (px >= image_width) break;

            /* array index within the image */
            idx = (py*image_width) + px;

            /* set the input value within the range */
            bp_set_input(net, i,
                         NEURON_LOW +
                         (img[idx]*NEURON_RANGE/255.0f));
            i++;
        }
    }

    return 0;
}

/**
* @brief Set the inputs of the network from an image.
*        It is assumed that the image is mono (1 byte per pixel)
* @param net Backprop neural net object
* @param img Array storing the image
* @param image_width Width of the image in pixels
* @param image_height Height of the image in pixels
* @returns zero on success
*/
int bp_inputs_from_image(bp * net,
                         unsigned char img[],
                         int image_width, int image_height)
{
    /* check that the number of inputs is the same as the
       number of pixels */
    if (net->no_of_inputs != image_width*image_height)
        return 1;

    /* set the input values within the range */
    COUNTDOWN(i, image_width*image_height)
        bp_set_input(net, i,
                     NEURON_LOW + (img[i]*NEURON_RANGE/255.0f));

    return 0;
}

/**
* @brief Plots weight matrices within an image
* @param net Backprop neural net object
* @param filename Filename of the image to save as
* @param image_width Width of the image in pixels
* @param image_height Height of the image in pixels
* @param input_image_width When displaying all inputs as an image this
         is the number of inputs across.  Set this to zero for the
         inputs image to be square.
*/
int bp_plot_weights(bp * net,
                    char * filename,
                    int image_width, int image_height,
                    int input_image_width)
{
    int neurons_x, neurons_y, ty, by, h, ix, iy;
    int wx, wy, inputs_x, inputs_y, n, i, unit, no_of_neurons;
    int no_of_weights,wdth, max_unit;
    float neuronx, neurony, dw, db, min_bias, max_bias;
    float min_activation, max_activation, da;
    bp_neuron ** neurons, * curr_neuron;
    unsigned char * img;

    /* allocate memory for the image */
    UCHARALLOC(img, image_width*image_height*3);
    if (!img)
        return -1;

    /* clear the image with a white background */
    memset((void*)img, '\255',
           image_width*image_height*3*sizeof(unsigned char));

    /* dimension of the neurons matrix for each layer */
    neurons_x = (int)sqrt(net->no_of_hiddens);
    neurons_y = (net->no_of_hiddens/neurons_x);

    /* dimensions of the weight matrix */
    if (input_image_width <= 0)
        inputs_x = (int)sqrt(net->no_of_inputs);
    else
        inputs_x = input_image_width;

    inputs_y = (net->no_of_inputs/inputs_x);

    no_of_weights = net->no_of_inputs;

    /* plot the inputs */
    ty = 0;
    by = image_height/(net->hidden_layers+3);
    h = (by-ty)*95/100;
    wdth = h;
    if (wdth>=image_width) wdth=image_width;
    COUNTUP(y, h) {
        iy = y*inputs_y/h;
        COUNTUP(x, wdth) {
            ix = x*inputs_x/wdth;
            unit = (iy*inputs_x) + ix;
            if (unit < net->no_of_inputs) {
                n = (y*image_width + x)*3;
                img[n] = (unsigned char)(net->inputs[unit]->value*255);
                img[n+1] = img[n];
                img[n+2] = img[n];
            }
        }
    }

    COUNTUP(layer, net->hidden_layers+1) {
        /* vertical top and bottom coordinates */
        ty = (layer+1)*image_height/(net->hidden_layers+3);
        by = (layer+2)*image_height/(net->hidden_layers+3);
        h = (by-ty)*95/100;

        /* reset ranges */
        min_bias = 9999999.0f;
        max_bias = -9999999.0f;
        min_activation = 9999999.0f;
        max_activation = -9999999.0f;

        /* number of patches across and down for the final layer */
        if (layer == net->hidden_layers) {
            neurons_x = (int)sqrt(net->no_of_outputs);
            neurons_y = (net->no_of_outputs/neurons_x);
            neurons = net->outputs;
            no_of_neurons = net->no_of_outputs;
            max_unit = net->no_of_outputs;
        }
        else {
            neurons_x = (int)sqrt(HIDDENS_IN_LAYER(net,layer));
            neurons_y = (HIDDENS_IN_LAYER(net,layer)/neurons_x);
            neurons = net->hiddens[layer];
            no_of_neurons = HIDDENS_IN_LAYER(net,layer);
            max_unit = HIDDENS_IN_LAYER(net,layer);
        }

        /* get the bias range within this layer */
        COUNTUP(y, max_unit) {
            if (neurons[y]->bias < min_bias) min_bias = neurons[y]->bias;
            if (neurons[y]->bias > max_bias) max_bias = neurons[y]->bias;
            if (neurons[y]->value < min_activation)
                min_activation = neurons[y]->value;
            if (neurons[y]->value > max_activation)
                max_activation = neurons[y]->value;
        }

        /* update ranges */
        db = max_bias - min_bias;
        da = max_activation - min_activation;

        /* for every pixel within the region */
        FOR(y, ty, by) {
            neurony = (y-ty)*neurons_y/(float)h;
            /* y coordinate within the weights */
            wy = (neurony - (int)neurony)*inputs_y;
            COUNTUP(x, image_width) {
                neuronx = x*neurons_x/(float)image_width;
                /* x coordinate within the weights */
                wx = (neuronx - (int)neuronx)*inputs_x;
                /* coordinate within the image */
                n = ((y * image_width) + x)*3;
                /* weight index */
                i = (wy*inputs_x) + wx;
                if (i < no_of_weights) {
                    /* neuron index */
                    unit = ((int)neurony*neurons_x) + (int)neuronx;
                    if (unit < no_of_neurons)  {
                        curr_neuron = neurons[unit];
                        dw = curr_neuron->max_weight -
                            curr_neuron->min_weight;
                        if (dw > 0.0001f) {
                            img[n] =
                                (int)((curr_neuron->weights[i] -
                                       curr_neuron->min_weight)*255/dw);
                            img[n+1] =
                                (int)((curr_neuron->bias - min_bias)*255/db);
                            img[n+2] =
                                (int)((curr_neuron->value - min_activation)*
                                      255/da);
                        }
                        else {
                            img[n] =
                                (int)(curr_neuron->weights[i]*255);
                            img[n+1] = img[n];
                            img[n+2] = img[n];
                        }
                    }
                }
            }
        }
        if (layer < net->hidden_layers) {
            /* dimensions of the weight matrix for the next layer */
            inputs_x = (int)sqrt(HIDDENS_IN_LAYER(net,layer));
            inputs_y = (HIDDENS_IN_LAYER(net,layer)/inputs_x);
            no_of_weights = HIDDENS_IN_LAYER(net,layer);
        }
    }

    ty = (net->hidden_layers+2)*image_height/(net->hidden_layers+3);
    by = (net->hidden_layers+3)*image_height/(net->hidden_layers+3);
    h = (by-ty)*95/100;

    inputs_x = (int)sqrt(net->no_of_outputs);
    inputs_y = (net->no_of_outputs/inputs_x);

    wdth = h;
    if (wdth >= image_width) wdth = image_width;
    COUNTUP(y, h) {
        iy = y*inputs_y/h;
        COUNTUP(x, wdth) {
            ix = x*inputs_x/wdth;
            unit = (iy*inputs_x) + ix;
            if (unit < net->no_of_outputs) {
                n = ((ty+y)*image_width + x)*3;
                img[n] = (unsigned char)(net->outputs[unit]->value*255);
                img[n+1] = img[n];
                img[n+2] = img[n];
            }
        }
    }

    /* write the image to file */
    deeplearn_write_png_file(filename,
                             (unsigned int)image_width,
                             (unsigned int)image_height,
                             24, img);

    /* free the image memory */
    free(img);
    return 0;
}

/**
* @brief Returns the value of one of the input units
* @param net Backprop neural net object
* @param index Index of the input unit
* @return value in the range 0.0 to 1.0
*/
float bp_get_input(bp * net, int index)
{
    return net->inputs[index]->value;
}

/**
* @brief Sets the value of one of the output units
* @param net Backprop neural net object
* @param index Index of the output unit
* @param value The value to set the output to in the range 0.0 to 1.0
*/
void bp_set_output(bp * net, int index, float value)
{
    net->outputs[index]->desired_value = value;
}

/**
* @brief Gets the value of one of the hidden units
* @param net Backprop neural net object
* @param layer Index number of the hidden layer
* @param index Index of the unit within the given hidden layer
* @return value in the range 0.0 to 1.0
*/
float bp_get_hidden(bp * net, int layer, int index)
{
    return net->hiddens[layer][index]->value;
}

/**
* @brief Gets the value of one of the output units
* @param net Backprop neural net object
* @param index Index of the unit within the output layer
* @return value in the range 0.0 to 1.0
*/
float bp_get_output(bp * net, int index)
{
    return net->outputs[index]->value;
}

/**
* @brief Gets the desired value of one of the output units
* @param net Backprop neural net object
* @param index Index of the unit within the output layer
* @return Desired value in the range 0.0 to 1.0
*/
float bp_get_desired(bp * net, int index)
{
    return net->outputs[index]->desired_value;
}

/**
* @brief Exclusion flags indicate that a unit has temporarily dropped out.
*        This clears the all the exclusion flags
* @param net Backprop neural net object
*/
static void bp_clear_dropouts(bp * net)
{
    /* if no dropouts then don't continue */
    if (net->dropout_percent == 0) return;

    /* for every hidden layer */
    COUNTDOWN(l, net->hidden_layers) {
        COUNTDOWN(i, HIDDENS_IN_LAYER(net,l))
            net->hiddens[l][i]->excluded = 0;
    }
}

/**
 * @brief Returns the average weight change for a given layer
 * @param net Backprop neural net object
 * @param layer_index Index of the layer
 * @returns average weight change
 */
float bp_weight_gradient_mean(bp * net, int layer_index)
{
    float total_weight_change = 0;
    int no_of_neurons = HIDDENS_IN_LAYER(net, layer_index);
    int inputs = net->hiddens[layer_index][0]->no_of_inputs;

    COUNTDOWN(i, no_of_neurons) {
        bp_neuron * n = net->hiddens[layer_index][i];
        COUNTDOWN(w, inputs)
            total_weight_change += fabs(n->last_weight_change[w]);
    }

    return total_weight_change * 10000000 / (float)(inputs * no_of_neurons);
}

/**
 * @brief Returns a weight histogram with values in the range 0 -> 1000
 * @param net Backprop neural net object
 * @param histogram Histogram array
 * @param buckets Number of histogram buckets
 * @param max_value The maximum weight magnitude
 */
void bp_weight_histogram(bp * net,
                         unsigned int histogram[], int buckets,
                         float max_value)
{
    UINTCLEAR(histogram, buckets);

    COUNTDOWN(l, net->hidden_layers) {
        int no_of_neurons = HIDDENS_IN_LAYER(net, l);
        int inputs = net->hiddens[l][0]->no_of_inputs;

        COUNTDOWN(i, no_of_neurons) {
            bp_neuron * n = net->hiddens[l][i];
            COUNTDOWN(w, inputs) {
                float value = fabs(n->weights[w]);
                if (value < max_value) {
                    histogram[(int)(value * buckets / max_value)]++;
                }
            }
        }
    }

    /* normalize */
    unsigned int max = 1;
    COUNTDOWN(i, buckets) {
        if (histogram[i] > max)
            max = histogram[i];
    }

    COUNTDOWN(i, buckets) {
        histogram[i] = histogram[i] * 1000 / max;
    }
}

/**
 * @brief Sets small weights to zero
 * @param net Backprop neural net object
 * @param threshold Pruning threshold in the range 0.0 -> 1.0
 * @returns The percent of weights pruned
 */
int bp_prune_weights(bp * net, float threshold)
{
    float mean = 0;
    unsigned int hits = 0;
    unsigned int pruned = 0;

    COUNTDOWN(l, net->hidden_layers) {
        int no_of_neurons = HIDDENS_IN_LAYER(net, l);
        int inputs = net->hiddens[l][0]->no_of_inputs;

        hits += no_of_neurons*inputs;

        COUNTDOWN(i, no_of_neurons) {
            bp_neuron * n = net->hiddens[l][i];
            COUNTDOWN(w, inputs) {
                mean += fabs(n->weights[w]);
            }
        }
    }

    COUNTDOWN(i, net->no_of_outputs) {
        bp_neuron * n = net->outputs[i];
        COUNTDOWN(w, n->no_of_inputs) {
            mean += fabs(n->weights[w]);
            hits++;
        }
    }

    if (hits == 0)
        return 0;

    mean /= (float)hits;
    threshold = mean * threshold;

    /* set weights to zero if they are below the pruning threshold */
    COUNTDOWN(l, net->hidden_layers) {
        int no_of_neurons = HIDDENS_IN_LAYER(net, l);
        int inputs = net->hiddens[l][0]->no_of_inputs;

        COUNTDOWN(i, no_of_neurons) {
            bp_neuron * n = net->hiddens[l][i];
            COUNTDOWN(w, inputs) {
                if (fabs(n->weights[w]) < threshold) {
                    n->weights[w] = 0;
                    pruned++;
                }
            }
        }
    }

    COUNTDOWN(i, net->no_of_outputs) {
        bp_neuron * n = net->outputs[i];
        COUNTDOWN(w, n->no_of_inputs) {
            if (fabs(n->weights[w]) < threshold) {
                n->weights[w] = 0;
                pruned++;
            }
        }
    }

    return (int)(pruned * 100 / hits);
}

/**
 * @brief Returns the standard deviation of the weight change for a given layer
 * @param net Backprop neural net object
 * @param layer_index Index of the layer
 * @returns standard deviation of weight change
 */
float bp_weight_gradient_std(bp * net, int layer_index)
{
    float mean_weight_change = 0;
    float total_deviation = 0;
    int no_of_neurons = HIDDENS_IN_LAYER(net, layer_index);
    int inputs = net->hiddens[layer_index][0]->no_of_inputs;

    /* calculate the average weight magnitude */
    COUNTDOWN(i, no_of_neurons) {
        bp_neuron * n = net->hiddens[layer_index][i];
        COUNTDOWN(w, inputs) {
            mean_weight_change += n->last_weight_change[w];
        }
    }
    mean_weight_change /= (no_of_neurons * inputs);

    /* sum of percentage deviation from the average weight magnitude */
    if (fabs(mean_weight_change) > 0.0000000001f) {
        COUNTDOWN(i, no_of_neurons) {
            bp_neuron * n = net->hiddens[layer_index][i];
            COUNTDOWN(w, inputs) {
                total_deviation +=
                    fabs((n->last_weight_change[w] - mean_weight_change)/mean_weight_change);
            }
        }
    }

    return total_deviation * 100 / (no_of_neurons * inputs);
}

/**
* @brief Randomly sets exclusion flags to cause units to drop out
* @param net Backprop neural net object
*/
static void bp_dropouts(bp * net)
{
    int no_of_dropouts, hidden_units=0;

    if (net->dropout_percent == 0) return;

    /* total number of hidden units */
    COUNTDOWN(l, net->hidden_layers)
        hidden_units += HIDDENS_IN_LAYER(net,l);

    /* total number of dropouts */
    no_of_dropouts = net->dropout_percent*hidden_units/100;

    /* set the exclusion flags */
    COUNTDOWN(n, no_of_dropouts) {
        int l = rand_num(&net->random_seed)%net->hidden_layers;
        int i = rand_num(&net->random_seed)%HIDDENS_IN_LAYER(net,l);
        net->hiddens[l][i]->excluded = 1;
    }
}

/**
* @brief Update the neural net during training
* @param net Backprop neural net object
*/
void bp_update(bp * net, int current_hidden_layer)
{
    bp_dropouts(net);
    bp_feed_forward(net, 1);
    bp_backprop(net, current_hidden_layer);
    bp_learn(net, current_hidden_layer);
    bp_clear_dropouts(net);
}

/**
* @brief Save a neural network to file
* @brief fp File pointer
* @param net Backprop neural net object
* @return zero on success
*/
int bp_save(FILE * fp, bp * net)
{
    if (UINTWRITE(net->itterations) == 0)
        return -1;

    if (UINTWRITE(net->pruning_cycle) == 0)
        return -2;

    if (FLOATWRITE(net->pruning_rate) == 0)
        return -3;

    if (INTWRITE(net->no_of_inputs) == 0)
        return -4;

    if (INTWRITE(net->no_of_hiddens) == 0)
        return -5;

    if (INTWRITE(net->no_of_outputs) == 0)
        return -6;

    if (INTWRITE(net->hidden_layers) == 0)
        return -7;

    if (FLOATWRITE(net->learning_rate) == 0)
        return -8;

    if (FLOATWRITE(net->noise) == 0)
        return -9;

    if (FLOATWRITE(net->backprop_error_average) == 0)
        return -10;

    if (FLOATWRITE(net->dropout_percent) == 0)
        return -11;

    if (UINTWRITE(net->random_seed) == 0)
        return -12;

    COUNTUP(l, net->hidden_layers) {
        COUNTUP(i, HIDDENS_IN_LAYER(net,l))
            bp_neuron_save(fp,net->hiddens[l][i]);
    }

    COUNTUP(i, net->no_of_outputs)
        bp_neuron_save(fp,net->outputs[i]);

    return 0;
}

/**
* @brief Load a network from file
* @brief fp File pointer
* @param net Backprop neural net object
* @returns zero on success
*/
int bp_load(FILE * fp, bp * net)
{
    int no_of_inputs=0, no_of_hiddens=0, no_of_outputs=0;
    int hidden_layers=0;
    float learning_rate=0, noise=0, backprop_error_average=0;
    float dropout_percent=0,pruning_rate=0;
    unsigned int itterations=0;
    unsigned int pruning_cycle=0;
    unsigned int random_seed=0;

    if (UINTREAD(itterations) == 0)
        return -1;

    if (UINTREAD(pruning_cycle) == 0)
        return -2;

    if (FLOATREAD(pruning_rate) == 0)
        return -3;

    if (INTREAD(no_of_inputs) == 0)
        return -4;

    if (INTREAD(no_of_hiddens) == 0)
        return -5;

    if (INTREAD(no_of_outputs) == 0)
        return -6;

    if (INTREAD(hidden_layers) == 0)
        return -7;

    if (FLOATREAD(learning_rate) == 0)
        return -8;

    if (FLOATREAD(noise) == 0)
        return -9;

    if (FLOATREAD(backprop_error_average) == 0)
        return -10;

    if (FLOATREAD(dropout_percent) == 0)
        return -11;

    if (UINTREAD(random_seed) == 0)
        return -12;

    if (bp_init(net, no_of_inputs, no_of_hiddens,
                hidden_layers, no_of_outputs,
                &random_seed) != 0)
        return -13;

    COUNTUP(l, net->hidden_layers) {
        COUNTUP(i, HIDDENS_IN_LAYER(net,l)) {
            if (bp_neuron_load(fp,net->hiddens[l][i]) != 0)
                return -14;
        }
    }

    COUNTUP(i, net->no_of_outputs) {
        if (bp_neuron_load(fp,net->outputs[i]) != 0)
            return -15;
    }

    net->learning_rate = learning_rate;
    net->noise = noise;
    net->backprop_error_average = backprop_error_average;
    net->backprop_error = backprop_error_average;
    net->backprop_error_total = backprop_error_average;
    net->itterations = itterations;
    net->dropout_percent = dropout_percent;
    net->pruning_cycle = pruning_cycle;
    net->pruning_rate = pruning_rate;

    return 0;
}

/**
* @brief compares two networks and returns a greater than
*        zero value if they are the same
* @param net1 The first backprop neural net object
* @param net2 The second backprop neural net object
*/
int bp_compare(bp * net1, bp * net2)
{
    int retval;

    if (net1->no_of_inputs != net2->no_of_inputs)
        return -1;

    if (net1->no_of_hiddens != net2->no_of_hiddens)
        return -2;

    if (net1->no_of_outputs != net2->no_of_outputs)
        return -3;

    if (net1->hidden_layers != net2->hidden_layers)
        return -4;

    if (net1->learning_rate != net2->learning_rate)
        return -5;

    if (net1->pruning_cycle != net2->pruning_cycle)
        return -6;

    if (net1->pruning_rate != net2->pruning_rate)
        return -7;

    if (net1->noise != net2->noise)
        return -8;

    COUNTDOWN(l, net1->hidden_layers) {
        COUNTDOWN(i, HIDDENS_IN_LAYER(net1,l)) {
            retval =
                bp_neuron_compare(net1->hiddens[l][i],
                                  net2->hiddens[l][i]);
            if (retval == 0)
                return -9;
        }
    }

    COUNTDOWN(i, net1->no_of_outputs) {
        retval = bp_neuron_compare(net1->outputs[i], net2->outputs[i]);
        if (retval == 0)
            return -10;
    }

    if (net1->itterations != net2->itterations)
        return -11;

    if (net1->backprop_error_average != net2->backprop_error_average)
        return -12;

    if (net1->dropout_percent!= net2->dropout_percent)
        return -13;

    return 1;
}

/**
* @brief Extract the classification string from the filename.
*        This assumes a filename of the type class.instance.extension
* @param filename The filename to examine
* @param classification The returned classification
*/
void bp_get_classification_from_filename(char * filename,
                                         char * classification)
{
    int j, start=0;

    /* start with an empty classification string */
    classification[0] = 0;

    /* find the last separator */
    COUNTUP(i, strlen(filename)) {
        if (filename[i] == '/')
            start = i+1;
    }

    /* find the first full stop */
    for (j = start; j < strlen(filename); j++) {
        if ((filename[j] == '.') ||
            (filename[j] == '-') ||
            (filename[j] == '_')) break;
        classification[j-start] = filename[j];
    }

    /* add a string terminator */
    classification[j-start] = 0;
}

/**
* @brief Takes a set of classification text descriptions (labels) for
*        each instance within a training or test set and produces an
*        array of classification numbers corresponding to the text
*        descriptions. It's easier for the system to deal with
*        classification numbers rather than text descriptions.
* @param no_of_instances The number of instances in the training or test set
* @param instance_classification Text Description for each instance
* @param numbers Array of numbers corresponding to each instance
*/
int bp_classifications_to_numbers(int no_of_instances,
                                  char ** instance_classification,
                                  int * numbers)
{
    int j;
    int unique_ctr = 0;
    char ** unique_classification;

    /* allocate memory for a list of unique descriptions (labels) */
    CHARPTRALLOC(unique_classification, no_of_instances);
    if (!unique_classification)
        return -1;

    /* create a list of unique classification names */
    COUNTUP(i, no_of_instances) {
        /* for every class number assigned so far */
        for (j = 0; j < unique_ctr; j++) {
            /* is this instance description (label) the same as a previous
                instance description ? */
            if (strcmp(instance_classification[i],
                       unique_classification[j])==0) {
                /* assign the same class number and finish the search */
                numbers[i] = j;
                break;
            }
        }
        /* if this instance has a description which has not been used before */
        if (j == unique_ctr) {
            /* store the description */
            CHARALLOC(unique_classification[unique_ctr],
                      1+strlen(instance_classification[i]));
            if (!unique_classification[unique_ctr]) {
                COUNTDOWN(i, unique_ctr)
                    free(unique_classification[i]);
                free(unique_classification);
                return -2;
            }

            sprintf(unique_classification[unique_ctr],
                    "%s", instance_classification[i]);

            /* store the classification number */
            numbers[i] = unique_ctr;

            /* increment the number of classes */
            unique_ctr++;
        }
    }

    /* free memory which was used to store descriptions */
    COUNTDOWN(i, unique_ctr)
        free(unique_classification[i]);

    free(unique_classification);
    return 0;
}
