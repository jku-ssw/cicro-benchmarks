/*
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

#include "tests_data.h"

static void test_data_add()
{
    deeplearn learner;
    int no_of_inputs=10;
    int no_of_hiddens=4;
    int hidden_layers=2;
    int no_of_outputs=2;
    float error_threshold[] = { 0.01f, 0.01f, 0.01f };
    unsigned int random_seed = 123;
    char ** inputs_text = 0;

    printf("test_data_add...");

    /* create the learner */
    deeplearn_init(&learner,
                   no_of_inputs, no_of_hiddens,
                   hidden_layers,
                   no_of_outputs,
                   error_threshold,
                   &random_seed);

    assert((&learner)->net!=0);
    assert((&learner)->autocoder!=0);

    /* add some data samples */
    for (int i = 0; i < 5; i++) {
        float * inputs = (float*)malloc(no_of_inputs*sizeof(float));
        float * outputs = (float*)malloc(no_of_outputs*sizeof(float));
        for (int j = 0; j < no_of_inputs; j++) {
            inputs[j] = i;
        }
        for (int j = 0; j < no_of_outputs; j++) {
            outputs[j] = j;
        }
        assert(deeplearndata_add(&learner.data,
                                 &learner.data_samples,
                                 inputs, inputs_text,
                                 outputs, no_of_inputs, no_of_outputs,
                                 learner.input_range_min,
                                 learner.input_range_max,
                                 learner.output_range_min,
                                 learner.output_range_max) == 0);
        free(inputs);
        free(outputs);
    }
    assert(learner.data_samples == 5);
    assert(learner.indexed_data_samples == 0);

    assert(deeplearndata_index_data(
            learner.data,
            learner.data_samples,
            &learner.indexed_data,
            &learner.indexed_data_samples) == 0);
    assert(learner.indexed_data_samples == 5);

    /* check that the samples have the expected values.
       Note that the sequence is reversed */
    for (int i = 0; i < 5; i++) {
        deeplearndata * sample = deeplearndata_get(&learner, 4-i);
        assert(sample != 0);
        if ((int)sample->inputs[0] != i) {
          printf("(int)sample->inputs[0] %d != %d\n",
                 (int)sample->inputs[0], i);
        }
        assert((int)sample->inputs[0] == i);
        for (int j = 0; j < no_of_outputs; j++) {
            assert((int)sample->outputs[j] == j);
        }
    }

    /* free memory */
    deeplearn_free(&learner);

    printf("Ok\n");
}

static void test_data_training_test()
{
    deeplearn learner;
    int no_of_inputs=10;
    int no_of_hiddens=4;
    int hidden_layers=2;
    int no_of_outputs=2;
    float error_threshold[] = { 0.01f, 0.01f, 0.01f };
    unsigned int random_seed = 123;
    char ** inputs_text = 0;

    printf("test_data_training_test...");

    /* create the learner */
    deeplearn_init(&learner,
                   no_of_inputs, no_of_hiddens,
                   hidden_layers,
                   no_of_outputs,
                   error_threshold,
                   &random_seed);

    assert((&learner)->net!=0);
    assert((&learner)->autocoder!=0);

    /* add some data samples */
    for (int i = 0; i < 100; i++) {
        float * inputs = (float*)malloc(no_of_inputs*sizeof(float));
        float * outputs = (float*)malloc(no_of_outputs*sizeof(float));
        for (int j = 0; j < no_of_inputs; j++) {
            inputs[j] = i;
        }
        if ((i != 54) && (i != 79) && (i != 23)) {
            /* labeled sample */
            for (int j = 0; j < no_of_outputs; j++) {
                outputs[j] = j;
            }
        }
        else {
            /* unlabeled sample */
            for (int j = 0; j < no_of_outputs; j++) {
                outputs[j] = DEEPLEARN_UNKNOWN_VALUE;
            }
        }
        assert(deeplearndata_add(&learner.data,
                                 &learner.data_samples,
                                 inputs, inputs_text, outputs,
                                 no_of_inputs, no_of_outputs,
                                 learner.input_range_min,
                                 learner.input_range_max,
                                 learner.output_range_min,
                                 learner.output_range_max) == 0);
        free(inputs);
        free(outputs);
    }
    assert(learner.data_samples == 100);
    assert(learner.indexed_data_samples == 0);

    assert(deeplearndata_index_data(
            learner.data,
            learner.data_samples,
            &learner.indexed_data,
            &learner.indexed_data_samples) == 0);
    assert(learner.indexed_data_samples == 100);

    assert(learner.training_data_samples == 0);
    assert(learner.training_data_labeled_samples == 0);
    assert(learner.test_data_samples == 0);
    assert(learner.indexed_training_data_samples == 0);
    assert(learner.indexed_training_data_labeled_samples == 0);
    assert(learner.indexed_test_data_samples == 0);
    assert(deeplearndata_create_datasets(&learner, 20) == 0);
    assert(learner.training_data_samples == 80);
    assert(learner.training_data_labeled_samples == 77);
    assert(learner.test_data_samples == 20);
    assert(learner.indexed_training_data_samples == 80);
    assert(learner.indexed_training_data_labeled_samples == 77);
    assert(learner.indexed_test_data_samples == 20);

    /* check that all test samples are labeled */
    for (int i = 0; i < learner.test_data_samples; i++) {
        deeplearndata * test_sample_labeled =
            deeplearndata_get_test(&learner, i);
        assert(test_sample_labeled->labeled == 1);
    }

    /* check that all labeled training samples are labeled */
    for (int i = 0; i < learner.training_data_labeled_samples; i++) {
        deeplearndata * training_sample_labeled2 =
            deeplearndata_get_training_labeled(&learner, i);
        assert(training_sample_labeled2->labeled == 1);
    }

    deeplearndata * training_sample =
      deeplearndata_get_training(&learner, 24);
    assert(training_sample != 0);
    for (int j = 0; j < no_of_outputs; j++) {
        assert(training_sample->outputs[j] == j);
    }
    deeplearndata * training_sample_labeled =
      deeplearndata_get_training_labeled(&learner, 22);
    assert(training_sample_labeled != 0);
    for (int j = 0; j < no_of_outputs; j++) {
        assert(training_sample_labeled->outputs[j] == j);
    }
    deeplearndata * test_sample =
      deeplearndata_get_test(&learner, 11);
    assert(test_sample != 0);
    for (int j = 0; j < no_of_outputs; j++) {
        assert(test_sample->outputs[j] == j);
    }

    /* free memory */
    deeplearn_free(&learner);

    printf("Ok\n");
}

int run_tests_data()
{
    printf("\nRunning data tests\n");

    test_data_add();
    test_data_training_test();

    printf("All data tests completed\n");
    return 0;
}
