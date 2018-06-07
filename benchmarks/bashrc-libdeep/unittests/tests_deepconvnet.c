/*
  Copyright (C) 2017  Bob Mottram <bob@freedombone.net>

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

#include "tests_deepconvnet.h"

static void test_deepconvnet_init()
{
    /* convolution network */
    int no_of_convolutions = 3;
    int image_width = 64;
    int image_height = 64;
    int image_depth = 1;
    int max_features = 4*4;
    int feature_width = 4;
    int final_image_width = 4;
    int final_image_height = 4;
    float error_threshold[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    unsigned int random_seed = 123;
    unsigned int layer_itterations = 1000;

    /* deep learner */
    int no_of_deep_layers = 3;
    int no_of_outputs = 2;

    deepconvnet convnet;

    printf("test_deepconvnet_init...");

    assert(deepconvnet_init(no_of_convolutions,
                            no_of_deep_layers,
                            image_width,
                            image_height,
                            image_depth,
                            max_features,
                            feature_width,
                            final_image_width,
                            final_image_height,
                            layer_itterations,
                            no_of_outputs,
                            &convnet,
                            error_threshold,
                            &random_seed) == 0);


    deepconvnet_free(&convnet);

    printf("Ok\n");
}

int run_tests_deepconvnet()
{
    printf("\nRunning deepconvnet tests\n");

    test_deepconvnet_init();

    printf("All deepconvnet tests completed\n");
    return 0;
}
