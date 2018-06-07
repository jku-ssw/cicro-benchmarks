/****************************************************************

 Phosphene: Plot data in the style of a Cathode Ray Oscilloscope

 =============================================================

 Copyright 2013, 2015, 2017 Bob Mottram
 bob@freedombone.net

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or
 sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the followingp
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

****************************************************************/

#ifndef PHOSPHENE_H
#define PHOSPHENE_H

#define PHOSPHENE_MAX_TIME_STEPS 60000
#define PHOSPHENE_NO_TRACE -999

enum {
    PHOSPHENE_SHAPE_RECTANGULAR = 0,
    PHOSPHENE_SHAPE_CIRCULAR
};

enum {
    PHOSPHENE_DRAW_ALL = 0,
    PHOSPHENE_DRAW_BACKGROUND,
    PHOSPHENE_DRAW_FOREGROUND
};

enum {
    PHOSPHENE_MODE_DEFAULT = 0,
    PHOSPHENE_MODE_XY,
    PHOSPHENE_MODE_POINTS
};

enum {
    PHOSPHENE_MARKER_VERTICAL = 0,
    PHOSPHENE_MARKER_HORIZONTAL
};

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lodepng.h"

struct scope_struct {
    unsigned char mode;
    unsigned int no_of_traces;
    unsigned int border_percent;
    double vertical_percent[2];
    double trace1_min, trace1_max;
    double trace2_min, trace2_max;
    double trace1[PHOSPHENE_MAX_TIME_STEPS];
    double trace2[PHOSPHENE_MAX_TIME_STEPS];
    unsigned char trace_class[PHOSPHENE_MAX_TIME_STEPS];
    int offset_ms;
    unsigned int time_ms, step_ms;
    unsigned int trace1_scan_ms, trace2_scan_ms;
    double marker_position;
    unsigned char marker_orientation;
    unsigned int marker_thickness;
    double trigger_voltage;
    unsigned char background_colour[3];
    unsigned char background_border_colour[3];
    unsigned char markings_colour[3];
    unsigned char trace_colour[3];
    unsigned char trace_colour_green[3];
    unsigned char trace_colour_red[3];
    unsigned char trace_colour_blue[3];
    unsigned char trace_surround_colour[3];
    unsigned int horizontal_multiplier;
    double noise;
};
typedef struct scope_struct scope;

int phosphene_write_png_file(char* filename,
                             unsigned int width, unsigned int height,
                             unsigned int bitsperpixel,
                             unsigned char buffer[]);
void create_scope(scope * s, unsigned int step_ms);
void scope_clear(scope * s);
void scope_update(scope * s,
                  unsigned int trace_index,
                  double value, double min, double max,
                  unsigned int t_ms, unsigned char class);
void scope_draw_bounded(scope * s,
                        unsigned char draw_type,
                        double intensity_percent,
                        int grid_x, int grid_y,
                        unsigned char img[],
                        unsigned int tx, unsigned int ty,
                        unsigned int bx, unsigned int by,
                        unsigned int width, unsigned int height,
                        unsigned char shape, unsigned char graph_type);
void scope_draw(scope * s,
                unsigned char draw_type,
                double intensity_percent,
                int grid_x, int grid_y,
                unsigned char img[],
                unsigned int width, unsigned int height,
                unsigned char shape);
void scope_text(char text[], scope * s,
                int x, int y,
                int character_width_pixels,
                int character_height_pixels,
                double intensity_percent,
                unsigned char img[],
                unsigned int width, unsigned int height);
void scope_text_vertical(char text[], scope * s,
                         int x, int y,
                         int character_width_pixels,
                         int character_height_pixels,
                         double intensity_percent,
                         unsigned char img[],
                         unsigned int width, unsigned int height);
void scope_draw_graph(scope * s,
                      unsigned char draw_type,
                      unsigned int radius,
                      double intensity_percent,
                      int grid_x, int grid_y,
                      unsigned char img[],
                      unsigned int width, unsigned int height,
                      unsigned char shape,
                      char title[],
                      char vertical_text[],
                      char horizontal_text[],
                      int text_size_pixels,
                      int no_of_increments);

#endif
