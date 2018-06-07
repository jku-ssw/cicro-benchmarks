/*
 libdeep - a library for deep learning
 Copyright (C) 2013,2015-2017  Bob Mottram <bob@freedombone.net>

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tests_random.h"
#include "tests_backprop.h"
#include "tests_deeplearn.h"
#include "tests_data.h"
#include "tests_images.h"
#include "tests_encoding.h"
#include "tests_features.h"
#include "tests_conv.h"
#include "tests_deepconvnet.h"
#include "tests_autocoder.h"

int main(int argc, char* argv[])
{
    system("rm training.png");

    run_tests_autocoder();
    run_tests_backprop();
    run_tests_images();
    run_tests_random();
    run_tests_deeplearn();
    run_tests_data();
    run_tests_encoding();
    run_tests_features();
    run_tests_conv();
    run_tests_deepconvnet();

    printf("\nAll tests completed\n");

    return 0;
}
