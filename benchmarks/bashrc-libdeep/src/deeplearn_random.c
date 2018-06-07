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

#include "deeplearn_random.h"

#define PRNG_LEHMER(x) (((unsigned long long)(x) * 279470273UL) % 4294967291UL)

/**
 * @brief Lehmer random number generator
 * @param seed Random number generator seed
 * @return Pseudo-random number
 */
unsigned int rand_num(unsigned int * seed)
{
    unsigned int v = PRNG_LEHMER(*seed);

    /* If the seed value is zero then further sequence generation
       stops and this is a singularity. We can avoid this by incrementing
       the value until a non-zero result occurs */
    if (v == 0) {
        v++;
        while (PRNG_LEHMER(v) == 0)
            v++;
    }

    /* new seed value */
    *seed = v;

    return v;
}

/**
 * @brief Returns a small random value used to initialise weights
 * @param seed Random number generator seed
 * @param no_of_inputs The number of input weights
 * @return Small random value
 */
float rand_initial_weight(unsigned int * seed, int no_of_inputs)
{
    float magnitude = 1.0f / no_of_inputs;
    return (magnitude*(rand_num(seed)%100000/100000.0f)*
            (rand_num(seed)%100000/100000.0f)) - (magnitude*0.5f);
}
