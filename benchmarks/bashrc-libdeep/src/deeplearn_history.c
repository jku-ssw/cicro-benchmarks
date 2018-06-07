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

#include "deeplearn_history.h"

/**
 * @brief Initialise a structure containing training history
 * @param history History instance
 * @param filename The image filename to save the history plot
 * @param title Title of the history plot
 * @param label_horizontal The text used on the horizontal axis
 * @param label_vertical The text used on the vertical axis
 */
void deeplearn_history_init(deeplearn_history * history,
                            char filename[], char title[],
                            char label_horizontal[],
                            char label_vertical[])
{
    history->itterations = 0;
    history->ctr = 0;
    history->index = 0;
    history->step = 1;
    history->interval = 10;
    history->no_of_points = 0;

    sprintf(history->filename,"%s", filename);
    sprintf(history->title,"%s", title);
    sprintf(history->label_horizontal, "%s", label_horizontal);
    sprintf(history->label_vertical, "%s", label_vertical);

    COUNTDOWN(t, DEEPLEARN_HISTORY_SIZE)
        memset((void*)&history->history[0], '\0',
               sizeof(float)*HISTORY_DIMENSIONS);
}

/**
 * @brief Update the history of scores during feature learning
 * @param history History instance
 * @param value The value to be logged
 */
void deeplearn_history_update(deeplearn_history * history, float value)
{
    history->itterations++;

    if (history->step == 0) return;

    history->ctr++;
    if (history->ctr >= history->step) {
        if (value == DEEPLEARN_UNKNOWN_ERROR)
            value = 0;

        history->history[history->index][0] = value;

        history->index++;
        history->ctr = 0;

        if (history->index >= DEEPLEARN_HISTORY_SIZE) {
            COUNTUP(i, history->index)
                memcpy((void*)&history->history[i/2][0],
                       (void*)&history->history[i][0],
                       sizeof(float)*HISTORY_DIMENSIONS);

            history->index /= 2;
            history->step *= 2;
        }
    }
}

/**
 * @brief Update the history of scores during feature learning
 * @param history History instance
 * @param value The value array to be logged
 * @param plot_type The type of plot
 */
void deeplearn_history_update_from_array(deeplearn_history * history, float value[],
                                         int plot_type)
{
    float prev_value;

    history->itterations++;

    if (history->step == 0) return;

    history->ctr++;
    if (history->ctr >= history->step) {
        if (plot_type == PLOT_RUNNING_AVERAGE) {
            COUNTDOWN(i, HISTORY_DIMENSIONS) {
                if (history->index > 0) {
                    prev_value = history->history[history->index-1][i];
                    history->history[history->index][i] =
                        prev_value + ((value[i] - prev_value)*0.02f);
                }
                else
                    history->history[history->index][i] = value[i];
            }
        }
        else {
            memcpy((void*)&history->history[history->index][0],
                   (void*)&value[0], sizeof(float)*HISTORY_DIMENSIONS);
        }

        history->index++;
        history->ctr = 0;

        if (history->index >= DEEPLEARN_HISTORY_SIZE) {
            COUNTUP(i, history->index)
                memcpy((void*)&history->history[i/2][0],
                       (void*)&history->history[i][0],
                       sizeof(float)*HISTORY_DIMENSIONS);

            history->index /= 2;
            history->step *= 2;
        }
    }
}

/**
 * @brief Uses gnuplot to plot the training error
 * @param history History instance
 * @param img_width Width of the image in pixels
 * @param img_height Height of the image in pixels
 * @return zero on success
 */
int deeplearn_history_gnuplot(deeplearn_history * history,
                              int img_width, int img_height)
{
    int retval=0;
    FILE * fp;
    char data_filename[256];
    char plot_filename[256];
    char command_str[256];
    float value;
    float max_value = 0.01f;
    char * filename = history->filename;
    char * title = history->title;

    if (history->no_of_points > 0)
        return 0;

    if (strlen(filename) == 0)
        return -1;

    if (strlen(title) == 0)
        return -2;

    sprintf(data_filename,"%s%s",DEEPLEARN_TEMP_DIRECTORY,
            "libdeep_data.dat");
    sprintf(plot_filename,"%s%s",DEEPLEARN_TEMP_DIRECTORY,
            "libdeep_data.plot");

    /* save the data */
    fp = fopen(data_filename,"w");

    if (!fp)
        return -3;

    COUNTUP(index, history->index) {
        value = history->history[index][0];
        fprintf(fp,"%d    %.10f\n",
                index*history->step,value);
        /* record the maximum error value */
        if (value > max_value)
            max_value = value;
    }
    fclose(fp);

    /* create a plot file */
    fp = fopen(plot_filename,"w");

    if (!fp)
        return -4;

    fprintf(fp,"%s","reset\n");
    fprintf(fp,"set title \"%s\"\n",title);
    fprintf(fp,"set xrange [0:%d]\n",
            history->index*history->step);
    fprintf(fp,"set yrange [0:%f]\n",max_value*102/100);
    fprintf(fp,"%s","set lmargin 9\n");
    fprintf(fp,"%s","set rmargin 2\n");
    fprintf(fp,"%s","set xlabel \"Time Step\"\n");
    fprintf(fp,"%s","set ylabel \"Training Error %\"\n");

    fprintf(fp,"%s","set grid\n");
    fprintf(fp,"%s","set key right top\n");

    fprintf(fp,"set terminal png size %d,%d\n",
            img_width, img_height);
    fprintf(fp,"set output \"%s\"\n", filename);
    fprintf(fp,"plot \"%s\" using 1:2 notitle with lines\n",
            data_filename);
    fclose(fp);

    /* run gnuplot using the created files */
    sprintf(command_str,"gnuplot %s", plot_filename);
    retval = system(command_str); /* I assume this is synchronous */

    /* remove temporary files */
    sprintf(command_str,"rm %s %s", data_filename,plot_filename);
    retval = system(command_str);

    return retval;
}

/**
 * @brief Uses phosphene to plot the training error
 * @param history History instance
 * @param img_width Width of the image in pixels
 * @param img_height Height of the image in pixels
 * @return zero on success
 */
int deeplearn_history_phosphene(deeplearn_history * history,
                                int img_width, int img_height)
{
    double value,x,y;
    scope s;
    unsigned int t, channel = 0, t2 = 0;
    double min_time=9999999;
    double max_time=-9999999;
    double max_voltage = 0.01f;
    double min_voltage = history->history[0][0];
    unsigned int grid_horizontal = 20;
    unsigned int grid_vertical = 16;
    unsigned char * img;

    if (history->no_of_points == 0) {
        COUNTUP(index, history->index) {
            value = history->history[index][0];
            if (value > max_voltage)
                max_voltage = value;
            if (value < min_voltage)
                min_voltage = value;
        }
    }
    else {
        COUNTUP(index, history->index) {
            COUNTUP(p, history->no_of_points) {
                if (p*2+1 >= HISTORY_DIMENSIONS)
                    break;

                x = history->history[index][p*2];
                y = history->history[index][p*2+1];

                if (x > max_time)
                    max_time = x;
                if (x < min_time)
                    min_time = x;

                if (y > max_voltage)
                    max_voltage = y;
                if (y < min_voltage)
                    min_voltage = y;
            }
        }
        min_time = min_time-(max_time*2/100);
        max_time = max_time*102/100;
    }
    min_voltage = min_voltage-(max_voltage*2/100);
    max_voltage = max_voltage*102/100;

    UCHARALLOC(img, img_width*img_height*3);
    if (!img)
        return 1;

    create_scope(&s, 1);
    if (history->no_of_points > 0)
        s.mode = PHOSPHENE_MODE_POINTS;
    s.offset_ms = 0;
    s.marker_position = 0;
    s.time_ms = history->index;
    s.horizontal_multiplier = (unsigned int)history->step;

    if (history->no_of_points == 0) {
        for (t = 0; t < history->index; t++) {
            scope_update(&s, channel,
                         history->history[t][0],
                         min_voltage, max_voltage, t, 0);
        }
    }
    else {
        for (t = 0; t < history->index; t++) {
            COUNTUP(p, history->no_of_points) {
                if (p*2+1 >= HISTORY_DIMENSIONS)
                    break;

                x = history->history[t][p*2];
                y = history->history[t][p*2+1];

                scope_update(&s, channel, x,
                             min_time, max_time, t2, (unsigned char)p);
                scope_update(&s, channel+1, y,
                             min_voltage, max_voltage, t2, (unsigned char)p);
                t2++;
            }
        }
    }

    scope_draw_graph(&s, PHOSPHENE_DRAW_ALL, 3, 100,
                     grid_horizontal, grid_vertical,
                     img, img_width, img_height,
                     PHOSPHENE_SHAPE_RECTANGULAR,
                     history->title,
                     history->label_vertical,
                     history->label_horizontal, 25, 4);

    phosphene_write_png_file(history->filename,
                             img_width, img_height, 24, img);

    free(img);

    return 0;
}

/**
 * @brief Plot the training error to a graph image
 * @param history History instance
 * @param img_width Width of the image in pixels
 * @param img_height Height of the image in pixels
 * @return zero on success
 */
int deeplearn_history_plot(deeplearn_history * history,
                           int img_width, int img_height)
{
#ifdef PLOT_WITH_GNUPLOT
    return deeplearn_history_gnuplot(history, img_width, img_height);
#else
    return deeplearn_history_phosphene(history, img_width, img_height);
#endif
}
