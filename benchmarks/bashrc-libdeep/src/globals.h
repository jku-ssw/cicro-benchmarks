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

#ifndef DEEPLEARN_GLOBALS_H
#define DEEPLEARN_GLOBALS_H

#define DEEPLEARN_THREADS                 4

#undef PLOT_WITH_GNUPLOT
#define DEEPLEARN_PLOT_WIDTH              1024
#define DEEPLEARN_PLOT_HEIGHT             1024

#define DEEPLEARN_TEMP_DIRECTORY          "/tmp/"
#define DEEPLEARN_HISTORY_SIZE            8000
#define DEEPLEARN_UNKNOWN_ERROR           9999
#define DEEPLEARN_UNKNOWN_VALUE          -9999
#define DEEPLEARN_MAX_FIELD_LENGTH_CHARS  1024
#define DEEPLEARN_MAX_CSV_INPUTS          2048
#define DEEPLEARN_MAX_CSV_OUTPUTS         1024

/* The number of bits per character in a text string */
#define CHAR_BITS               (sizeof(char)*8)

#define AUTOCODER_UNKNOWN      -9999
#define AUTOCODER_DROPPED_OUT  -9999

#define NEURON_HIGH             0.75
#define NEURON_LOW              0.25
#define NEURON_UNKNOWN          0.5
#define NEURON_RANGE            (NEURON_HIGH - NEURON_LOW)

#define AF_SIGMOID              0
#define AF_TANH                 1
#define AF_LINEAR               2

#define ACTIVATION_FUNCTION     AF_SIGMOID

#if ACTIVATION_FUNCTION == AF_SIGMOID
#define AF(adder) (1.0f / (1.0f + exp(-(adder))))
#elif ACTIVATION_FUNCTION == AF_TANH
#define AF(adder) ((((2.0f / (1.0f + exp(-(2*adder)))) - 1.0f)*0.5f)+0.5f)
#elif ACTIVATION_FUNCTION == AF_LINEAR
#define AF(adder) ((adder) < 1.0f ? ((adder) > -1.0f ? \
                                     (((adder)*0.5f)+0.5f) : 0.0f) : 1.0f)
#endif

#define PIXEL_TO_FLOAT(p) (NEURON_LOW + ((p)*NEURON_RANGE/255.0f))

#define FOR(i, start, end) for (int (i) = (start); (i) < (end); (i)++)
#define COUNTUP(i, end) for (int (i) = 0; (i) < (end); (i)++)
#define COUNTDOWN(i, end) for (int (i) = (end-1); (i) >= 0; (i)--)

#define FLOATALLOC(m, size) m = (float*)malloc((size)*sizeof(float))
#define FLOATCLEAR(m, size) memset((void*)m, '\0', size*sizeof(float))

#define CHARALLOC(m, size) m = (char*)malloc((size)*sizeof(char))
#define UCHARALLOC(m, size) m = (unsigned char*)malloc((size)*sizeof(unsigned char))
#define UCHARPTRALLOC(m, size) m = (unsigned char**)malloc((size)*sizeof(unsigned char*))
#define CHARPTRALLOC(m, size) m = (char**)malloc((size)*sizeof(char*))
#define INTALLOC(m, size) m = (int*)malloc((size)*sizeof(int))
#define UINTALLOC(m, size) m = (unsigned int*)malloc((size)*sizeof(unsigned int))
#define UINTCLEAR(m, size) memset((void*)m, '\0', size*sizeof(unsigned int))

#define WRITEVAR(m, type) fwrite(&m, sizeof(type), 1, fp)
#define WRITEARRAY(m, type, size) fwrite(m, sizeof(type), size, fp)

#define FLOATWRITE(m) WRITEVAR(m, float)
#define FLOATWRITEARRAY(m, size) WRITEARRAY(m, float, size)
#define INTWRITEARRAY(m, size) WRITEARRAY(m, int, size)
#define INTWRITE(m) WRITEVAR(m, int)
#define UINTWRITE(m) WRITEVAR(m, unsigned int)
#define BYTEWRITE(m) WRITEVAR(m, unsigned char)

#define READVAR(m, type) fread(&m, sizeof(type), 1, fp)
#define READARRAY(m, type, size) fread(m, sizeof(type), size, fp)

#define FLOATREAD(m) READVAR(m, float)
#define FLOATREADARRAY(m, size) READARRAY(m, float, size)
#define INTREADARRAY(m, size) READARRAY(m, int, size)
#define INTREAD(m) READVAR(m, int)
#define UINTREAD(m) READVAR(m, unsigned int)
#define BYTEREAD(m) READVAR(m, unsigned char)

/* clip between min and max */
#define CLIP(x, min, max) ( ((x) > max) ? max : ( ((x) < min) ? (min) : (x) ) )
#define CLIP_WEIGHT(x) (CLIP((x), -2, 2))

#endif
