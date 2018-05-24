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

#include "deeplearn_conv.h"

/**
 * @brief Create a number of convolutional layers
 * @param no_of_layers The number of layers
 * @param image_width Width of the input image or layer
 * @param image_height Height of the input image or layer
 * @param image_depth Depth of the input image
 * @param no_of_features The number of features to learn in the first layer
 * @param feature_width Width of features in the first layer
 * @param final_image_width Width of the final output layer
 * @param final_image_height Height of the final layer
 * @param conv Instance to be updated
 * @returns zero on success
 */
int conv_init(int no_of_layers,
              int image_width, int image_height, int image_depth,
              int no_of_features, int feature_width,
              int final_image_width, int final_image_height,
              deeplearn_conv * conv)
{
    conv->no_of_layers = no_of_layers;
    conv->current_layer = 0;
    conv->learning_rate = 0.1f;
    conv->training = (1==1);

    conv->noise = 0.1f;
    conv->random_seed = 672593;

    deeplearn_history_init(&conv->history, "feature_learning.png",
                           "Feature Learning Training History",
                           "Time Step", "Training Error %");

    COUNTUP(l, no_of_layers) {
        conv->layer[l].ctr = (unsigned int)0;
        conv->layer[l].depth = image_depth;
        conv->layer[l].no_of_features = no_of_features;

        conv->layer[l].width =
            image_width -
            ((image_width-final_image_width)*l/no_of_layers);

        conv->layer[l].pooling_factor = 1;

        /* After the initial layer, width and height are the same */
        if (l == 0)
            conv->layer[l].height =
                image_height -
                ((image_height-final_image_height)*l/no_of_layers);
        else {
            if (conv->layer[l].width / POOLING_FACTOR > final_image_width) {
                conv->layer[l].width /= POOLING_FACTOR;
                conv->layer[l-1].pooling_factor = POOLING_FACTOR;
            }
            conv->layer[l].height = conv->layer[l].width;
        }

        /* make feature width proportional to width of the layer */
        conv->layer[l].feature_width =
            feature_width*conv->layer[l].width/image_width;

        /* feature width should not be too small */
        if (conv->layer[l].feature_width < 3)
            conv->layer[l].feature_width = 3;

        if (l == 0) {
            /* allocate memory for the layer */
            FLOATALLOC(conv->layer[l].layer,
                       conv->layer[l].width*conv->layer[l].height*
                       conv->layer[l].depth);
            if (!conv->layer[l].layer)
                return 1;
            FLOATCLEAR(conv->layer[l].layer,
                       conv->layer[l].width*conv->layer[l].height*
                       conv->layer[l].depth);
        }
        else {
            /* allocate memory for the layer */
            FLOATALLOC(conv->layer[l].layer,
                       conv->layer[l].width*conv->layer[l].height*
                       conv->layer[l].depth*conv->layer[l-1].no_of_features);
            if (!conv->layer[l].layer)
                return 1;
            FLOATCLEAR(conv->layer[l].layer,
                       conv->layer[l].width*conv->layer[l].height*
                       conv->layer[l].depth*conv->layer[l-1].no_of_features);
        }

        /* allocate memory for learned feature set */
        FLOATALLOC(conv->layer[l].feature,
                   conv->layer[l].no_of_features*
                   conv->layer[l].feature_width*conv->layer[l].feature_width*
                   conv->layer[l].depth);
        if (!conv->layer[l].feature)
            return 2;
        FLOATCLEAR(conv->layer[l].feature,
                   conv->layer[l].no_of_features*
                   conv->layer[l].feature_width*conv->layer[l].feature_width*
                   conv->layer[l].depth);
    }

    if (conv->layer[no_of_layers-1].width < final_image_width) {
        printf("%d %d\n", conv->layer[no_of_layers-1].width, final_image_width);
        return 3;
    }

    conv->outputs_width = final_image_width;

    /* for convenience this is the size of the outputs array */
    conv->no_of_outputs =
        final_image_width*final_image_width*image_depth*
        conv->layer[no_of_layers-1].no_of_features;

    /* allocate array of output values */
    FLOATALLOC(conv->outputs, conv->no_of_outputs);
    if (!conv->outputs)
        return 4;

    /* clear the outputs */
    FLOATCLEAR(conv->outputs, conv->no_of_outputs);

    return 0;
}

/**
 * @brief Frees memory for a preprocessing pipeline
 * @param conv Convolution instance
 */
void conv_free(deeplearn_conv * conv)
{
    COUNTDOWN(l, conv->no_of_layers) {
        free(conv->layer[l].layer);
        free(conv->layer[l].feature);
    }

    free(conv->outputs);
}

/**
 * @brief Uses gnuplot to plot the training error
 * @param conv Convolution instance
 * @param img_width Width of the image in pixels
 * @param img_height Height of the image in pixels
 * @return zero on success
 */
int conv_plot_history(deeplearn_conv * conv,
                      int img_width, int img_height)
{
    return deeplearn_history_plot(&conv->history,
                                  img_width, img_height);
}

/**
 * @brief Saves the given convolution object to a file
 * @param fp File pointer
 * @param conv Convolution object
 * @return zero value on success
 */
int conv_save(FILE * fp, deeplearn_conv * conv)
{
    if (FLOATWRITE(conv->no_of_layers) == 0)
        return -1;

    COUNTUP(l, conv->no_of_layers) {
        if (INTWRITE(conv->layer[l].width) == 0)
            return -2;
        if (INTWRITE(conv->layer[l].height) == 0)
            return -3;
        if (INTWRITE(conv->layer[l].depth) == 0)
            return -4;
        if (INTWRITE(conv->layer[l].no_of_features) == 0)
            return -5;
        if (INTWRITE(conv->layer[l].feature_width) == 0)
            return -6;
    }

    if (INTWRITE(conv->outputs_width) == 0)
        return -7;
    if (INTWRITE(conv->no_of_outputs) == 0)
        return -8;
    if (INTWRITE(conv->learning_rate) == 0)
        return -9;
    if (INTWRITE(conv->current_layer) == 0)
        return -10;
    if (fwrite(&conv->history, sizeof(deeplearn_history), 1, fp) == 0)
        return -12;

    return 0;
}

/**
 * @brief Loads a convolution object from file
 * @param fp File pointer
 * @param conv Convolution object
 * @return zero value on success
 */
int conv_load(FILE * fp, deeplearn_conv * conv)
{
    if (FLOATREAD(conv->no_of_layers) == 0)
        return -1;

    COUNTUP(l, conv->no_of_layers) {
        if (INTREAD(conv->layer[l].width) == 0)
            return -2;
        if (INTREAD(conv->layer[l].height) == 0)
            return -3;
        if (INTREAD(conv->layer[l].depth) == 0)
            return -4;
        if (INTREAD(conv->layer[l].no_of_features) == 0)
            return -5;
        if (INTREAD(conv->layer[l].feature_width) == 0)
            return -6;
    }

    if (INTREAD(conv->outputs_width) == 0)
        return -7;
    if (INTREAD(conv->no_of_outputs) == 0)
        return -8;

    conv_init(conv->no_of_layers,
              conv->layer[0].width, conv->layer[0].height,
              conv->layer[0].depth,
              conv->layer[0].no_of_features,
              conv->layer[0].feature_width,
              conv->outputs_width, conv->outputs_width,
              conv);

    if (INTREAD(conv->learning_rate) == 0)
        return -9;
    if (INTREAD(conv->current_layer) == 0)
        return -10;
    if (fread(&conv->history, sizeof(deeplearn_history), 1, fp) == 0)
        return -12;

    return 0;
}

/**
 * @brief Convolves an input image or layer to an output layer
 * @param img Input image or previous layer with values in the range 0.0 -> 1.0
 * @param img_width Width of the image
 * @param img_height Height of the image
 * @param img_depth Depth of the image. If this is the first layer then it is
 *        the color depth, otherwise it is the number of features learned in
 *        the previous layer
 * @param feature_width Width if each image patch
 * @param no_of_features The number of features in the set
 * @param pooling_factor Pooling factor
 * @param feature Array containing the learned features, having values in
 *        the range 0.0 -> 1.0
 * @param layer The output layer
 * @param layer_width Width of the output layer. The total size of the
 *        output layer should be layer_width*layer_width*no_of_features
 */
void convolve_image(float img[],
                    int img_width, int img_height, int img_depth,
                    int feature_width, int no_of_features,
                    int pooling_factor,
                    float feature[],
                    float layer[], int layer_width)
{
    if (img_depth == 1) {
        convolve_image_mono(img, img_width, img_height,
                            feature_width, no_of_features,
                            pooling_factor,
                            feature, layer, layer_width);
        return;
    }

    int half_feature_width = feature_width/2;
    int unpooled_layer_width = layer_width;

    if (pooling_factor > 1) {
        /* if we are pooling then clear the values within the layer
           which will be updated, so that maximums can be calculated */
        unpooled_layer_width = layer_width*pooling_factor;
        FLOATCLEAR(layer, layer_width*layer_width*no_of_features*img_depth);
    }

    /* for each unit in the output layer */
#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
    COUNTDOWN(layer_y, unpooled_layer_width) {
        int pooled_layer_y = layer_y / pooling_factor;
        int y_img = layer_y * img_height / unpooled_layer_width;
        int ty = y_img - half_feature_width;
        int by = ty + feature_width;
        if (ty < 0) ty = 0;
        if (by >= img_height) by = img_height-1;
        COUNTDOWN(layer_x, unpooled_layer_width) {
            int pooled_layer_x = layer_x / pooling_factor;
            int x_img = layer_x * img_width / unpooled_layer_width;
            int tx = x_img - half_feature_width;
            int bx = tx + feature_width;
            if (tx < 0) tx = 0;
            if (bx >= img_width) bx = img_width-1;

            /* for every learned feature */
            COUNTDOWN(f, no_of_features) {
                float * curr_feature =
                    &feature[f*feature_width*feature_width*img_depth];

                float match[3] = { 0.0f, 0.0f, 0.0f };
                FOR(yy, ty, by) {
                    /* position in the input image */
                    int n0 = ((yy*img_width) + tx) * img_depth;
                    /* position within the feature */
                    int n1 = ((yy-ty) * feature_width) * img_depth;
                    FOR(xx, tx, bx) {
                        COUNTDOWN(d, img_depth) {
                            match[d] +=
                                (img[n0+d] - curr_feature[n1+d]);
                        }
                        n0 += img_depth;
                        n1 += img_depth;
                    }
                }

                if (pooling_factor <= 1) {
                    int layer_unit_index =
                        ((layer_y*unpooled_layer_width) + layer_x)*
                        no_of_features + f;
                    COUNTDOWN(d, img_depth) {
                        layer[(layer_unit_index*img_depth) + d] =
                            AF(match[d]);
                    }
                }
                else {
                    int layer_unit_index =
                        ((pooled_layer_y*layer_width) + pooled_layer_x) *
                        no_of_features + f;
                    COUNTDOWN(d, img_depth) {
                        float v = AF(match[d]);
                        /* max pooling */
                        if (v > layer[(layer_unit_index*img_depth) + d])
                            layer[(layer_unit_index*img_depth) + d] = v;
                    }
                }
            }
        }
    }
}

/**
 * @brief Convolves a mono input image or layer to an output layer
 * @param img Input image or previous layer with values in the range 0.0 -> 1.0
 * @param img_width Width of the image
 * @param img_height Height of the image
 *        the color depth, otherwise it is the number of features learned in
 *        the previous layer
 * @param feature_width Width if each image patch
 * @param no_of_features The number of features in the set
 * @param feature Array containing the learned features, having values in
 *        the range 0.0 -> 1.0
 * @param layer The output layer
 * @param layer_width Width of the output layer. The total size of the
 *        output layer should be layer_width*layer_width*no_of_features
 */
void convolve_image_mono(float img[],
                         int img_width, int img_height,
                         int feature_width, int no_of_features,
                         int pooling_factor,
                         float feature[],
                         float layer[], int layer_width)
{
    int half_feature_width = feature_width/2;
    int unpooled_layer_width = layer_width;

    if (pooling_factor > 1) {
        /* if we are pooling then clear the values within the layer
           which will be updated, so that maximums can be calculated */
        unpooled_layer_width = layer_width*pooling_factor;
        FLOATCLEAR(layer, layer_width*layer_width*no_of_features);
    }

    /* for each unit in the output layer */
#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
    COUNTDOWN(layer_y, unpooled_layer_width) {
        int pooled_layer_y = layer_y / pooling_factor;
        int y_img = layer_y * img_height / unpooled_layer_width;
        int ty = y_img - half_feature_width;
        int by = ty + feature_width;
        if (ty < 0) ty = 0;
        if (by >= img_height) by = img_height-1;
        COUNTDOWN(layer_x, unpooled_layer_width) {
            int pooled_layer_x = layer_x / pooling_factor;
            int x_img = layer_x * img_width / unpooled_layer_width;
            int tx = x_img - half_feature_width;
            int bx = tx + feature_width;
            if (tx < 0) tx = 0;
            if (bx >= img_width) bx = img_width-1;

            /* for every learned feature */
            COUNTDOWN(f, no_of_features) {
                float * curr_feature =
                    &feature[f*feature_width*feature_width];

                float match = 0.0f;
                FOR(yy, ty, by) {
                    /* position in the input image */
                    int n0 = (yy*img_width) + tx;
                    /* position within the feature */
                    int n1 = (yy-ty) * feature_width;
                    FOR(xx, tx, bx) {
                        match += (img[n0] - curr_feature[n1]);
                        n0++;
                        n1++;
                    }
                }

                if (pooling_factor <= 1) {
                    int layer_unit_index =
                        ((layer_y*layer_width) + layer_x)*no_of_features + f;
                    layer[layer_unit_index] = AF(match);
                }
                else {
                    int layer_unit_index =
                        ((pooled_layer_y*layer_width) +
                         pooled_layer_x)*no_of_features + f;
                    /* max pooling */
                    if (AF(match) > layer[layer_unit_index])
                        layer[layer_unit_index] = AF(match);
                }
            }
        }
    }
}

/**
 * @brief Deconvolves a layer back to the source image
 * @param img Input image or previous layer with values in the range 0.0 -> 1.0
 * @param img_width Width of the image
 * @param img_height Height of the image
 *        the color depth, otherwise it is the number of features learned in
 *        the previous layer
 * @param feature_width Width if each image patch
 * @param no_of_features The number of features in the set
 * @param feature Array containing the learned features, having values in
 *        the range 0.0 -> 1.0
 * @param layer The output layer to begin from
 * @param layer_width Width of the output layer. The total size of the
 *        output layer should be layer_width*layer_width*no_of_features
 */
void deconvolve_image_mono(float img[],
                           int img_width, int img_height,
                           int feature_width, int no_of_features,
                           float feature[],
                           float layer[], int layer_width)
{
    int half_feature_width = feature_width/2;
    unsigned int * updates_per_pixel;

    UINTALLOC(updates_per_pixel, img_width*img_height);
    if (!updates_per_pixel)
        return;

    UINTCLEAR(updates_per_pixel, img_width*img_height);

    /* clear the input image */
    FLOATCLEAR(img, img_width*img_height);

    /* for each unit in the output layer */
#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
    COUNTDOWN(layer_y, layer_width) {
        int y_img = layer_y * img_height / layer_width;
        int ty = y_img - half_feature_width;
        int by = ty + feature_width;
        if (ty < 0) ty = 0;
        if (by >= img_height) by = img_height-1;
        COUNTDOWN(layer_x, layer_width) {
            int x_img = layer_x * img_width / layer_width;
            int tx = x_img - half_feature_width;
            int bx = tx + feature_width;
            if (tx < 0) tx = 0;
            if (bx >= img_width) bx = img_width-1;

            /* for every learned feature */
            COUNTDOWN(f, no_of_features) {
                float * curr_feature =
                    &feature[f*feature_width*feature_width];

                int layer_unit_index =
                    ((layer_y*layer_width) + layer_x)*no_of_features + f;
                float weight = layer[layer_unit_index];

                FOR(yy, ty, by) {
                    /* position in the input image */
                    int n0 = (yy*img_width) + tx;
                    /* position within the feature */
                    int n1 = (yy-ty) * feature_width;
                    FOR(xx, tx, bx) {
                        img[n0] += weight*curr_feature[n1];
                        updates_per_pixel[n0]++;
                        n0++;
                        n1++;
                    }
                }
            }
        }
    }

    /* divide the values for each pixel by the number of times that
       pixel is touched during convolution */
    COUNTDOWN(i, img_width*img_height) {
        if (updates_per_pixel[i] == 0)
            continue;

        img[i] /= (float)updates_per_pixel[i];
    }

    /* get the range of values */
    int minval = img[0];
    int maxval = img[0];
    COUNTDOWN(i, img_width*img_height) {
        if (img[i] < minval) minval = img[i];
        if (img[i] > maxval) maxval = img[i];
    }

    /* normalise */
    float range = maxval - minval;
    if (range > 0) {
        COUNTDOWN(i, img_width*img_height) {
            img[i] = (img[i] - minval)/range;
        }
    }

    /* fill horizontal gaps */
    COUNTDOWN(y, img_height) {
        int prev_n = -1;
        COUNTDOWN(x, img_width) {
            int n = y * img_width + x;
            if (img[n] > MIN_DECONVOLVED) {
                prev_n = n;
            }
            else {
                if (prev_n > -1)
                    img[n] = img[prev_n];
            }
        }
    }

    /* fill vertical gaps */
    COUNTDOWN(x, img_width) {
        int prev_n = -1;
        COUNTDOWN(y, img_height) {
            int n = y * img_width + x;
            if (img[n] > MIN_DECONVOLVED) {
                prev_n = n;
            }
            else {
                if (prev_n > -1)
                    img[n] = img[prev_n];
            }
        }
    }

    free(updates_per_pixel);
}

/**
 * @brief Deconvolves a layer back to the source image
 * @param img Input image or previous layer with values in the range 0.0 -> 1.0
 * @param img_width Width of the image
 * @param img_height Height of the image
 * @param img_depth Depth of the image. If this is the first layer then it is
 *        the color depth, otherwise it is the number of features learned in
 *        the previous layer
 * @param feature_width Width if each image patch
 * @param no_of_features The number of features in the set
 * @param feature Array containing the learned features, having values in
 *        the range 0.0 -> 1.0
 * @param layer The output layer to start from
 * @param layer_width Width of the output layer. The total size of the
 *        output layer should be layer_width*layer_width*no_of_features
 */
void deconvolve_image(float img[],
                      int img_width, int img_height, int img_depth,
                      int feature_width, int no_of_features,
                      float feature[],
                      float layer[], int layer_width)
{
    if (img_depth == 1) {
        deconvolve_image_mono(img, img_width, img_height,
                              feature_width, no_of_features,
                              feature, layer, layer_width);
        return;
    }

    int half_feature_width = feature_width/2;
    unsigned int * updates_per_pixel;

    UINTALLOC(updates_per_pixel, img_width*img_height);
    if (!updates_per_pixel)
        return;

    UINTCLEAR(updates_per_pixel, img_width*img_height);

    /* clear the input image */
    FLOATCLEAR(img, img_width*img_height*img_depth);

    /* for each unit in the output layer */
#pragma omp parallel for schedule(static) num_threads(DEEPLEARN_THREADS)
    COUNTDOWN(layer_y, layer_width) {
        int y_img = layer_y * img_height / layer_width;
        int ty = y_img - half_feature_width;
        int by = ty + feature_width;
        if (ty < 0) ty = 0;
        if (by >= img_height) by = img_height-1;
        COUNTDOWN(layer_x, layer_width) {
            int x_img = layer_x * img_width / layer_width;
            int tx = x_img - half_feature_width;
            int bx = tx + feature_width;
            if (tx < 0) tx = 0;
            if (bx >= img_width) bx = img_width-1;

            /* for every learned feature */
            COUNTDOWN(f, no_of_features) {
                float * curr_feature =
                    &feature[f*feature_width*feature_width*img_depth];

                int layer_unit_index =
                    ((layer_y*layer_width) + layer_x)*no_of_features + f;

                FOR(yy, ty, by) {
                    /* position in the input image */
                    int n0 = ((yy*img_width) + tx) * img_depth;
                    /* position within the feature */
                    int n1 = ((yy-ty) * feature_width) * img_depth;
                    updates_per_pixel[(yy*img_width) + tx]++;
                    FOR(xx, tx, bx) {
                        COUNTDOWN(d, img_depth) {
                            float weight =
                                layer[(layer_unit_index*img_depth) + d];
                            img[n0+d] += weight*curr_feature[n1+d];
                        }
                        n0 += img_depth;
                        n1 += img_depth;
                    }
                }
            }
        }
    }

    /* divide the values for each pixel by the number of times that
       pixel is touched during convolution */
    COUNTDOWN(i, img_width*img_height) {
        if (updates_per_pixel[i] == 0)
            continue;

        COUNTDOWN(d, img_depth)
            img[i*img_depth + d] /= (float)updates_per_pixel[i];
    }

    /* get the range of values */
    int minval = img[0];
    int maxval = img[0];
    FOR(i, 1, img_width*img_height*img_depth) {
        if (img[i] < minval) minval = img[i];
        if (img[i] > maxval) maxval = img[i];
    }

    /* normalise */
    float range = maxval - minval;
    if (range > 0) {
        COUNTDOWN(i, img_width*img_height*img_depth) {
            img[i] = (img[i] - minval)/range;
        }
    }

    /* fill horizontal gaps */
    COUNTDOWN(y, img_height) {
        int prev_n = -1;
        COUNTDOWN(x, img_width) {
            int n = (y * img_width + x)*img_depth;
            if (img[n] > MIN_DECONVOLVED) {
                prev_n = n;
            }
            else {
                if (prev_n > -1) {
                    COUNTDOWN(d, img_depth)
                        img[n+d] = img[prev_n+d];
                }
            }
        }
    }

    /* fill vertical gaps */
    COUNTDOWN(x, img_width) {
        int prev_n = -1;
        COUNTDOWN(y, img_height) {
            int n = (y * img_width + x)*img_depth;
            if (img[n] > MIN_DECONVOLVED) {
                prev_n = n;
            }
            else {
                if (prev_n > -1) {
                    COUNTDOWN(d, img_depth)
                        img[n+d] = img[prev_n+d];
                }
            }
        }
    }

    free(updates_per_pixel);
}

/**
 * @brief Feed forward to the given layer
 * @param img The input image
 * @param conv Convolution instance
 * @param layer The number of layers to convolve
 */
void conv_feed_forward(unsigned char * img,
                       deeplearn_conv * conv, int layer)
{
    if (!conv->training) {
        /* convert the input image to floats */
        COUNTDOWN(i,
                  conv->layer[0].width*conv->layer[0].height*conv->layer[0].depth) {
            conv->layer[0].layer[i] = (float)img[i]/255.0f;
        }
    }
    else {
        /* if we are still training add noise to the inputs */
        COUNTDOWN(i,
                  conv->layer[0].width*conv->layer[0].height*conv->layer[0].depth) {
            conv->layer[0].layer[i] =
                ((float)img[i]/255.0f) +
                (conv->noise *
                 (((rand_num(&conv->random_seed)%200000)/100000.0f)-1.0f));

            /* limit within range 0.0 -> 1.0 */
            if (conv->layer[0].layer[i] < 0)
                conv->layer[0].layer[i] = 0;
            else if (conv->layer[0].layer[i] > 1)
                conv->layer[0].layer[i] = 1;
        }
    }

    COUNTUP(l, layer) {
        float * next_layer = conv->outputs;
        int next_layer_width = conv->outputs_width;

        if (l < conv->no_of_layers-1) {
            next_layer = conv->layer[l+1].layer;
            next_layer_width = conv->layer[l+1].width;
        }

        convolve_image(conv->layer[l].layer,
                       conv->layer[l].width, conv->layer[l].height,
                       conv->layer[l].depth,
                       conv->layer[l].feature_width,
                       conv->layer[l].no_of_features,
                       conv->layer[l].pooling_factor,
                       conv->layer[l].feature,
                       next_layer, next_layer_width);
    }
}

/**
 * @brief Feed backwards to the input layer and return the image
 * @param img image to reconstruct
 * @param conv Convolution instance
 * @param layer The starting layer to deconvolve from
 */
void conv_feed_backwards(unsigned char img[], deeplearn_conv * conv, int layer)
{
    for (int l = layer; l >= 0; l--) {
        float * next_layer = conv->outputs;
        int next_layer_width = conv->outputs_width;

        if (l < conv->no_of_layers-1) {
            next_layer = conv->layer[l+1].layer;
            next_layer_width = conv->layer[l+1].width;
        }

        deconvolve_image(conv->layer[l].layer,
                         conv->layer[l].width, conv->layer[l].height,
                         conv->layer[l].depth,
                         conv->layer[l].feature_width,
                         conv->layer[l].no_of_features,
                         conv->layer[l].feature,
                         next_layer, next_layer_width);
    }

    /* convert the input image to bytes */
    COUNTDOWN(i,
              conv->layer[0].width*conv->layer[0].height*conv->layer[0].depth)
        img[i] = (unsigned char)(conv->layer[0].layer[i]*255);
}

/**
 * @brief Clears all values in all layers other than the first,
 *        typically for testing purposes
 * @param conv Convolution instance
 */
void conv_clear(deeplearn_conv * conv)
{
    FOR(l, 1, conv->no_of_layers) {
        FLOATCLEAR(conv->layer[l].layer,
                   conv->layer[l].width * conv->layer[l].height *
                   conv->layer[l].depth);
    }
}

/**
 * @brief Learn features
 * @param conv Convolution instance
 * @param samples The number of samples from the image or layer
 * @param layer_itterations The number of training itterations per layer
 * @param random_seed Random number generator seed
 * @returns matching score/error, with lower values being better match
 */
float conv_learn(unsigned char * img,
                 deeplearn_conv * conv,
                 int samples, int layer_itterations,
                 unsigned int * random_seed)
{
    float matching_score = 0;
    float * feature_score;
    int layer = conv->current_layer;

    if (layer >= conv->no_of_layers)
        return 0;

    FLOATALLOC(feature_score, conv->layer[layer].no_of_features);
    if (!feature_score)
        return -1;

    conv_feed_forward(img, conv, layer);

    matching_score +=
        learn_features(&conv->layer[layer].layer[0],
                       conv->layer[layer].width,
                       conv->layer[layer].height,
                       conv->layer[layer].depth,
                       conv->layer[layer].feature_width,
                       conv->layer[layer].no_of_features,
                       &conv->layer[layer].feature[0],
                       feature_score,
                       samples, conv->learning_rate, random_seed);

    /* check for NaN */
    if (matching_score != matching_score) {
        printf("matching_score = %f\n", matching_score);
        free(feature_score);
        return -2;
    }

    deeplearn_history_update(&conv->history, matching_score);

    free(feature_score);

    /* proceed to the next layer if the counter threshold is reached */
    conv->layer[conv->current_layer].ctr++;
    if (conv->layer[conv->current_layer].ctr >= layer_itterations)
        conv->current_layer++;

    /* turn off training flag if we are on the last layer */
    if (conv->current_layer >= conv->no_of_layers)
        conv->training = (1==0);

    return matching_score;
}

/**
 * @brief Draws features for a given convolution layer
 * @param img Image to draw to
 * @param img_width Width of the image
 * @param img_height Height of the image
 * @param img_depth Depth of the image
 * @param layer Index of the layer whose features will be shown
 * @param conv Convolution instance
 * @returns zero on success
 */
int conv_draw_features(unsigned char img[],
                       int img_width, int img_height, int img_depth,
                       int layer,
                       deeplearn_conv * conv)
{
    int feature_width, feature_depth;
    int no_of_features;
    float * feature;

    if ((layer < 0) || (layer >= conv->no_of_layers))
        return -1;

    feature_width = conv->layer[layer].feature_width;
    feature_depth = conv->layer[layer].depth;
    no_of_features = conv->layer[layer].no_of_features;
    feature = conv->layer[layer].feature;

    if (layer == 0)
        return draw_features(img, img_width, img_height, img_depth,
                             feature_depth, feature_width,
                             no_of_features, feature);

    /* TODO: subsequent layers */

    return 0;
}

/**
* @brief Set the inputs of the backprop network from the outputs
*        of a convolutional network
* @param net Backprop neural net object
* @param conv Convolutional network instance
* @returns zero on success
*/
int bp_inputs_from_convnet(bp * net, deeplearn_conv * conv)
{
    /* check that the number of inputs is the same as the
       number of convolution network outputs */
    if(net->no_of_inputs != conv->no_of_outputs)
        return 1;

    /* set the input values within the range NEURON_LOW to NEURON_HIGH */
    COUNTDOWN(i, conv->no_of_outputs)
        bp_set_input(net, i,
                     NEURON_LOW +
                     (conv->outputs[i]*NEURON_RANGE/255.0f));

    return 0;
}

/**
 * @brief Returns the value of an output from the convolutional network
 * @param conv Convolutional network instance
 * @param index Index of the output
 * @returns Output value in the range 0.0 -> 1.0
 */
float conv_get_output(deeplearn_conv * conv, int index)
{
    return conv->layer[conv->no_of_layers-1].layer[index];
}

/**
 * @brief Returns the current training error
 * @param conv Convolutional network instance
 * @returns Training error in the range 0.0 -> 100.0
 */
float conv_get_error(deeplearn_conv * conv)
{
    return conv->history.history[conv->history.index][0];
}
