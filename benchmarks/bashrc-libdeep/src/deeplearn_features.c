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

#include "deeplearn_features.h"

/**
 * @brief Draws the given feature set within the given image
 * @param img The image to be displayed
 * @param img_width Width of the image
 * @param img_height Height of the image
 * @param img_depth Depth of the image
 * @param feature_depth Depth of the features
 * @param feature_width Width of each feature
 * @param no_of_features The number of features in the set
 * @param feature Array storing the features
 * @returns zero on success
 */
int draw_features(unsigned char img[],
                  int img_width, int img_height, int img_depth,
                  int feature_depth, int feature_width,
                  int no_of_features, float feature[])
{
    int grid_dimension = (int)sqrt(no_of_features);

    if (grid_dimension*grid_dimension != no_of_features) {
        printf("Features cannot be displayed as a grid\n");
        return 1;
    }

    /* clear the image */
    memset((void*)img, '\0',
           img_width*img_height*img_depth*sizeof(unsigned char));

    int feature_index = 0;
    COUNTUP(gx, grid_dimension) {
        int tx = gx * img_width / grid_dimension;
        int bx = (gx+1) * img_width / grid_dimension;
        COUNTUP(gy, grid_dimension) {
            int ty = gy * img_height / grid_dimension;
            int by = (gy+1) * img_height / grid_dimension;

            /* get the feature from the array */
            float * curr_feature =
                &feature[feature_index*
                         feature_width*feature_width*feature_depth];

            FOR(yy, ty+1, by-1) {
                int y = (yy - ty) * feature_width / (by - ty);
                FOR(xx, tx+1, bx-1) {
                    int x = (xx - tx) * feature_width / (bx - tx);
                    /* pixel index within the image */
                    int n0 = ((yy*img_width) + xx) * img_depth;
                    /* pixel index within the feature */
                    int n1 = ((y*feature_width) + x) * feature_depth;
                    if (feature_depth == 1) {
                        COUNTDOWN(d, img_depth)
                            img[n0+d] = (unsigned char)(curr_feature[n1]*255);
                    }
                    else {
                        COUNTDOWN(d, img_depth)
                            img[n0+d] = (unsigned char)(curr_feature[n1+d]*255);
                    }
                }
            }
            feature_index++;
        }
    }
    return 0;
}

/**
 * @brief Learns a set of features from a given mono input layer
 *        If the initial layer is an image it should be converted to floats
 *        in the range 0.0 -> 1.0.
 *        We can eliminate many calculations and loops for the mono image case.
 * @param img The image to be learned from with values in the range 0.0 -> 1.0
 * @param img_width Width of the image
 * @param img_height Height of the image
 * @param feature_width Width if each image patch
 * @param no_of_features The number of features to be learned
 * @param feature Array containing the learned features
 * @param feature_score Array used to store feature scores
 * @param samples The number of samples to take from the image
 * @param learning_rate Learning rate in the range 0.0 -> 1.0
 * @param random_seed Random number generator seed
 * @returns Total matching score
 */
static float learn_features_mono(float img[],
                                 int img_width, int img_height,
                                 int feature_width, int no_of_features,
                                 float feature[],
                                 float feature_score[],
                                 int samples,
                                 float learning_rate,
                                 unsigned int * random_seed)
{
    int width = img_width-1-feature_width;
    int height = img_height-1-feature_width;
    float total_match_score = 0;
    const int closest_matches = 3;

    /* sample the image a number of times */
    COUNTDOWN(i, samples) {

        /* top left corner of the image patch */
        int tx = rand_num(random_seed) % width;
        int ty = rand_num(random_seed) % height;

        /* calculate matching scores for each feature for this image patch */
        COUNTDOWN(f, no_of_features) {
            float * curr_feature =
                &feature[f*feature_width*feature_width];

            /* calculate the matching score for this feature */
            feature_score[f] = 0;
            COUNTDOWN(yy, feature_width) {
                int n0 = ((ty + yy)*img_width) + tx;
                int n1 = yy * feature_width;
                COUNTDOWN(xx, feature_width) {
                    feature_score[f] +=
                        (img[n0] - curr_feature[n1])*
                        (img[n0] - curr_feature[n1]);
                    n0++;
                    n1++;
                }
            }

            total_match_score +=
                (float)sqrt(feature_score[f]/
                            (float)(feature_width*feature_width));
        }

        /* get the N closest feature indexes based upon match scores */
        int index[closest_matches];
        COUNTDOWN(fi, closest_matches)
            index[fi] = (int)(rand_num(random_seed) % no_of_features);

        COUNTUP(match, closest_matches) {
            float min = 0;
            float max = 0;
            if (match > 0)
                max = feature_score[index[match-1]];
            COUNTDOWN(f, no_of_features) {
                if ((max == 0) || (feature_score[f] > max)) {
                    if ((min == 0) || (feature_score[f] < min)) {
                        min = feature_score[f];
                        index[match] = f;
                    }
                }
            }
        }

        /* occasionally choose a feature index at random */
        if (rand_num(random_seed) % 32 < 8) {
            index[rand_num(random_seed) % closest_matches] =
                (int)(rand_num(random_seed) % no_of_features);
        }

        /* use any currently unused features */
        int index_ctr = 0;
        COUNTDOWN(f, no_of_features) {
            float * curr_feature =
                &feature[f*feature_width*feature_width];
            if ((curr_feature[0] == 0) && (curr_feature[2] == 0)) {
                index[index_ctr] = f;
                index_ctr++;
                if (index_ctr >= closest_matches) break;
            }
        }

        /* move the closest features towards the image patch */
        COUNTUP(match, closest_matches) {
            int curr_index = index[match];
            float * curr_feature =
                &feature[curr_index*feature_width*feature_width];

            /* incrementally adjust the feature towards the image */
            COUNTDOWN(yy, feature_width) {
                /* pixel index within the image */
                int n0 = ((ty + yy)*img_width) + tx;
                /* pixel index within the feature */
                int n1 = yy * feature_width;
                COUNTDOWN(xx, feature_width) {
                    /* move towards the image patch */
                    curr_feature[n1] +=
                        (img[n0] - curr_feature[n1])*learning_rate;

                    /* move a little more for the top match */
                    if (match == 0)
                        curr_feature[n1] +=
                            (img[n0] - curr_feature[n1])*learning_rate;

                    /* move a little more for the top two */
                    if (match < 2)
                        curr_feature[n1] +=
                            (img[n0] - curr_feature[n1])*learning_rate;

                    /* occasionally modify at random */
                    if (rand_num(random_seed) % 32 < 8) {
                        if (rand_num(random_seed) % 8 < 4)
                            curr_feature[n1] +=
                                (img[n0] -
                                 curr_feature[n1])*learning_rate;
                        else
                            curr_feature[n1] -=
                                (img[n0] -
                                 curr_feature[n1])*learning_rate;

                        curr_feature[n1] = CLIP(curr_feature[n1], 0.0f, 1.0f);
                    }

                    n0++;
                    n1++;
                }

            }
        }
    }

    return total_match_score/(float)samples;
}

/**
 * @brief Learns a set of features from a given input layer
 *        If the initial layer is an image it should be converted to floats
 *        in the range 0.0 -> 1.0
 * @param img The image to be learned from with values in the range 0.0 -> 1.0
 * @param img_width Width of the image
 * @param img_height Height of the image
 * @param img_depth Depth of the image. If this is the first layer then it is
 *        the color depth, otherwise it is the number of features learned in
 *        the previous layer
 * @param feature_width Width if each image patch
 * @param no_of_features The number of features to be learned
 * @param feature Array containing the learned features
 * @param feature_score Array used to store feature scores
 * @param samples The number of samples to take from the image
 * @param learning_rate Learning rate in the range 0.0 -> 1.0
 * @param random_seed Random number generator seed
 * @returns Total matching score
 */
float learn_features(float img[],
                     int img_width, int img_height, int img_depth,
                     int feature_width, int no_of_features,
                     float feature[],
                     float feature_score[],
                     int samples,
                     float learning_rate,
                     unsigned int * random_seed)
{
    int width = img_width-1-feature_width;
    int height = img_height-1-feature_width;
    float total_match_score = 0.0f;
    const int closest_matches = 3;

    /* if this is a mono input image then there are a lot of
       calculations which can be avoided by using a more
       specialised version of this function */

    if (img_depth == 1)
        return learn_features_mono(img, img_width, img_height,
                                   feature_width, no_of_features,
                                   feature, feature_score, samples,
                                   learning_rate, random_seed);

    /* sample the image a number of times */
    COUNTDOWN(i, samples) {

        /* top left corner of the image patch */
        int tx = rand_num(random_seed) % width;
        int ty = rand_num(random_seed) % height;

        /* calculate matching scores for each feature for this image patch */
        COUNTDOWN(f, no_of_features) {
            float * curr_feature =
                &feature[f*feature_width*feature_width*img_depth];

            /* calculate the matching score for this feature */
            feature_score[f] = 0;
            COUNTDOWN(yy, feature_width) {
                int n0 = (((ty + yy)*img_width) + tx) * img_depth;
                int n1 = (yy * feature_width) * img_depth;
                COUNTDOWN(xx, feature_width) {
                    COUNTDOWN(d, img_depth)
                        feature_score[f] +=
                            (img[n0+d] - curr_feature[n1+d])*
                            (img[n0+d] - curr_feature[n1+d]);
                    n0 += img_depth;
                    n1 += img_depth;
                }
            }

            total_match_score +=
                (float)sqrt(feature_score[f]/
                            (float)(feature_width*feature_width*img_depth));
        }

        /* get the N closest feature indexes based upon match scores */
        int index[closest_matches];
        COUNTDOWN(fi, closest_matches)
            index[fi] = (int)(rand_num(random_seed) % no_of_features);

        COUNTUP(match, closest_matches) {
            float min = 0;
            float max = 0;
            if (match > 0)
                max = feature_score[index[match-1]];
            COUNTDOWN(f, no_of_features) {
                if ((max == 0) || (feature_score[f] > max)) {
                    if ((min == 0) || (feature_score[f] < min)) {
                        min = feature_score[f];
                        index[match] = f;
                    }
                }
            }
        }

        /* occasionally choose a feature index at random */
        if (rand_num(random_seed) % 32 < 8) {
            index[rand_num(random_seed) % closest_matches] =
                (int)(rand_num(random_seed) % no_of_features);
        }

        /* use any currently unused features */
        int index_ctr = 0;
        COUNTDOWN(f, no_of_features) {
            float * curr_feature =
                &feature[f*feature_width*feature_width*img_depth];
            if ((curr_feature[0] == 0) && (curr_feature[2] == 0)) {
                index[index_ctr] = f;
                index_ctr++;
                if (index_ctr >= closest_matches) break;
            }
        }

        /* move the closest features towards the image patch */
        COUNTUP(match, closest_matches) {
            int curr_index = index[match];
            float * curr_feature =
                &feature[curr_index*feature_width*feature_width*img_depth];

            /* incrementally adjust the feature towards the image */
            COUNTDOWN(yy, feature_width) {
                /* pixel index within the image */
                int n0 = (((ty + yy)*img_width) + tx) * img_depth;
                /* pixel index within the feature */
                int n1 = (yy * feature_width) * img_depth;
                COUNTDOWN(xx, feature_width) {
                    COUNTDOWN(d, img_depth) {
                        /* move towards the image patch */
                        curr_feature[n1+d] +=
                            (img[n0+d] - curr_feature[n1+d])*learning_rate;

                        /* move a little more for the top match */
                        if (match == 0)
                            curr_feature[n1+d] +=
                                (img[n0+d] - curr_feature[n1+d])*learning_rate;

                        /* move a little more for the top two */
                        if (match < 2)
                            curr_feature[n1+d] +=
                                (img[n0+d] - curr_feature[n1+d])*learning_rate;

                        /* occasionally modify at random */
                        if (rand_num(random_seed) % 32 < 8) {
                            if (rand_num(random_seed) % 8 < 4)
                                curr_feature[n1+d] +=
                                    (img[n0+d] -
                                     curr_feature[n1+d])*learning_rate;
                            else
                                curr_feature[n1+d] -=
                                    (img[n0+d] -
                                     curr_feature[n1+d])*learning_rate;

                            curr_feature[n1+d] =
                                CLIP(curr_feature[n1+d], 0.0f, 1.0f);
                        }
                    }
                    n0 += img_depth;
                    n1 += img_depth;
                }

            }
        }
    }

    return total_match_score/(float)samples;
}
