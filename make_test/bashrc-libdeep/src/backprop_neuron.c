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

#include "backprop_neuron.h"

/**
 * @brief Randomly initialises the weights within the given range
 * @param n Backprop neuron object
 * @param random_seed Random number generator seed
 */
static void bp_neuron_init_weights(bp_neuron * n,
                                   unsigned int * random_seed)
{
    n->min_weight = 9999;
    n->max_weight = -9999;

    COUNTDOWN(i, n->no_of_inputs) {
        n->weights[i] = rand_initial_weight(random_seed, n->no_of_inputs);
        n->last_weight_change[i] = 0;

        if (n->weights[i] < n->min_weight)
            n->min_weight = n->weights[i];

        if (n->weights[i] > n->max_weight)
            n->max_weight = n->weights[i];
    }

    /* dont forget the bias value */
    n->bias = rand_initial_weight(random_seed, 2);
    n->last_bias_change = 0;
}

/**
* @brief Initialises the neuron
* @param n Backprop neuron object
* @param no_of_inputs The number of input connections
* @param random_seed Random number generator seed
*/
int bp_neuron_init(bp_neuron * n,
                   int no_of_inputs,
                   unsigned int * random_seed)
{
    /* should have more than zero inpyts */
    assert(no_of_inputs > 0);

    n->no_of_inputs = no_of_inputs;

    /* create some weights */
    FLOATALLOC(n->weights, no_of_inputs);
    if (!n->weights)
        return -1;

    FLOATALLOC(n->last_weight_change, no_of_inputs);
    if (!n->last_weight_change) {
        free(n->weights);
        return -2;
    }

    bp_neuron_init_weights(n, random_seed);
    n->desired_value = -1;
    n->value = 0;
    n->value_reprojected = 0;
    n->backprop_error = 0;
    n->excluded = 0;

    /* pointers to input neurons */
    n->inputs = (struct bp_n **)malloc(no_of_inputs*sizeof(struct bp_n *));
    if (!n->inputs) {
        free(n->weights);
        free(n->last_weight_change);
        return -3;
    }

    memset(n->inputs, '\0', no_of_inputs*sizeof(struct bp_n *));

    return 0;
}

/**
* @brief Compares two neurons and returns a non-zero value
*        if they are the same
* @param n1 First backprop neuron object
* @param n2 Second backprop neuron object
* @return 1 if they are the same, 0 otherwise
*/
int bp_neuron_compare(bp_neuron * n1, bp_neuron * n2)
{
    if ((n1->no_of_inputs != n2->no_of_inputs) || (n1->bias != n2->bias))
        return 0;

    COUNTDOWN(i, n1->no_of_inputs) {
        if ((n1->weights[i] != n2->weights[i]) ||
            (n1->last_weight_change[i] != n2->last_weight_change[i])) {
            return 0;
        }
    }
    return 1;
}

/**
* @brief Deallocates memory for a neuron
* @param n Backprop neuron object
*/
void bp_neuron_free(bp_neuron * n)
{
    /* free the weights */
    free(n->weights);
    free(n->last_weight_change);

    /* clear the pointers to input neurons */
    COUNTDOWN(i, n->no_of_inputs)
        n->inputs[i]=0;

    /* free the inputs */
    free(n->inputs);
}

/**
* @brief Activation function
* @param x The weighted sum of inputs
* @return Result of the activation function
*/
static float af(float x)
{
    return x * (1.0f - x);
}


/**
* @brief Adds a connection to a neuron
* @param dest Destination backprop neuron object
* @param index Index number of the input connection
* @param source Incoming backprop neuron object
*/
void bp_neuron_add_connection(bp_neuron * dest,
                              int index, bp_neuron * source)
{
    dest->inputs[index] = source;
}

/**
* @brief Feed forward
* @param n Backprop neuron object
* @param noise Noise in the range 0.0 to 1.0
* @param dropout_percent Dropouts percent in the range 0 -> 100
* @param random_seed Random number generator seed
*/
void bp_neuron_feedForward(bp_neuron * n,
                           float noise,
                           unsigned int dropout_percent,
                           unsigned int * random_seed)
{
    float adder;

    /* if the neuron has dropped out then set its output to zero */
    if (n->excluded > 0) {
        n->value = 0;
        return;
    }

    /* Sum with initial bias */
    adder = n->bias;

    /* calculate weighted sum of inputs */
    if (dropout_percent == 0) {
        COUNTDOWN(i, n->no_of_inputs)
            adder += n->weights[i] * n->inputs[i]->value;
    }
    else {
        COUNTDOWN(i, n->no_of_inputs) {
            if (rand_num(random_seed)%10000 > dropout_percent)
                adder += n->weights[i] * n->inputs[i]->value;
        }
    }

    /* add some random noise */
    if (noise > 0)
        adder = ((1.0f - noise) * adder) +
            (noise * ((rand_num(random_seed)%10000)/10000.0f));

    /* activation function */
    n->value = AF(adder);
}

/**
* @brief back-propagate the error
* @param n Backprop neuron object
*/
void bp_neuron_backprop(bp_neuron * n)
{
    float bperr;

    /* if the neuron has dropped out then don't continue */
    if (n->excluded > 0) return;

    /* output unit */
    if (n->desired_value > -1)
        n->backprop_error = n->desired_value - n->value;

    /* prepare variable so that we don't need to calculate
       it repeatedly within the loop */
    bperr = n->backprop_error * af(n->value);

    /* back-propogate the error */
    COUNTDOWN(i, n->no_of_inputs)
        n->inputs[i]->backprop_error += bperr * n->weights[i];
}

/**
* @brief Reprojects a neuron value back into the previous layer
* @param n Backprop neuron object
*/
void bp_neuron_reproject(bp_neuron * n)
{
    COUNTDOWN(i, n->no_of_inputs) {
        bp_neuron * nrn = n->inputs[i];
        if (nrn != 0)
            nrn->value_reprojected +=
                (n->value_reprojected * n->weights[i]);
    }
}

/**
* @brief Adjust the weights of a neuron
* @param n Backprop neuron object
* @param learning_rate Learning rate in the range 0.0 to 1.0
*/
void bp_neuron_learn(bp_neuron * n,
                     float learning_rate)
{
    float afact,e,gradient,egradient;

    if (n->excluded > 0) return;

    e = learning_rate / (1.0f + n->no_of_inputs);
    afact = af(n->value);
    gradient = afact * n->backprop_error;
    egradient = e * gradient;
    n->last_bias_change = e * (n->last_bias_change + 1.0f) * gradient;
    n->bias = CLIP_WEIGHT(n->bias + n->last_bias_change);
    n->min_weight = -2;
    n->max_weight = 2;

    /* for each input */
    COUNTDOWN(i, n->no_of_inputs) {
        if (n->inputs[i] != 0) {
            n->last_weight_change[i] =
                egradient * (n->last_weight_change[i] + 1) *
                n->inputs[i]->value;
            n->weights[i] =
                CLIP_WEIGHT(n->weights[i] + n->last_weight_change[i]);
        }
    }
}

/**
* @brief Saves neuron parameters to a file.  Note that there is no need to
         save the connections, since layers are always fully interconnected
* @param fp File pointer
* @param n Backprop neuron object
* @return zero value if saving is successful
*/
int bp_neuron_save(FILE * fp, bp_neuron * n)
{
    if (INTWRITE(n->no_of_inputs) == 0)
        return -1;

    if (FLOATWRITEARRAY(n->weights, n->no_of_inputs) == 0)
        return -2;

    if (FLOATWRITEARRAY(n->last_weight_change, n->no_of_inputs) == 0)
        return -3;

    if (FLOATWRITE(n->min_weight) == 0)
        return -4;

    if (FLOATWRITE(n->max_weight) == 0)
        return -5;

    if (FLOATWRITE(n->bias) == 0)
        return -6;

    if (FLOATWRITE(n->last_bias_change) == 0)
        return -7;

    if (FLOATWRITE(n->desired_value) == 0)
        return -8;

    return 0;
}

/**
* @brief Load neuron parameters from file
* @param fp File pointer
* @param n Backprop neuron object
* @return zero value on success
*/
int bp_neuron_load(FILE * fp, bp_neuron * n)
{
    if (INTREAD(n->no_of_inputs) == 0)
        return -1;

    if (FLOATREADARRAY(n->weights, n->no_of_inputs) == 0)
        return -2;

    if (FLOATREADARRAY(n->last_weight_change, n->no_of_inputs) == 0)
        return -3;

    if (FLOATREAD(n->min_weight) == 0)
        return -4;

    if (FLOATREAD(n->max_weight) == 0)
        return -5;

    if (FLOATREAD(n->bias) == 0)
        return -6;

    if (FLOATREAD(n->last_bias_change) == 0)
        return -7;

    if (FLOATREAD(n->desired_value) == 0)
        return -8;

    n->value = 0;
    n->backprop_error = 0;
    n->excluded = 0;

    return 0;
}
