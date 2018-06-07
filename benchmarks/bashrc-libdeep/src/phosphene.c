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

#include "phosphene.h"

/*
 * Based on public domain 8x8 font by Marcel Sondaar
 */
unsigned char font8x8_basic[128][8] = {
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   /* U+0021 (!) */
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   /* U+0022 (") */
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   /* U+0023 (#) */
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   /* U+0024 ($) */
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   /* U+0025 (%) */
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   /* U+0026 (&) */
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   /* U+0027 (') */
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   /* U+0028 (() */
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   /* U+0029 ()) */
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   /* U+002A (*) */
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   /* U+002B (+) */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   /* U+002C (,) */
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   /* U+002D (-) */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   /* U+002E (.) */
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   /* U+002F (/) */
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   /* U+0030 (0) */
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   /* U+0031 (1) */
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   /* U+0032 (2) */
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   /* U+0033 (3) */
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   /* U+0034 (4) */
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   /* U+0035 (5) */
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   /* U+0036 (6) */
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   /* U+0037 (7) */
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   /* U+0038 (8) */
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   /* U+0039 (9) */
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   /* U+003A (:) */
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   /* U+003B (//) */
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   /* U+003C (<) */
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   /* U+003D (=) */
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   /* U+003E (>) */
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   /* U+003F (?) */
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   /* U+0040 (@) */
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   /* U+0041 (A) */
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   /* U+0042 (B) */
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   /* U+0043 (C) */
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   /* U+0044 (D) */
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   /* U+0045 (E) */
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   /* U+0046 (F) */
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   /* U+0047 (G) */
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   /* U+0048 (H) */
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   /* U+0049 (I) */
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   /* U+004A (J) */
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   /* U+004B (K) */
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   /* U+004C (L) */
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   /* U+004D (M) */
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   /* U+004E (N) */
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   /* U+004F (O) */
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   /* U+0050 (P) */
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   /* U+0051 (Q) */
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   /* U+0052 (R) */
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   /* U+0053 (S) */
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   /* U+0054 (T) */
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   /* U+0055 (U) */
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   /* U+0056 (V) */
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   /* U+0057 (W) */
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   /* U+0058 (X) */
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   /* U+0059 (Y) */
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   /* U+005A (Z) */
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   /* U+005B ([) */
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   /* U+005C (\) */
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   /* U+005D (]) */
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   /* U+005E (^) */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   /* U+005F (_) */
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   /* U+0060 (`) */
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   /* U+0061 (a) */
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   /* U+0062 (b) */
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   /* U+0063 (c) */
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   /* U+0064 (d) */
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   /* U+0065 (e) */
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   /* U+0066 (f) */
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   /* U+0067 (g) */
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   /* U+0068 (h) */
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   /* U+0069 (i) */
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   /* U+006A (j) */
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   /* U+006B (k) */
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   /* U+006C (l) */
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   /* U+006D (m) */
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   /* U+006E (n) */
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   /* U+006F (o) */
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   /* U+0070 (p) */
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   /* U+0071 (q) */
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   /* U+0072 (r) */
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   /* U+0073 (s) */
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   /* U+0074 (t) */
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   /* U+0075 (u) */
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   /* U+0076 (v) */
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   /* U+0077 (w) */
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   /* U+0078 (x) */
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   /* U+0079 (y) */
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   /* U+007A (z) */
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   /* U+007B ({) */
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   /* U+007C (|) */
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   /* U+007D (}) */
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    /* U+007E (~) */
};

/**
 * @brief Saves a graph as a PNG image
 * @param filename Filename for the image
 * @param width Width of the image
 * @param height Height of the image
 * @param bitsperpixel Number of bits per pixel (typically 24)
 * @param buffer Array containing image to be saved
 * @returns Zero on success
 */
int phosphene_write_png_file(char* filename,
                             unsigned int width, unsigned int height,
                             unsigned int bitsperpixel,
                             unsigned char buffer[])
{
    unsigned error=1;
    unsigned int i, d;
    unsigned char * image = buffer;

    if (bitsperpixel == 32)
        error = lodepng_encode32_file(filename, image, width, height);

    if (bitsperpixel == 24)
        error = lodepng_encode24_file(filename, image, width, height);

    if (bitsperpixel == 8) {
        image = (unsigned char*)malloc(width*height*3);
        if (image) {
            for (i = 0; i < width*height; i++)
                for (d = 2; d >= 0; d--)
                    image[i*3+d] = buffer[i];

            error = lodepng_encode24_file(filename, image, width, height);
            free(image);
        }
        else {
            return -2;
        }
    }

    if (error) {
        printf("write_png_file: error %u: %s\n",
               error, lodepng_error_text(error));
        return -1;
    }
    return 0;
}

/**
 * @brief Creates an oscilloscope instance
 * @param s Oscilloscope instance
 * @param step_ms Update time step
 */
void create_scope(scope * s, unsigned int step_ms)
{
    s->mode = PHOSPHENE_MODE_DEFAULT;
    s->marker_orientation = PHOSPHENE_MARKER_VERTICAL;

    s->no_of_traces = 1;
    s->border_percent = 2;

    s->step_ms = step_ms;

    s->background_colour[0] = 105;
    s->background_colour[1] = 183;
    s->background_colour[2] = 169;

    s->background_border_colour[0] = 38;
    s->background_border_colour[1] = 118;
    s->background_border_colour[2] = 117;

    s->markings_colour[0] = 64;
    s->markings_colour[1] = 119;
    s->markings_colour[2] = 114;

    s->trace_colour[0] = 252;
    s->trace_colour[1] = 252;
    s->trace_colour[2] = 252;

    s->trace_colour_green[0] = 252;
    s->trace_colour_green[1] = 152;
    s->trace_colour_green[2] = 152;

    s->trace_colour_blue[0] = 152;
    s->trace_colour_blue[1] = 152;
    s->trace_colour_blue[2] = 252;

    s->trace_colour_red[0] = 252;
    s->trace_colour_red[1] = 252;
    s->trace_colour_red[2] = 122;

    s->trace_surround_colour[0] = 30;
    s->trace_surround_colour[1] = 164;
    s->trace_surround_colour[2] = 127;

    s->trace1_min = 0;
    s->trace1_max = 0;
    s->trace2_min = 0;
    s->trace2_max = 0;

    s->vertical_percent[0] = 0;
    s->vertical_percent[1] = 0;

    s->marker_position = 0;
    s->marker_thickness = 2;
    s->trace1_scan_ms = 0;
    s->trace2_scan_ms = 0;
    s->time_ms = 2000;

    s->trace1[0] = PHOSPHENE_NO_TRACE;
    s->trace2[0] = PHOSPHENE_NO_TRACE;

    s->offset_ms = 0;
    s->trigger_voltage = 0;

    s->noise = 0.03;
    s->horizontal_multiplier=1;
}

/**
 * @brief Clears all plots
 * @param s Oscilloscope instance
 */
void scope_clear(scope * s)
{
    s->trace1[0] = PHOSPHENE_NO_TRACE;
    s->trace2[0] = PHOSPHENE_NO_TRACE;
}

/**
 * @brief for circular scopes this returns the radius squared
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 */
static double scope_radius_squared(unsigned int top_x,
                                   unsigned int top_y,
                                   unsigned int bottom_x,
                                   unsigned int bottom_y)
{
    if (bottom_x-top_x < bottom_y-top_y)
        return ((bottom_x-top_x)*0.5)*((bottom_x-top_x)*0.5);

    return ((bottom_y-top_y)*0.5)*((bottom_y-top_y)*0.5);
}

/**
 * @brief Update the scope with a value/voltage
 * @param s Oscilloscope instance
 * @param trace_index
 * @param value Value or voltage measured
 * @param min Minimum value
 * @param max Maximum value
 * @param t_ms Time on the horizontal axis
 * @param class
 */
void scope_update(scope * s,
                  unsigned int trace_index,
                  double value, double min, double max,
                  unsigned int t_ms, unsigned char class)
{
    unsigned int t = t_ms/s->step_ms;

    if (t >= PHOSPHENE_MAX_TIME_STEPS)
        return;

    if ((t_ms < s->time_ms) ||
        (s->mode > PHOSPHENE_MODE_DEFAULT)) {
        if (trace_index == 0) {
            s->trace1[t] = value;
            s->trace1_min = min;
            s->trace1_max = max;
            s->trace1_scan_ms = t_ms;
        }
        else {
            s->no_of_traces = 2;
            s->trace2[t] = value;
            s->trace2_min = min;
            s->trace2_max = max;
            s->trace2_scan_ms = t_ms;
        }
        s->trace_class[t] = class;
    }
}

/**
 * @brief draws a single point with the beam using a given intensity and radius
 * @param s Oscilloscope instance
 * @param x X coordinate of the point
 * @param y Y coordinate of the point
 * @param radius Radius of the point
 * @param intensity_percent Intensity in the range 0-100
 * @param img Array containing scope image data
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 * @param class Class indicates colour of the point
 */
static void scope_point(scope * s,
                        int x, int y,
                        unsigned int radius, double intensity_percent,
                        unsigned char img[],
                        unsigned int top_x, unsigned int top_y,
                        unsigned int bottom_x, unsigned int bottom_y,
                        unsigned int width, unsigned int height,
                        unsigned char shape, unsigned char class)
{
    int xx,yy,dx,dy,dist,c,target,n,noise;
    int tx = x - radius;
    int ty = y - radius;
    int bx = x + radius;
    int by = y + radius;
    double fraction, diff;
    unsigned int border_y = (bottom_y-top_y)*s->border_percent/100;
    unsigned int border_x = (bottom_x-top_x)*s->border_percent/100;
    double radius_squared = 0;
    int cx,cy;
    unsigned char * trace_colour = &s->trace_colour[0];

    if (shape == PHOSPHENE_SHAPE_CIRCULAR) {
        radius_squared = scope_radius_squared(top_x+radius,
                                              top_y+radius,
                                              bottom_x-radius,
                                              bottom_y-radius);
        cx = top_x + ((bottom_x-top_x)/2);
        cy = top_y + ((bottom_y-top_y)/2);
        dx = x - cx;
        dy = y - cy;
        if (dx*dx + dy*dy > radius_squared)
            return;
    }

    /* alter the colour depending on the class of the point */
    switch(class) {
    case 1: {
        trace_colour = &s->trace_colour_green[0];
        break;
    }
    case 2: {
        trace_colour = &s->trace_colour_blue[0];
        break;
    }
    case 3: {
        trace_colour = &s->trace_colour_red[0];
        break;
    }
    }

    if (tx < top_x) tx = top_x;
    if (ty < top_y) ty = top_y;
    if (bx >= (int)bottom_x) bx = (int)bottom_x-1;
    if (by >= (int)bottom_y) by = (int)bottom_y-1;

    intensity_percent /= 100.0;

    for (xx = tx; xx <= bx; xx++) {
        if ((xx < (int)(top_x + border_x)) ||
            (xx > (int)(bottom_x - border_x)))
            continue;

        dx = xx - x;
        for (yy = ty; yy <= by; yy++) {
            if ((yy < (int)(top_y + border_y)) ||
                (yy > (int)(bottom_y - border_y)))
                continue;

            dy = yy - y;
            dist = (int)sqrt(dx*dx + dy*dy);
            if (dist < (int)radius) {
                n = (yy*width + xx)*3;

                if (dist > 1)
                    fraction = (radius - dist) / (double)radius;
                else
                    fraction = 1;

                noise = (rand()%60)-30;
                for (c = 2; c >= 0; c--) {
                    diff =
                        (trace_colour[c] - s->trace_surround_colour[c]) *
                        intensity_percent;
                    target =
                        s->trace_surround_colour[c] +
                        (unsigned char)(diff * fraction) + noise;
                    if (target > 255) target = 255;

                    if (target > img[n+c])
                        img[n+c] += (target - img[n+c])/2;
                }
            }
        }
    }
}

/**
 * @brief Adds a screen marking point. These are the darker grid markings
 * @param s Oscilloscope instance
 * @param x X coordinate of the point
 * @param y Y coordinate of the point
 * @param radius Radius of the point
 * @param img Array containing scope image data
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_marking_point(scope * s,
                                int x, int y,
                                unsigned int radius,
                                unsigned char img[],
                                unsigned int top_x, unsigned int top_y,
                                unsigned int bottom_x, unsigned int bottom_y,
                                unsigned int width, unsigned int height,
                                unsigned char shape)
{
    int xx,yy,dx,dy,dist,c,n,diff,v;
    int tx = x - radius;
    int ty = y - radius;
    int bx = x + radius;
    int by = y + radius;
    double radius_squared = 0;
    int cx,cy;

    if (shape == PHOSPHENE_SHAPE_CIRCULAR) {
        radius_squared = scope_radius_squared(top_x+radius,
                                              top_y+radius,
                                              bottom_x-radius,
                                              bottom_y-radius);
        cx = top_x + ((bottom_x-top_x)/2);
        cy = top_y + ((bottom_y-top_y)/2);
        dx = x - cx;
        dy = y - cy;
        if (dx*dx + dy*dy > radius_squared)
            return;
    }

    if (tx < top_x) tx = top_x;
    if (ty < top_y) ty = top_y;
    if (bx >= (int)bottom_x) bx = bottom_x-1;
    if (by >= (int)bottom_y) by = bottom_y-1;

    for (xx = tx; xx <= bx; xx++) {
        dx = xx - x;
        for (yy = ty; yy <= by; yy++) {
            dy = yy - y;
            dist = (int)sqrt(dx*dx + dy*dy);
            if (dist < (int)radius) {
                n = (yy*width + xx)*3;
                for (c = 0; c < 3; c++) {
                    diff = s->background_colour[c] - s->markings_colour[c];
                    v = img[n+c] - diff;
                    if (v < 30) v = 30;
                    img[n+c] = v;
                }
            }
        }
    }
}

/**
 * @brief draws a dotted screen marking
 * @param s Oscilloscope instance
 * @param x0 Start x coordinate
 * @param y0 Start y coordinate
 * @param x1 End x coordinate
 * @param y1 End y coordinate
 * @param dots Number of dots on the line
 * @param radius Radius of each dot
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_dotted_line(scope * s,
                              int x0, int y0,
                              int x1, int y1,
                              int dots,
                              unsigned int radius,
                              unsigned char img[],
                              unsigned int top_x, unsigned int top_y,
                              unsigned int bottom_x, unsigned int bottom_y,
                              unsigned int width, unsigned int height,
                              unsigned char shape)
{
    int i, x, y;
    int dx = x1 - x0;
    int dy = y1 - y0;

    for (i = 0; i <= dots; i++) {
        x = x0 + (dx*i/dots);
        y = y0 + (dy*i/dots);
        scope_marking_point(s, x, y,
                            radius, img,
                            top_x, top_y,
                            bottom_x, bottom_y,
                            width, height, shape);
    }
}

/**
 * @brief draws a solid screen marking
 * @param s Oscilloscope instance
 * @param x0 Start x coordinate
 * @param y0 Start y coordinate
 * @param x1 End x coordinate
 * @param y1 End y coordinate
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_marking_line(scope * s,
                               int x0, int y0,
                               int x1, int y1,
                               unsigned char img[],
                               unsigned int top_x, unsigned int top_y,
                               unsigned int bottom_x, unsigned int bottom_y,
                               unsigned int width, unsigned int height,
                               unsigned char shape)
{
    int i, x, y, c, diff, v, cx, cy, dx, dy, length;
    unsigned int n;
    double radius_squared;

    if (shape == PHOSPHENE_SHAPE_CIRCULAR) {
        radius_squared = scope_radius_squared(top_x, top_y,
                                              bottom_x, bottom_y);
        cx = top_x + ((bottom_x-top_x)/2);
        cy = top_y + ((bottom_y-top_y)/2);
    }

    dx = (int)x1 - (int)x0;
    dy = (int)y1 - (int)y0;
    length = (int)sqrt(dx*dx + dy*dy);
    for (i = 0; i <= length; i++) {
        x = (int)x0 + (i*dx/length);
        y = (int)y0 + (i*dy/length);

        if (shape == PHOSPHENE_SHAPE_CIRCULAR)
            if ((x - cx)*(x - cx) + (y - cy)*(y - cy) > radius_squared)
                continue;

        n = (y*width + x)*3;
        for (c = 0; c < 3; c++) {
            diff = s->background_colour[c] - s->markings_colour[c];
            v = img[n+c] - diff;
            if (v < 10) v = 10;
            img[n+c] = v;
        }
    }
}

/**
 * @brief draws a solid screen marking
 * @param s Oscilloscope instance
 * @param x0 Start x coordinate
 * @param y0 Start y coordinate
 * @param x1 End x coordinate
 * @param y1 End y coordinate
 * @param thickness Line thickness
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_marking(scope * s,
                          int x0, int y0,
                          int x1, int y1,
                          int thickness,
                          unsigned char img[],
                          unsigned int top_x, unsigned int top_y,
                          unsigned int bottom_x, unsigned int bottom_y,
                          unsigned int width, unsigned int height,
                          unsigned char shape)
{
    int dx = (int)x1 - (int)x0;
    int dy = (int)y1 - (int)y0;
    int x,y,offset = thickness/2;

    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    if (dx > dy)
        for (y = -thickness; y < 0; y++)
            scope_marking_line(s, x0,  y0+y+offset, x1, y1+y+offset,
                               img, top_x, top_y, bottom_x, bottom_y,
                               width, height, shape);
    else
        for (x = -thickness; x < 0; x++)
            scope_marking_line(s, x0+x+offset,  y0, x1+x+offset, y1,
                               img, top_x, top_y, bottom_x, bottom_y,
                               width, height, shape);
}

/**
 * @brief draws the increment screen markings
 * @param s Oscilloscope instance
 * @param x0 Start x coordinate
 * @param y0 Start y coordinate
 * @param x1 End x coordinate
 * @param y1 End y coordinate
 * @param increments Number of increment markings
 * @param radius Radius of lines
 * @param thickness Thickness of lines
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_increments(scope * s,
                             int x0, int y0,
                             int x1, int y1,
                             int increments,
                             unsigned int radius,
                             unsigned int thickness,
                             unsigned char img[],
                             unsigned int top_x, unsigned int top_y,
                             unsigned int bottom_x, unsigned int bottom_y,
                             unsigned int width, unsigned int height,
                             unsigned char shape)
{
    int i, x, y;
    int dx = x1 - x0;
    int dy = y1 - y0;

    for (i = 0; i <= increments; i++) {
        x = x0 + (dx*i/increments);
        y = y0 + (dy*i/increments);

        if (abs(dx) > abs(dy))
            scope_marking(s, x, y-radius, x, y+radius-1,
                          thickness, img,
                          top_x, top_y, bottom_x, bottom_y,
                          width, height, shape);
        else
            scope_marking(s, x-radius, y, x+radius-1, y,
                          thickness, img,
                          top_x, top_y, bottom_x, bottom_y,
                          width, height, shape);
    }
}

/**
 * @brief draws the screen marking grid
 * @param s Oscilloscope instance
 * @param cells_x Cells across
 * @param cells_y Cells down
 * @param radius Radius of markings
 * @param thickness Thickness of lines
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_grid(scope * s,
                       unsigned int cells_x, unsigned int cells_y,
                       int radius,
                       int thickness,
                       unsigned char img[],
                       unsigned int top_x, unsigned int top_y,
                       unsigned int bottom_x, unsigned int bottom_y,
                       unsigned int width, unsigned int height,
                       unsigned char shape)
{
    unsigned int x,y,xx,yy,h;
    unsigned int border_y = (bottom_y-top_y)*s->border_percent/100;
    unsigned int border_x = (bottom_x-top_x)*s->border_percent/100;

    for (y = 0; y <= cells_y; y++) {
        yy = top_y + border_y + (((bottom_y-top_y)-(border_y*2))*y/cells_y);
        scope_marking(s, top_x + border_x, yy,
                      bottom_x - border_x, yy, thickness,
                      img, top_x, top_y, bottom_x, bottom_y,
                      width, height, shape);
    }

    for (x = 0; x <= cells_x; x++) {
        xx = top_x + border_x + (((bottom_x-top_x)-(border_x*2))*x/cells_x);
        scope_marking(s, xx, top_y + border_y,
                      xx, bottom_y-border_y, thickness,
                      img, top_x, top_y, bottom_x, bottom_y,
                      width, height, shape);
    }

    h = top_y + border_y + (((bottom_y-top_y)-(border_y*2))*20/100);
    scope_dotted_line(s, top_x + border_x, h, bottom_x-border_x, h,
                      cells_x*5, thickness,
                      img, top_x, top_y, bottom_x, bottom_y,
                      width, height, shape);

    h = top_y + border_y + (((bottom_y-top_y)-(border_y*2))*80/100);
    scope_dotted_line(s, top_x + border_x, h, bottom_x-border_x, h,
                      cells_x*5, thickness,
                      img, top_x, top_y, bottom_x, bottom_y,
                      width, height, shape);

    h = top_x + border_x + (((bottom_x-top_x)-(border_x*2))*50/100);
    scope_increments(s, h, top_y + border_y, h, bottom_y-border_y,
                     cells_y*5, radius/2, thickness,
                     img, top_x, top_y, bottom_x, bottom_y,
                     width, height, shape);

    h = top_y + border_y + (((bottom_y - top_y)-(border_y*2))*50/100);
    scope_increments(s, top_x + border_x, h, bottom_x-border_x, h,
                     cells_x*5, radius/2, thickness,
                     img, top_x, top_y, bottom_x, bottom_y,
                     width, height, shape);
}

/**
 * @brief traces a beam line on the screen with the given intensity and radius
 * @param s Oscilloscope instance
 * @param x0 Start x coordinate
 * @param y0 Start y coordinate
 * @param x1 End x coordinate
 * @param y1 End y coordinate
 * @param radius Radius of markings
 * @param intensity_percent Intensity of markings in the range 0-100
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_trace_line(scope * s,
                             int x0, int y0,
                             int x1, int y1,
                             unsigned int radius, double intensity_percent,
                             unsigned char img[],
                             unsigned int top_x, unsigned int top_y,
                             unsigned int bottom_x, unsigned int bottom_y,
                             unsigned int width, unsigned int height,
                             unsigned char shape)
{
    int dx = (int)x1 - (int)x0;
    int dy = (int)y1 - (int)y0;
    int length = (int)sqrt(dx*dx + dy*dy);
    int i, x, y, prev_x=-9999, prev_y=-9999;

    for (i = 0; i <= length; i++) {
        x = (int)x0 + (i*dx/(length+1));
        y = (int)y0 + (i*dy/(length+1));
        if (!((x==prev_x) && (y==prev_y))) {
            scope_point(s, x, y, radius, intensity_percent,
                        img, top_x, top_y, bottom_x, bottom_y,
                        width, height, shape, 0);
            prev_x = x;
            prev_y = y;
        }
    }
}

/**
 * @brief returns various vertical parameters
 * @param s Oscilloscope instance
 * @param trace_index Index of the trace, 0 or 1
 * @param no_of_traces 1 or 2
 * @param min Returned minimum value for each trace
 * @param max Returned maximum value for each trace
 * @param screen_by Returned bottom Y coordinate
 * @param screen_yy Returned top Y coordinate
 * @param top_x Top x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom x coordinate
 * @param bottom_y Bottom y coordinate
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_verticals(scope * s, unsigned int trace_index,
                            unsigned int no_of_traces,
                            double * min, double * max,
                            int * screen_by, int * screen_ty,
                            unsigned int top_x, unsigned int top_y,
                            unsigned int bottom_x, unsigned int bottom_y,
                            unsigned char shape)
{
    unsigned int border_y = (bottom_y-top_y) * s->border_percent / 100;
    double vertical_percent;

    if (trace_index == 0) {
        *min = s->trace1_min;
        *max = s->trace1_max;
        vertical_percent = s->vertical_percent[0];
    }
    else {
        *min = s->trace2_min;
        *max = s->trace2_max;
        vertical_percent = s->vertical_percent[1];
    }

    if (no_of_traces == 1) {
        if (shape != PHOSPHENE_SHAPE_CIRCULAR) {
            *screen_by = bottom_y - border_y -
                (int)(((bottom_y-top_y)-(border_y*2))*(0+vertical_percent)/100);
            *screen_ty = bottom_y - border_y -
                (int)(((bottom_y-top_y)-(border_y*2))*(100+vertical_percent)/100);
        }
        else {
            *screen_by = bottom_y - border_y -
                (int)(((bottom_y-top_y)-(border_y*2))*(20+vertical_percent)/100);
            *screen_ty = bottom_y - border_y -
                (int)(((bottom_y-top_y)-(border_y*2))*(80+vertical_percent)/100);
        }
        if (*screen_ty < (int)border_y) *screen_ty = border_y;
    }
    else {
        if (trace_index == 0) {
            *screen_by = bottom_y - border_y -
                (int)(((bottom_y-top_y)-(border_y*2))*(60+vertical_percent)/100);
            *screen_ty = bottom_y - border_y -
                (int)(((bottom_y-top_y)-(border_y*2))*(85+vertical_percent)/100);
            if (*screen_ty < (int)border_y) *screen_ty = border_y;
        }
        else {
            *screen_by = bottom_y - border_y -
                (int)(((bottom_y-top_y)-(border_y*2))*(20+vertical_percent)/100);
            *screen_ty = bottom_y - border_y -
                (int)(((bottom_y-top_y)-(border_y*2))*(45+vertical_percent)/100);
        }
    }
}

/**
 * @brief traces a beam on the screen with the given intensity and radius
 * @param s Oscilloscope instance
 * @param trace_index Index of the trace, 0 or 1
 * @param radius Radius of markings
 * @param intensity_percent Intensity of markings in the range 0-100
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_trace(scope * s,
                        unsigned int trace_index,
                        unsigned int radius, double intensity_percent,
                        unsigned char img[],
                        unsigned int top_x, unsigned int top_y,
                        unsigned int bottom_x, unsigned int bottom_y,
                        unsigned int width, unsigned int height,
                        unsigned char shape)
{
    int x,y,prev_x=-99999,prev_y=-99999, screen_by, screen_ty,n;
    unsigned int t_ms=0, border_x, t;
    double value, min, max;

    border_x = (bottom_x-top_x) * s->border_percent / 100;

    scope_verticals(s, trace_index, s->no_of_traces,
                    &min, &max,
                    &screen_by, &screen_ty,
                    top_x, top_y, bottom_x, bottom_y, shape);

    while (t_ms < s->time_ms) {
        n = (int)t_ms - s->offset_ms;
        if (n < 0) n += (int)s->time_ms;
        t = (((unsigned int)n)%s->time_ms) / s->step_ms;

        if (trace_index == 0)
            value = s->trace1[t] + (((rand()%10000)-5000)/5000.0*s->noise);
        else
            value = s->trace2[t] + (((rand()%10000)-5000)/5000.0*s->noise);

        if ((value == PHOSPHENE_NO_TRACE) || (max <= min)) {
            t_ms += s->step_ms;
            continue;
        }

        x = top_x + border_x +
            (t_ms * ((bottom_x-top_x)-(border_x*2)) / s->time_ms);

        y = screen_by - (int)((screen_by - screen_ty)*(value-min)/(max - min));

        scope_trace_line(s, prev_x, prev_y, x, y,
                         radius, intensity_percent -
                         (intensity_percent*0.4*t_ms/s->time_ms),
                         img, top_x, top_y, bottom_x, bottom_y,
                         width, height, shape);

        prev_x = x;
        prev_y = y;
        t_ms += s->step_ms;
    }
}

/**
 * @brief draws a vertical or horizontal alignment marker on the screen
 * @param s Oscilloscope instance
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_marker(scope * s, unsigned char img[],
                         unsigned int top_x, unsigned int top_y,
                         unsigned int bottom_x, unsigned int bottom_y,
                         unsigned int width, unsigned int height,
                         unsigned char shape)
{
    unsigned int border_x, border_y, x, y;
    unsigned int i, trace_index = 0;
    double min=0, max=1;
    int screen_by, screen_ty;

    if (s->marker_position == 0)
        return;

    border_x = (bottom_x-top_x) * s->border_percent / 100;
    border_y = (bottom_y-top_y) * s->border_percent / 100;

    if (s->marker_orientation == PHOSPHENE_MARKER_VERTICAL) {
        x = top_x + border_x +
            (s->marker_position *
             ((bottom_x-top_x)-(border_x*2)) / s->time_ms);
        for (i = 0; i < s->marker_thickness; i++) {
            scope_trace_line(s, x+i, border_y, x+i, bottom_y - border_y,
                             1, 50, img, top_x, top_y, bottom_x, bottom_y,
                             width, height, shape);
        }
    }
    else {
        scope_verticals(s, trace_index, s->no_of_traces,
                        &min, &max,
                        &screen_by, &screen_ty,
                        top_x, top_y, bottom_x, bottom_y, shape);

        y = screen_by - (int)((screen_by - screen_ty)*
            (s->marker_position-min)/(max - min));

        for (i = 0; i < s->marker_thickness; i++)
            scope_trace_line(s, border_x, y+i, bottom_x-border_x, y+i,
                             1, 50, img,
                             top_x, top_y, bottom_x, bottom_y,
                             width, height, shape);
    }
}

/**
 * @brief Draws the background
 * @param s Oscilloscope instance
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_background(scope * s, unsigned char img[],
                             unsigned int top_x, unsigned int top_y,
                             unsigned int bottom_x, unsigned int bottom_y,
                             unsigned int width, unsigned int height,
                             unsigned char shape)
{
    int x, y, dx, dy, n, c, diff, r;
    int cx = top_x + ((bottom_x-top_x)/2);
    int cy = top_y + ((bottom_y-top_y)/2);
    double fraction_x, fraction_y, fraction;
    double scale_x = 3.1415927*0.5/(double)(cx-top_x);
    double scale_y = 3.1415927*0.5/(double)(cy-top_y);
    double radius_squared = 0;

    if (shape == PHOSPHENE_SHAPE_CIRCULAR)
        radius_squared = scope_radius_squared(top_x, top_y,
                                              bottom_x, bottom_y);

    for (y = (int)top_y; y < (int)bottom_y; y++) {
        dy = y - cy;
        if (dy < 0) dy = -dy;
        fraction_y = cos(dy * scale_y);
        for (x = (int)top_x; x < (int)bottom_x; x++) {
            n = (y*width + x)*3;
            dx = x - cx;
            if (dx < 0) dx = -dx;

            if ((shape == PHOSPHENE_SHAPE_CIRCULAR) &&
                (dx*dx + dy*dy > radius_squared))
                continue;

            if (shape == PHOSPHENE_SHAPE_RECTANGULAR) {
                fraction_x = cos(dx * scale_x);

                if (fraction_x < fraction_y)
                    fraction = 1.0 - fabs(fraction_x);
                else
                    fraction = 1.0 - fabs(fraction_y);
            }
            else {
                /* circular shape */
                r = dx*dx + dy*dy;
                fraction = 1.0 - (((cos(3.1415927*r/radius_squared))+1)*0.5);
            }
            fraction = 1.0 - (fraction*fraction*fraction*fraction);
            for (c = 2; c >= 0; c--) {
                diff =
                    s->background_colour[c] - s->background_border_colour[c];
                img[n+c] =
                    s->background_border_colour[c] + (int)(diff*fraction);
            }
        }
    }
}

/**
 * @brief plots traces 0 and 1 together
 * @param s Oscilloscope instance
 * @param radius Radius of points in the trace
 * @param intensity_percent Intensity in the range 0-100
 * @param draw_lines Draw using lines or points
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_xy(scope * s,
                     unsigned int radius, double intensity_percent,
                     unsigned char draw_lines,
                     unsigned char img[],
                     unsigned int top_x, unsigned int top_y,
                     unsigned int bottom_x, unsigned int bottom_y,
                     unsigned int width, unsigned int height,
                     unsigned char shape)
{
    int x, y, prev_x = -9999, prev_y = -9999;
    int screen_bx, screen_tx, screen_by, screen_ty, n;
    unsigned int t_ms=0, t;
    double value_x, value_y, min_x, max_x, min_y, max_y;

    /* limits of trace 0 */
    scope_verticals(s, 0, 1,
                    &min_x, &max_x,
                    &screen_bx, &screen_tx,
                    top_x, top_y, bottom_x, bottom_y, shape);

    /* limits of trace 1 */
    scope_verticals(s, 1, 1,
                    &min_y, &max_y,
                    &screen_by, &screen_ty,
                    top_x, top_y, bottom_x, bottom_y, shape);

    if ((max_x <= min_x) || (max_y <= min_y))
        return;

    while (t_ms < s->time_ms) {
        n = (int)t_ms - s->offset_ms;
        if (n < 0) n += (int)s->time_ms;
        t = (((unsigned int)n)%s->time_ms) / s->step_ms;
        value_x = s->trace1[t] + (((rand()%10000)-5000)/5000.0*s->noise);
        value_y = s->trace2[t] + (((rand()%10000)-5000)/5000.0*s->noise);
        if ((value_x == PHOSPHENE_NO_TRACE) ||
            (value_y == PHOSPHENE_NO_TRACE)) {
            t_ms += s->step_ms;
            continue;
        }

        x = screen_bx -
            (int)((screen_bx - screen_tx)*(value_x-min_x)/(max_x - min_x));

        y = screen_by -
            (int)((screen_by - screen_ty)*(value_y-min_y)/(max_y - min_y));

        if (draw_lines == 0)
            scope_point(s, x, y,
                        radius, intensity_percent,
                        img, top_x, top_y, bottom_x, bottom_y,
                        width, height, shape, s->trace_class[t]);
        else
            scope_trace_line(s, prev_x, prev_y, x, y,
                             radius, intensity_percent,
                             img, top_x, top_y, bottom_x, bottom_y,
                             width, height, shape);

        prev_x = x;
        prev_y = y;
        t_ms += s->step_ms;
    }
}

/**
 * @brief draws the background
 * @param s Oscilloscope instance
 * @param grid_x Number of grid cells across
 * @param grid_y Number of grid cells down
 * @param radius Radius for plotting points
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
static void scope_draw_background(scope * s,
                                  int grid_x, int grid_y,
                                  unsigned int radius,
                                  unsigned char img[],
                                  unsigned int top_x, unsigned int top_y,
                                  unsigned int bottom_x, unsigned int bottom_y,
                                  unsigned int width, unsigned int height,
                                  unsigned char shape)
{
    int thickness = (bottom_x-top_x)/320;

    if (thickness < 1)
        thickness = 1;

    scope_background(s, img, top_x, top_y, bottom_x, bottom_y,
                     width, height, shape);

    scope_grid(s, grid_x, grid_y, radius, thickness, img,
               top_x, top_y, bottom_x, bottom_y,
               width, height, shape);
}

/**
 * @brief The main drawing function
 * @param s Oscilloscope instance
 * @param draw_type Drawing mode
 * @param intensity_percent Intensity of points in the range 0-100
 * @param grid_x Number of grid cells across
 * @param grid_y Number of grid cells down
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 * @param graph_type 0 = standard, 1 = with axes and title
 */
void scope_draw_bounded(scope * s,
                        unsigned char draw_type,
                        double intensity_percent,
                        int grid_x, int grid_y,
                        unsigned char img[],
                        unsigned int top_x, unsigned int top_y,
                        unsigned int bottom_x, unsigned int bottom_y,
                        unsigned int width, unsigned int height,
                        unsigned char shape, unsigned char graph_type)
{
    unsigned int i;
    unsigned int border_y = height*s->border_percent/100;
    unsigned int border_x = width*s->border_percent/100;
    unsigned int radius = (bottom_x-top_x)/(320/2);
    unsigned int top_xx = top_x;
    unsigned int top_yy = top_y;
    unsigned int bottom_xx = bottom_x;
    unsigned int bottom_yy = bottom_y;
    unsigned int grid_width = (width - (border_x*2)) / (unsigned int)grid_x;
    unsigned int grid_height = (height - (border_y*2)) / (unsigned int)grid_y;

    if (radius < 1) radius = 1;

    if (graph_type == 1) {
        top_xx = top_x + grid_width;
        top_yy = top_y + grid_height;
        bottom_xx = bottom_x - grid_width;
        bottom_yy = bottom_y - grid_height;
    }

    /* draw the background */
    if ((draw_type == PHOSPHENE_DRAW_ALL) ||
        (draw_type == PHOSPHENE_DRAW_BACKGROUND))
        scope_draw_background(s, grid_x, grid_y, radius,
                              img, top_x, top_y,
                              bottom_x, bottom_y,
                              width, height, shape);

    if ((draw_type == PHOSPHENE_DRAW_ALL) ||
        (draw_type == PHOSPHENE_DRAW_FOREGROUND)) {
        /* check that the time base doesn't exceed the maximum */
        if (s->time_ms/s->step_ms >= PHOSPHENE_MAX_TIME_STEPS)
            s->time_ms = (PHOSPHENE_MAX_TIME_STEPS-1) * s->step_ms;

        /* if a trigger voltage is set then check for it
           and adjust the marker position accordingly */
        if (s->trigger_voltage > 0) {
            for (i = 1; i < s->time_ms/s->step_ms; i++) {
                if ((s->trace1[i-1] < s->trigger_voltage) &&
                    (s->trace1[i] > s->trigger_voltage)) {
                    s->offset_ms = -(i*s->step_ms) +
                        (int)(s->time_ms*0.05);
                    s->marker_position = s->time_ms*0.05;
                    break;
                }
            }
        }

        /* show marker */
        scope_marker(s, img, top_x, top_y,
                     bottom_x, bottom_y,
                     width, height, shape);

        /* draw traces */
        switch(s->mode) {
        case PHOSPHENE_MODE_DEFAULT: {
            for (i = 0; i < s->no_of_traces; i++) {
                scope_trace(s, i, radius, intensity_percent,
                            img, top_xx, top_yy,
                            bottom_xx, bottom_yy,
                            width, height, shape);
            }
            break;
        }
        case PHOSPHENE_MODE_XY: {
            scope_xy(s, radius, intensity_percent, 1,
                     img, top_xx, top_yy, bottom_xx, bottom_yy,
                     width, height, shape);
            break;
        }
        case PHOSPHENE_MODE_POINTS: {
            scope_xy(s, radius, intensity_percent, 0,
                     img, top_xx, top_yy, bottom_xx, bottom_yy,
                     width, height, shape);
            break;
        }
        }
    }
}

/**
 * @brief Draw a scope
 * @param s Oscilloscope instance
 * @param draw_type Drawing mode
 * @param intensity_percent Intensity of points in the range 0-100
 * @param grid_x Number of grid cells across
 * @param grid_y Number of grid cells down
 * @param img Array containing scope image
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 */
void scope_draw(scope * s,
                unsigned char draw_type,
                double intensity_percent,
                int grid_x, int grid_y,
                unsigned char img[],
                unsigned int width, unsigned int height,
                unsigned char shape)
{
    /* if this is a circular scope then clear the background */
    if (shape == PHOSPHENE_SHAPE_CIRCULAR)
        memset((void*)img, '\0', width*height*3);

    scope_draw_bounded(s, draw_type,
                       intensity_percent,
                       grid_x, grid_y,
                       img, 0,0, width, height,
                       width, height, shape, 0);
}

/**
 * @brief Draw a single character to the scope
 * @param c The character to draw
 * @param s Oscilloscope instance
 * @param tx Top left x coordinate
 * @param ty Top y coordinate
 * @param bx Bottom right x coordinate
 * @param by Bottom y coordinate
 * @param radius Radius of pixel points within the character image
 * @param intensity_percent Intensity of points in the range 0-100
 * @param img Array containing scope image
 * @param top_x Top left x coordinate
 * @param top_y Top y coordinate
 * @param bottom_x Bottom right x coordinate
 * @param bottom_y Bottom y coordinate
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 * @param vertical Whether to draw the character in vertical orientation
 */
static void scope_character(char c, scope * s,
                            int tx, int ty, int bx, int by,
                            unsigned int radius, double intensity_percent,
                            unsigned char img[],
                            unsigned int top_x, unsigned int top_y,
                            unsigned int bottom_x, unsigned int bottom_y,
                            unsigned int width, unsigned int height,
                            unsigned char shape, unsigned char vertical)
{
    int x, y, index = (int)c - (int)'!';
    int img_x, img_y;
    char * bitmap;

    if ((index < 0) || (index >= 105))
        return;

    if ((tx >= width) || (bx >= width))
        return;

    if ((ty >= height) || (by >= height))
        return;

    bitmap = (char*)font8x8_basic[index];

    for (y = 0; y < 8; y++) {
        if (vertical == 0)
            img_y = ty + (y*(by-ty)/8);
        else
            img_y = by - (y*(by-ty)/8);
        for (x = 0; x < 8; x++) {
            img_x = tx + (x*(bx-tx)/8);
            if (((vertical != 0) && (bitmap[x] & 1 << y)) ||
                ((vertical == 0) && (bitmap[y] & 1 << x)))
                scope_point(s, img_x, img_y, radius, intensity_percent,
                            img, top_x, top_y, bottom_x, bottom_y,
                            width, height, shape, 0);
        }
    }
}

/**
 * @brief Draws text to the scope
 * @param text The text to be drawn
 * @param s Oscilloscope instance
 * @param x Top left coordinate of to begin drawing from
 * @param y Top coordinate of to begin drawing from
 * @param character_width_pixels
 * @param character_height_pixels
 * @param intensity_percent
 * @param img Array containing scope image
 * @param width Width of the scope image
 * @param height Height of the scope image
 */
void scope_text(char text[], scope * s,
                int x, int y,
                int character_width_pixels,
                int character_height_pixels,
                double intensity_percent,
                unsigned char img[],
                unsigned int width, unsigned int height)
{
    int i, tx;
    unsigned int radius = (unsigned int)(character_width_pixels/4);

    if (radius < 1) radius = 1;

    for (i = 0; i < (int)strlen(text); i++) {
        tx = x + (i*character_width_pixels);
        scope_character(text[i], s,
                        tx, y, tx+character_width_pixels,
                        y+character_height_pixels,
                        radius, intensity_percent, img,
                        0, 0, width, height,
                        width, height, PHOSPHENE_SHAPE_RECTANGULAR, 0);
    }
}

/**
 * @brief Draws text to the scope in vertical orientation
 * @param text The text to be drawn
 * @param s Oscilloscope instance
 * @param x Bottom left coordinate of to begin drawing from
 * @param y Bottom y coordinate of to begin drawing from
 * @param character_width_pixels
 * @param character_height_pixels
 * @param intensity_percent
 * @param img Array containing scope image
 * @param width Width of the scope image
 * @param height Height of the scope image
 */
void scope_text_vertical(char text[], scope * s,
                         int x, int y,
                         int character_width_pixels,
                         int character_height_pixels,
                         double intensity_percent,
                         unsigned char img[],
                         unsigned int width, unsigned int height)
{
    int i, ty;
    unsigned int radius = (unsigned int)(character_height_pixels/4);

    if (radius < 1) radius = 1;

    for (i = 0; i < (int)strlen(text); i++) {
        ty = y - (i*character_height_pixels);
        scope_character(text[i], s,
                        x, ty, x+character_width_pixels,
                        ty+character_height_pixels,
                        radius, intensity_percent, img,
                        0, 0, width, height,
                        width, height, PHOSPHENE_SHAPE_RECTANGULAR, 1);
    }
}

/**
 * @brief Draws a series of numbers along a given line
 * @param s Oscilloscope instance
 * @param x0 Start x coordinate
 * @param y0 Start y coordinate
 * @param x1 End x coordinate
 * @param y1 End y coordinate
 * @param start_number The first number in the series
 * @param end_number The last number in the series
 * @param radius Radius for pixel dots when drawing number characters
 * @param intensity_percent Intensity of points in the range 0-100
 * @param grid_x Number of cells across the grid
 * @param grid_y Number of cells down the grid
 * @param img Array containing scope image
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 * @param text_size_pixels Size of each number character
 * @param no_of_increments How many numbers along the line
 */
void scope_number_line(scope * s,
                       int x0, int y0, int x1, int y1,
                       int start_number, int end_number,
                       unsigned int radius,
                       double intensity_percent,
                       int grid_x, int grid_y,
                       unsigned char img[],
                       unsigned int width, unsigned int height,
                       unsigned char shape,
                       int text_size_pixels,
                       int no_of_increments)
{
    unsigned int border_y = height*s->border_percent/100;
    int i, x, y, value;
    int dx = x1 - x0;
    int dy = y1 - y0;
    int offset_x = 0;
    char numstr[64];
    int offset_y =
        (((height - (border_y*2))/grid_y)/2) - text_size_pixels;
    if (abs(dx) < abs(dy)) {
        /* vertical */
        offset_x = -(text_size_pixels/2);
        offset_y = 0;
    }

    if ((start_number == 0) && (end_number == 0))
        end_number = 1;

    for (i = 0; i < no_of_increments; i++) {
        x = x0 + (dx*i/(no_of_increments-1)) + offset_x;
        y = y0 + (dy*i/(no_of_increments-1)) + offset_y;
        value = start_number +
            ((end_number - start_number)*i/(no_of_increments-1));

        /*
        if (abs(dx) > abs(dy))
            value *= s->horizontal_multiplier;
        */

        if ((i > 0) && (value == 0)) {
            sprintf(numstr, "%.1f",
                    (float)start_number +
                    ((end_number - start_number)*i/
                     (float)(no_of_increments-1)));
        }
        else {
            sprintf(numstr, "%d", value);
            if (abs(value) >= 1000)
                sprintf(numstr, "%.1fK", value/1000.0f);
            if (abs(value) >= 1000000)
                sprintf(numstr, "%.1fM", value/1000000.0f);
            if (abs(value) >= 1000000000)
                sprintf(numstr, "%.1fG", value/1000000000.0f);
        }

        if (abs(dx) < abs(dy))
            x -= (strlen(numstr)*text_size_pixels);
        else
            x -= (strlen(numstr)*text_size_pixels/2);
        scope_text(numstr, s, x, y,
                   text_size_pixels, text_size_pixels, 100,
                   img, width, height);
    }
}

/**
 * @brief Draws a scope with axes and titles
 * @param s Oscilloscope instance
 * @param draw_type Drawing mode
 * @param radius Radius for drawing points
 * @param intensity_percent Intensity of points in the range 0-100
 * @param grid_x Number of cells across the grid
 * @param grid_y Number of cells down the grid
 * @param img Array containing scope image
 * @param width Width of the scope image
 * @param height Height of the scope image
 * @param shape Shape of the screen, rectangular or circular
 * @param title Title text
 * @param vertical_text Text on the vertical axis
 * @param horizontal_text Text on the horizontal axis
 * @param text_size_pixels Size of each number character
 * @param no_of_increments How many numbers along the line
 */
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
                      int no_of_increments)
{
    unsigned int x0,y0,x1,y1;
    unsigned int border_y = height*s->border_percent/100;
    unsigned int border_x = width*s->border_percent/100;

    /* if this is a circular scope then clear the background */
    if (shape == PHOSPHENE_SHAPE_CIRCULAR)
        memset((void*)img, '\0', width*height*3);

    scope_draw_bounded(s, draw_type,
                       intensity_percent,
                       grid_x, grid_y,
                       img, 0, 0, width, height,
                       width, height, shape, 1);

    if ((shape != PHOSPHENE_SHAPE_CIRCULAR) &&
        ((draw_type == PHOSPHENE_DRAW_ALL) ||
         (draw_type == PHOSPHENE_DRAW_BACKGROUND))) {

        /* horizontal */
        x0 = border_x + ((width-(border_x*2))*1/grid_x);
        y0 = border_y + ((height-(border_y*2))*(grid_y-1)/grid_y);
        x1 = border_x + ((width-(border_x*2))*(grid_x-1)/grid_x);
        y1 = border_y + ((height-(border_y*2))*(grid_y-1)/grid_y);
        scope_trace_line(s, x0, y0, x1, y1,
                         radius, intensity_percent,
                         img, 0,0, width, height,
                         width, height, shape);

        if (s->mode == PHOSPHENE_MODE_DEFAULT) {
            scope_number_line(s, x0, y0, x1, y1,
                              0, (int)s->time_ms,
                              radius, intensity_percent,
                              grid_x, grid_y, img,
                              width, height, shape,
                              text_size_pixels*5/10, no_of_increments);
        }
        else {
            scope_number_line(s, x0, y0, x1, y1,
                              (int)s->trace1_min, (int)s->trace1_max,
                              radius, intensity_percent,
                              grid_x, grid_y, img,
                              width, height, shape,
                              text_size_pixels*5/10, no_of_increments);
        }

        /* vertical */
        x1 = x0;
        y1 = border_y + ((height-(border_y*2))*1/grid_y);
        scope_trace_line(s, x0, y0, x1, y1,
                         radius, intensity_percent,
                         img, 0,0, width, height,
                         width, height, shape);

        if (s->mode == PHOSPHENE_MODE_DEFAULT) {
            scope_number_line(s, x0, y0, x1, y1,
                              (int)s->trace1_min, (int)s->trace1_max,
                              radius, intensity_percent,
                              grid_x, grid_y, img,
                              width, height, shape,
                              text_size_pixels*5/10, no_of_increments);
        }
        else {
            scope_number_line(s, x0, y0, x1, y1,
                              (int)s->trace2_min, (int)s->trace2_max,
                              radius, intensity_percent,
                              grid_x, grid_y, img,
                              width, height, shape,
                              text_size_pixels*5/10, no_of_increments);
        }


        scope_text(title, s,
                   border_x + ((width - (border_x*2))/2) -
                   (strlen(title)*text_size_pixels/2),
                   border_y +
                   (((height - (border_y*2))/grid_y)-text_size_pixels),
                   text_size_pixels, text_size_pixels, 100,
                   img, width, height);

        text_size_pixels = text_size_pixels*6/10;

        scope_text(horizontal_text, s,
                   border_x + ((width - (border_x*2))/2) -
                   (strlen(horizontal_text)*text_size_pixels/2),
                   height - border_y - text_size_pixels,
                   text_size_pixels, text_size_pixels, 100,
                   img, width, height);

        scope_text_vertical(vertical_text, s,
                            border_x,
                            border_y + ((height - (border_y*2))/2) +
                            (strlen(vertical_text)*text_size_pixels/2),
                            text_size_pixels, text_size_pixels, 100,
                            img, width, height);
    }
}
