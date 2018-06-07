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

#include "tests_encoding.h"

static void test_encode_text()
{
    printf("test_encode_text...");

    char * text1 = "one";
    char * text2 = "two";
    char * text1_binary = "111101100111011010100110--------";
    char * text2_binary = "001011101110111011110110--------";
    bp_neuron ** inputs;
    int no_of_inputs = CHAR_BITS*8;
    int i, offset = 0, new_offset;
    int max_field_length_chars = 4;

    inputs = (bp_neuron**)malloc(no_of_inputs*sizeof(bp_neuron*));
    assert(inputs);
    for (i = 0; i < no_of_inputs; i++) {
        inputs[i] = (bp_neuron*)malloc(sizeof(bp_neuron));
        assert(inputs[i]);
    }

    new_offset = enc_text_to_binary(text1,
                                    inputs, no_of_inputs,
                                    offset,
                                    max_field_length_chars);
    assert(new_offset == 4*CHAR_BITS);
    offset = new_offset;

    new_offset = enc_text_to_binary(text1,
                                    inputs, no_of_inputs,
                                    offset,
                                    max_field_length_chars);
    assert(new_offset == 8*CHAR_BITS);

    /* check that the two encodings were the same */
    for (i = 0; i < 3*(int)CHAR_BITS; i++) {
        assert(inputs[i]->value == inputs[i+(4*(int)CHAR_BITS)]->value);
    }

    /* check the expected binary */
    for (i = 0; i < 4*(int)CHAR_BITS; i++) {
        if (inputs[i]->value < 0.4f) {
            assert(text1_binary[i] == '0');
        }
        if (inputs[i]->value > 0.6f) {
            assert(text1_binary[i] == '1');
        }
        if ((inputs[i]->value > 0.4f) &&
            (inputs[i]->value < 0.6f)) {
            assert(text1_binary[i] == '-');
        }
    }

    offset = 0;
    new_offset = enc_text_to_binary(text2,
                                    inputs, no_of_inputs,
                                    offset,
                                    max_field_length_chars);
    assert(new_offset == 4*CHAR_BITS);

    /* check the expected binary */
    for (i = 0; i < 3*(int)CHAR_BITS; i++) {
        if (inputs[i]->value < 0.4f) {
            assert(text2_binary[i] == '0');
        }
        if (inputs[i]->value > 0.6f) {
            assert(text2_binary[i] == '1');
        }
        if ((inputs[i]->value > 0.4f) &&
            (inputs[i]->value < 0.6f)) {
            assert(text2_binary[i] == '-');
        }
    }

    /*
    printf("\n");
    for (i = 0; i < 3*(int)CHAR_BITS; i++) {
        if (inputs[i]->value < 0.5f) {
            printf("0");
        }
        else {
            printf("1");
        }
    }
    printf("\n");
    */

    /* free memory */
    for (i = 0; i < no_of_inputs; i++) {
        free(inputs[i]);
    }
    free(inputs);

    printf("Ok\n");
}

int run_tests_encoding()
{
    printf("\nRunning encoding tests\n");

    test_encode_text();

    printf("All encoding tests completed\n");
    return 0;
}
