/*
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

#include "tests_deeplearn.h"

static void test_string_ends_with_extension()
{
    printf("test_string_ends_with_extension...");

    assert(string_ends_with_extension("test.png", "png"));
    assert(string_ends_with_extension("test.wibble.png", "png"));
    assert(!string_ends_with_extension("testpng", "png"));
    assert(!string_ends_with_extension("test.png", "jpg"));
    assert(!string_ends_with_extension("test.png.wibble", "png"));

    printf("Ok\n");
}

static void test_deeplearn_init()
{
    deeplearn learner;
    int no_of_inputs=10;
    int no_of_hiddens=16;
    int hidden_layers=2;
    int no_of_outputs=2;
    float error_threshold[] = { 0.01f, 0.01f, 0.01f };
    unsigned int random_seed = 123;

    printf("test_deeplearn_init...");

    /* create the learner */
    assert(deeplearn_init(&learner,
                          no_of_inputs, no_of_hiddens,
                          hidden_layers,
                          no_of_outputs,
                          error_threshold,
                          &random_seed) == 0);

    assert((&learner)->net!=0);
    assert((&learner)->autocoder!=0);

    assert((&learner)->autocoder[0]->no_of_inputs==no_of_inputs);
    assert((&learner)->autocoder[0]->no_of_hiddens==no_of_hiddens);

    assert((&learner)->autocoder[1]->no_of_inputs==no_of_hiddens);
    assert((&learner)->autocoder[1]->no_of_hiddens==9);

    /* free memory */
    deeplearn_free(&learner);

    printf("Ok\n");
}

static void test_deeplearn_update()
{
    deeplearn learner, learner2;
    int no_of_inputs=10;
    int no_of_hiddens=4;
    int hidden_layers=2;
    int no_of_outputs=2;
    float error_threshold[] = { 0.2f, 4.2f, 0.2f };
    int itt,i,retval;
    unsigned int random_seed = 123;
    float v,diff;
    int itterations[3];
    char filename[256];
    FILE * fp;

    printf("test_deeplearn_update...");

    itterations[0] = 0;
    itterations[1] = 0;
    itterations[2] = 0;

    /* create the learner */
    assert(deeplearn_init(&learner,
                          no_of_inputs, no_of_hiddens,
                          hidden_layers,
                          no_of_outputs,
                          error_threshold,
                          &random_seed) == 0);

    assert((&learner)->net!=0);
    assert((&learner)->autocoder!=0);

    /* perform pre-training with an autocoder */
    for (itt = 0; itt < 10000; itt++) {
        for (i = 0; i < no_of_inputs; i++) {
            deeplearn_set_input(&learner,i,
                                NEURON_LOW + (i*NEURON_RANGE/(float)no_of_inputs));
        }
        deeplearn_update(&learner);

        itterations[learner.current_hidden_layer]++;

        if (learner.current_hidden_layer==hidden_layers) {
            break;
        }
    }

    if (learner.current_hidden_layer < hidden_layers) {
        printf("\nDidn't finish training (at layer %d)\nbackprop_error %.5f\n",
               learner.current_hidden_layer,learner.backprop_error);
    }
    assert(learner.current_hidden_layer >= hidden_layers);

    /* we expect that there will be some non-zero error */
    assert(learner.backprop_error!=0);

    /* test that it took some itterations to train */
    assert(itterations[0] > 4);
    assert(itterations[1] > 4);

    /* perform the final training between the last
       hidden layer and the outputs */
    for (itt = 0; itt < 10000; itt++) {
        for (i = 0; i < no_of_inputs; i++) {
            deeplearn_set_input(&learner,i,i/(float)no_of_inputs);
        }
        for (i = 0; i < no_of_outputs; i++) {
            deeplearn_set_output(&learner,i,
                                 1.0f - (i/(float)no_of_inputs));
        }
        deeplearn_update(&learner);

        itterations[learner.current_hidden_layer]++;
    }

    /* test that it took some itterations to
       do the final training */
    assert(itterations[2] > 4);

    /* we expect that there will be some non-zero error */
    assert(learner.backprop_error!=0);

    /* check that there is some variation in the outputs */
    v = deeplearn_get_output(&learner,0);
    for (i = 1; i < no_of_outputs; i++) {
        diff = fabs(v - deeplearn_get_output(&learner,i));
        assert(diff > 0);
    }

    for (i = 0; i < no_of_inputs; i++) {
        learner.input_range_min[0] = i+100;
        learner.input_range_max[0] = i+109;
    }
    for (i = 0; i < no_of_outputs; i++) {
        learner.output_range_min[0] = i+1;
        learner.output_range_max[0] = i+5;
    }

    sprintf(filename,"%stemp_deep.dat",DEEPLEARN_TEMP_DIRECTORY);

    /* save the first learner */
    fp = fopen(filename,"wb");
    assert(fp!=0);
    deeplearn_save(fp, &learner);
    fclose(fp);

    /* load into the second learner */
    fp = fopen(filename,"rb");
    assert(fp!=0);
    deeplearn_load(fp, &learner2);
    fclose(fp);

    /* compare the two */
    retval = deeplearn_compare(&learner, &learner2);
    if (retval<1) {
        printf("\nretval = %d\n",retval);
    }
    assert(retval==1);

    /* save a graph */
    sprintf(&learner.history.filename[0], "%stemp_graph.png",
            DEEPLEARN_TEMP_DIRECTORY);
    deeplearn_plot_history(&learner, 1024, 480);

    /* free memory */
    deeplearn_free(&learner);
    deeplearn_free(&learner2);

    printf("Ok\n");
}

static void test_deeplearn_save_load()
{
    deeplearn learner1, learner2;
    int no_of_inputs=10;
    int no_of_hiddens=4;
    int no_of_outputs=3;
    int hidden_layers=3;
    float error_threshold[] = { 0.01f, 0.01f, 0.01f, 0.01f };
    int retval;
    unsigned int random_seed = 123;
    char filename[256];
    FILE * fp;

    printf("test_deeplearn_save_load...");

    /* create network */
    assert(deeplearn_init(&learner1,
                          no_of_inputs, no_of_hiddens,
                          hidden_layers, no_of_outputs,
                          error_threshold,
                          &random_seed) == 0);

    sprintf(filename,"%stemp_deep.dat",DEEPLEARN_TEMP_DIRECTORY);

    /* save the first learner */
    fp = fopen(filename,"wb");
    assert(fp!=0);
    deeplearn_save(fp, &learner1);
    fclose(fp);

    /* load into the second learner */
    fp = fopen(filename,"rb");
    assert(fp!=0);
    deeplearn_load(fp, &learner2);
    fclose(fp);

    /* compare the two */
    retval = deeplearn_compare(&learner1, &learner2);
    if (retval<1) {
        printf("\ncompare retval = %d\n",retval);
    }
    assert(retval==1);

    /* free memory */
    deeplearn_free(&learner1);
    deeplearn_free(&learner2);

    printf("Ok\n");
}

static void test_deeplearn_export()
{
    char * filename1 = "/tmp/libdeep_export.c";
    char * filename2 = "/tmp/libdeep_export.py";
    deeplearn learner;
    int no_of_inputs=10;
    int no_of_hiddens=4;
    int hidden_layers=2;
    int no_of_outputs=2;
    float error_threshold[] = { 0.01f, 0.01f, 0.01f };
    unsigned int random_seed = 123;
    FILE * fp;

    printf("test_deeplearn_export...");

    /* create the learner */
    assert(deeplearn_init(&learner,
                          no_of_inputs, no_of_hiddens,
                          hidden_layers,
                          no_of_outputs,
                          error_threshold,
                          &random_seed) == 0);

    assert((&learner)->net!=0);
    assert((&learner)->autocoder!=0);

    assert(deeplearn_export(&learner, filename1) == 0);
    fp = fopen(filename1,"r");
    assert(fp);
    fclose(fp);

    assert(deeplearn_export(&learner, filename2) == 0);
    fp = fopen(filename2,"r");
    assert(fp);
    fclose(fp);

    /* free memory */
    deeplearn_free(&learner);

    printf("Ok\n");
}

static void test_deeplearn_csv_with_text()
{
    deeplearn learner;
    int no_of_hiddens=16;
    int hidden_layers=3;
    int no_of_outputs = 1;
    int output_field_index[] = { 2 };
    float error_threshold_percent[] = { 1.6f, 1.6f, 3.0f, 3.0f };
    unsigned int random_seed = 123;
    char * csv_filename = "/tmp/libdeep.csv";
    char * export_filename1 = "/tmp/libdeep_text.c";
    char * export_filename2 = "/tmp/libdeep_text.py";
    FILE * fp;

    printf("test_deeplearn_csv_with_text...");

    /* create a csv file */
    fp = fopen(csv_filename,"w");
    assert(fp);
    fprintf(fp,"%f,%s,%f,%f\n",4.2,"one",62.1,1.0);
    fprintf(fp,"%f,%s,%f,%f\n",8.1,"two",57.6,2.0);
    fprintf(fp,"%f,%s,%f,%f\n",9.4,"three",63.2,3.0);
    fprintf(fp,"%f,%s,%f,%f\n",1.7,"four",68.3,4.0);
    fprintf(fp,"%f,%s,%f,%f\n",5.4,"five",91.9,5.0);
    fprintf(fp,"%f,%s,%f,%f\n",7.5,"six",88.7,6.0);
    fprintf(fp,"%f,%s,%f,%f\n",8.6,"seven",83.1,7.0);
    fprintf(fp,"%f,%s,%f,%f\n",6.9,"eight",77.4,8.0);
    fclose(fp);

    /* load the data */
    deeplearndata_read_csv(csv_filename,
                           &learner,
                           no_of_hiddens, hidden_layers,
                           no_of_outputs,
                           output_field_index, 0,
                           error_threshold_percent,
                           &random_seed);

    assert(learner.training_data_samples == 6);
    assert(learner.indexed_training_data_samples == 6);
    assert(learner.training_data_labeled_samples == 6);
    assert(learner.indexed_training_data_labeled_samples == 6);
    assert(learner.test_data_samples == 2);
    assert(learner.indexed_test_data_samples == 2);
    assert(learner.net->no_of_inputs == 2 + (5*CHAR_BITS));
    assert(learner.no_of_input_fields == 3);

    assert(learner.field_length != 0);
    assert(learner.field_length[0] == 0);
    assert(learner.field_length[1] == 5*CHAR_BITS);
    assert(learner.field_length[2] == 0);

    assert(deeplearn_export(&learner, export_filename1) == 0);
    fp = fopen(export_filename1,"r");
    assert(fp);
    fclose(fp);

    assert(deeplearn_export(&learner, export_filename2) == 0);
    fp = fopen(export_filename2,"r");
    assert(fp);
    fclose(fp);

    /* free memory */
    deeplearn_free(&learner);

    printf("Ok\n");
}

static void test_deeplearn_csv_numeric()
{
    deeplearn learner;
    int no_of_hiddens=16;
    int hidden_layers=3;
    int no_of_outputs = 1;
    int output_field_index[] = { 3 };
    float error_threshold_percent[] = { 1.6f, 1.6f, 3.0f, 3.0f };
    unsigned int random_seed = 123;
    char * csv_filename = "/tmp/libdeep.csv";
    FILE * fp;

    printf("test_deeplearn_csv_numeric...");

    /* create a csv file */
    fp = fopen(csv_filename,"w");
    assert(fp);
    fprintf(fp,"%f,%f,%f,%f\n",4.2,6.8,62.1,1.0);
    fprintf(fp,"%f,%f,%f,%f\n",8.1,7.2,57.6,2.0);
    fprintf(fp,"%f,%f,%f,%f\n",9.4,8.24,63.2,3.0);
    fprintf(fp,"%f,%f,%f,%f\n",1.7,3.83,68.3,4.0);
    fprintf(fp,"%f,%f,%f,%f\n",5.4,2.63,91.9,5.0);
    fprintf(fp,"%f,%f,%f,%f\n",7.5,8.24,88.7,6.0);
    fprintf(fp,"%f,%f,%f,%f\n",8.6,7.13,83.1,7.0);
    fprintf(fp,"%f,%f,%f,%f\n",6.9,72.7,77.4,8.0);
    fclose(fp);

    /* load the data */
    deeplearndata_read_csv(csv_filename,
                           &learner,
                           no_of_hiddens, hidden_layers,
                           no_of_outputs,
                           output_field_index, 0,
                           error_threshold_percent,
                           &random_seed);

    assert(learner.training_data_samples == 6);
    assert(learner.indexed_training_data_samples == 6);
    assert(learner.training_data_labeled_samples == 6);
    assert(learner.indexed_training_data_labeled_samples == 6);
    assert(learner.test_data_samples == 2);
    assert(learner.indexed_test_data_samples == 2);
    assert(learner.net->no_of_inputs == 3);
    assert(learner.no_of_input_fields == 3);

    assert(learner.field_length != 0);
    assert(learner.field_length[0] == 0);
    assert(learner.field_length[1] == 0);
    assert(learner.field_length[2] == 0);

    /* free memory */
    deeplearn_free(&learner);

    printf("Ok\n");
}

static void test_deeplearn_set_input_field_text()
{
    deeplearn learner;
    int i, retval, no_of_hiddens=5;
    int hidden_layers=1;
    int no_of_outputs = 1;
    int output_field_index[] = { 2 };
    float error_threshold_percent[] = { 0.07f, 0.07f };
    unsigned int random_seed = 123;
    char * csv_filename = "/tmp/libdeep.csv";
    FILE * fp;
    char * expected_inputs = "111101100111011010100110--------";

    printf("test_deeplearn_set_input_field_text...");

    /* create a csv file */
    fp = fopen(csv_filename,"w");
    assert(fp);
    for (i = 0; i < 10; i++) {
        fprintf(fp,"%s,%s,%f\n","zero","zero",0.0);
        fprintf(fp,"%s,%s,%f\n","zero","one",1.0);
        fprintf(fp,"%s,%s,%f\n","one","zero",1.0);
        fprintf(fp,"%s,%s,%f\n","one","one",0.0);
    }
    fclose(fp);

    /* load the data */
    deeplearndata_read_csv(csv_filename,
                           &learner,
                           no_of_hiddens, hidden_layers,
                           no_of_outputs,
                           output_field_index, 0,
                           error_threshold_percent,
                           &random_seed);

    assert(learner.net->no_of_inputs == 2*4*CHAR_BITS);
    assert(learner.no_of_input_fields == 2);

    assert(learner.field_length != 0);
    assert(learner.field_length[0] == 4*CHAR_BITS);
    assert(learner.field_length[1] == 4*CHAR_BITS);

    /* clear the inputs */
    for (i = 0; i < learner.net->no_of_inputs; i++) {
        deeplearn_set_input(&learner, i, 0.0f);
    }

    retval = deeplearn_set_input_field_text(&learner, 0, "one");
    assert(retval==0);
    for (i = 0; i < 4*(int)CHAR_BITS; i++) {
        if (learner.net->inputs[i]->value > 0.6f) {
            assert(expected_inputs[i] == '1');
        }
        if (learner.net->inputs[i]->value < 0.4f) {
            assert(expected_inputs[i] == '0');
        }
        if ((learner.net->inputs[i]->value > 0.4f) &&
            (learner.net->inputs[i]->value < 0.6f)) {
            assert(expected_inputs[i] == '-');
        }
    }
    /*
    printf("\n");
    for (i = 0; i < learner.net->no_of_inputs; i++) {
        if (learner.net->inputs[i]->value > 0.6f) {
            printf("1");
        }
        if (learner.net->inputs[i]->value < 0.4f) {
            printf("0");
        }
        if ((learner.net->inputs[i]->value > 0.4f) &&
            (learner.net->inputs[i]->value < 0.6f)) {
            printf("-");
        }
    }
    printf("\n");
    */

    /* free memory */
    deeplearn_free(&learner);

    printf("Ok\n");
}

int run_tests_deeplearn()
{
    printf("\nRunning deeplearn tests\n");

    test_string_ends_with_extension();
    test_deeplearn_init();
    test_deeplearn_save_load();
    test_deeplearn_update();
    test_deeplearn_export();
    test_deeplearn_csv_with_text();
    test_deeplearn_csv_numeric();
    test_deeplearn_set_input_field_text();

    printf("All deeplearn tests completed\n");
    return 0;
}
