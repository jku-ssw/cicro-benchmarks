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

#ifndef DEEPLEARN_HISTORY_H
#define DEEPLEARN_HISTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "globals.h"
#include "phosphene.h"

#define HISTORY_DIMENSIONS 16

/* types of plot */
enum {
    PLOT_STANDARD = 0,
    PLOT_RUNNING_AVERAGE
};

typedef struct {
    /* training itterations elapsed */
    unsigned int itterations;

    /* training history */
    unsigned int interval;
    char filename[256];
    char title[256];
    char label_horizontal[256];
    char label_vertical[256];
    char no_of_points;

    float history[DEEPLEARN_HISTORY_SIZE][HISTORY_DIMENSIONS];
    int index, ctr, step;
} deeplearn_history;

void deeplearn_history_init(deeplearn_history * history,
                            char filename[], char title[],
                            char label_horizontal[], char label_vertical[]);
void deeplearn_history_update(deeplearn_history * history, float value);
void deeplearn_history_update_from_array(deeplearn_history * history,
                                         float value[], int plot_type);
int deeplearn_history_gnuplot(deeplearn_history * history,
                              int img_width, int img_height);
int deeplearn_history_phosphene(deeplearn_history * history,
                                int img_width, int img_height);
int deeplearn_history_plot(deeplearn_history * history,
                           int img_width, int img_height);

#endif
