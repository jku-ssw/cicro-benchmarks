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

#include "deeplearn_images.h"

/**
 * @brief Converts a float image to one which can be displayed
 *        The float image is assumed to contain normalised values
 *        in the range 0.0 - 1.0
 * @param float_img An image array of floats
 * @param float_img_depth Depth of the floats image
 * @param img_width Width of the image
 * @param img_height Height of the image
 * @param img returned image with the same resolution
 * @param img_depth_bits Depth of the returned image
 */
void deeplearn_float_to_img(float float_img[],
                            int float_img_depth,
                            int img_width,
                            int img_height,
                            unsigned char img[],
                            int img_depth_bits)
{
    int img_depth_bytes = img_depth_bits/8;

    COUNTDOWN(i, img_width*img_height) {
        COUNTDOWN(j, img_depth_bytes) {
            int k = j*float_img_depth/img_depth_bytes;
            img[i*img_depth_bytes + j] =
                (unsigned char)(float_img[i*float_img_depth + k]*255);
        }
    }
}

/**
 * @brief Reads a PNG file into a buffer
 * @param filename Filename of the image
 * @param width Returned width of the image in pixels
 * @param height Returned height of the image in pixels
 * @param bitsperpixel Returned number of bits per pixel
 * @param buffer Pointer to buffer which contains the image data
 * @returns 0 on success
 */
int deeplearn_read_png_file(char * filename,
                            unsigned int * width,
                            unsigned int * height,
                            unsigned int * bitsperpixel,
                            unsigned char ** buffer)
{
    unsigned error;
    unsigned w=0, h=0;

    error = lodepng_decode24_file(buffer, &w, &h, filename);
    if (error) {
        printf("read_png_file: error %u: %s\n", error,
               lodepng_error_text(error));
        return -1;
    }

    *width = w;
    *height = h;
    *bitsperpixel=24;
    return 0;
}

/**
 * @brief Saves an image buffer to a PNG formatted image
 * @param filename Filename of the image
 * @param width Width of the image in pixels
 * @param height Height of the image in pixels
 * @param bitsperpixel Number of bits per pixel
 * @param buffer Image buffer
 * @return 0 on success
 */
int deeplearn_write_png_file(char * filename,
                             unsigned int width, unsigned int height,
                             unsigned int bitsperpixel,
                             unsigned char buffer[])
{
    unsigned error=1;
    unsigned char * image = buffer;

    if (bitsperpixel == 32)
        error = lodepng_encode32_file(filename, image, width, height);

    if (bitsperpixel == 24)
        error = lodepng_encode24_file(filename, image, width, height);

    if (bitsperpixel == 8) {
        UCHARALLOC(image, width*height*3);
        if (image) {
            COUNTDOWN(i, width*height)
                COUNTDOWN(d, 3)
                    image[i*3+d] = buffer[i];

            error = lodepng_encode24_file(filename, image, width, height);
            free(image);
        }
        else {
            return -2;
        }
    }

    if (error) {
        printf("write_png_file: error %u: %s\n", error,
               lodepng_error_text(error));
        return -1;
    }
    return 0;
}

/**
 * @brief Returns the number of images within the given directory having
 *        a given extension
 * @param images_directory The directory to search within
 * @param extension Filename extension (eg. "png")
 * @return Number of images found
 */
static int number_of_images(char * images_directory,
                            char * extension)
{
    int ctr,no_of_images = 0;
    struct dirent **namelist;
    int n,len;

    /* get image filenames */
    n = scandir(images_directory, &namelist, 0, alphasort);
    if (n >= 0) {
        /* count the number of image files */
        ctr = n;
        while (ctr--) {
            /* is the filename long enough? */
            len = strlen(namelist[ctr]->d_name);
            if (len > 4) {
                if (string_ends_with_extension(namelist[ctr]->d_name,
                                               extension))
                    no_of_images++;
            }
            free(namelist[ctr]);
        }
        free(namelist);
    }
    return no_of_images;
}

/**
 * @brief Downsample a colour image to a mono fixed size image
 * @param img Original image buffer (3 bytes per pixel)
 * @param width Width of the original image in pixels
 * @param height Height of the original image in pixels
 * @param downsampled Downsampled image buffer (1 byte per pixel)
 * @param downsampled_width Downsampled image width in pixels
 * @param downsampled_height Downsampled image height in pixels
 */
void deeplearn_downsample_colour_to_mono(unsigned char img[],
                                         int width, int height,
                                         unsigned char downsampled[],
                                         int downsampled_width,
                                         int downsampled_height)
{
    int n=0;

    COUNTUP(y, downsampled_height) {
        /* y coordinate in the original image */
        int yy = y * height / downsampled_height;
        COUNTUP(x, downsampled_width) {
            /* x coordinate in the original image */
            int xx = x * width / downsampled_width;
            /* index within the original image */
            int n2 = ((yy*width) + xx)*3;
            /* update downsampled image */
            downsampled[n] = (img[n2]+img[n2+1]+img[n2+2])/3;
            n++;
        }
    }
}

/**
 * @brief Downsample a colour image to a fixed size image
 * @param img Original image buffer (3 bytes per pixel)
 * @param width Width of the original image in pixels
 * @param height Height of the original image in pixels
 * @param downsampled Downsampled image buffer (3 bytes per pixel)
 * @param downsampled_width Downsampled image width in pixels
 * @param downsampled_height Downsampled image height in pixels
 */
void deeplearn_downsample_colour(unsigned char img[],
                                 int width, int height,
                                 unsigned char downsampled[],
                                 int downsampled_width,
                                 int downsampled_height)
{
    int n=0;

    COUNTUP(y, downsampled_height) {
        /* y coordinate in the original image */
        int yy = y * height / downsampled_height;
        COUNTUP(x, downsampled_width) {
            /* x coordinate in the original image */
            int xx = x * width / downsampled_width;
            /* index within the original image */
            int n2 = ((yy*width) + xx)*3;
            /* update downsampled image */
            COUNTDOWN(d, 3) {
                downsampled[n+d] = img[n2+d];
            }
            n+=3;
        }
    }
}

/**
 * @brief Loads a set of training images and automatically creates
 *        classification descriptions from the filenames and
 *        classification numbers
 * @param images_directory The directory to search for images
 * @param images Array which will be used to store the images (1 byte/pixel)
 * @param classifications Description of each image, taken from the filename
 * @param classification_number Class number of each image
 * @param width Standardised width of the images in pixels
 * @param height Standardised height of the images in pixels
 * @param extra_synthetic_images The number of extra synthetic images
 *        to generate for each loaded image
 * @return The number of images loaded
 */
int deeplearn_load_training_images(char * images_directory,
                                   unsigned char *** images,
                                   char *** classifications,
                                   int ** classification_number,
                                   int width, int height,
                                   int extra_synthetic_images)
{
    int ctr, no_of_images = 0;
    struct dirent **namelist;
    int n, len, centre_x, centre_y, x_spare, y_spare;
    unsigned int im_width=0, im_height=0;
    unsigned int im_bitsperpixel=0;
    unsigned char * img, * img2, * downsampled;
    char * extension = "png";
    char filename[512];
    char * classification;
    unsigned int random_seed = 763528;
    float scale;

    /* how many images are there? */
    no_of_images = number_of_images(images_directory, extension);
    if (no_of_images == 0)
        return 0;

    /* add the total number of synthetic images */
    no_of_images += (extra_synthetic_images*no_of_images);

    /* allocate an array for the images */
    UCHARPTRALLOC(*images, no_of_images);
    if (!images)
        return -1;

    /* allocate memory for the classifications */
    CHARPTRALLOC(*classifications, no_of_images);
    if (!classifications) {
        free(*images);
        return -2;
    }

    /* allocate memory for the class number assigned to each image */
    INTALLOC(*classification_number, no_of_images);
    if (!classification_number) {
        free(*classifications);
        free(*images);
        return -3;
    }

    /* get image filenames */
    no_of_images = 0;
    n = scandir(images_directory, &namelist, 0, alphasort);
    if (n >= 0) {
        /* for every filename */
        ctr = n;
        while (ctr--) {
            /* is the filename long enough? */
            len = strlen(namelist[ctr]->d_name);
            if (len > 4) {
                sprintf(filename,"%s/%s",
                        images_directory,namelist[ctr]->d_name);
                len = strlen(filename);
                /* is this a png image? */
                if (string_ends_with_extension(filename, extension)) {
                    downsampled = NULL;

                    /* obtain an image from the filename */
                    if (deeplearn_read_png_file(filename,
                                                &im_width, &im_height,
                                                &im_bitsperpixel, &img) == 0) {
                        /* create a fixed size image */
                        UCHARALLOC(downsampled, width*height);
                        if (!downsampled) {
                            COUNTDOWN(i, no_of_images) {
                                if ((*images)[i] != NULL)
                                    free((*images)[i]);
                            }
                            free(*classifications);
                            free(*images);
                            return -4;
                        }

                        deeplearn_downsample_colour_to_mono(img, (int)im_width,
                                                            (int)im_height,
                                                            downsampled,
                                                            width, height);

                        (*images)[no_of_images] = downsampled;

                        /* free the original image */
                        free(img);
                    }
                    else {
                        (*images)[no_of_images] = NULL;
                    }

                    /* allocate memory for the classification */
                    CHARALLOC(classification, 256);
                    if (!classification) {
                        COUNTDOWN(i, no_of_images) {
                            if ((*images)[i] != NULL)
                                free((*images)[i]);
                        }
                        free(*classifications);
                        free(*images);
                        return -5;
                    }

                    /* get the name of the classification */
                    bp_get_classification_from_filename(filename,
                                                        classification);
                    (*classifications)[no_of_images] = classification;

                    no_of_images++;

                    /* create some synthetic images for the
                       same classification */
                    if (downsampled == NULL)
                        continue;

                    COUNTDOWN(s, extra_synthetic_images) {
                        UCHARALLOC(img2, width*height);
                        if (img2 != NULL) {
                            /* scaling factor 0.5 -> 1.0 */
                            scale = 0.5f +
                                ((rand_num(&random_seed)%100000)/200000.0f);

                            /* spare room for translation */
                            x_spare = 1 + (int)((1.0f - scale)*im_width);
                            y_spare = 1 + (int)((1.0f - scale)*im_height);

                            /* new centre */
                            centre_x = (int)(im_width/2) - (x_spare/2) +
                                ((int)rand_num(&random_seed)%x_spare);
                            centre_y = (int)(im_height/2) - (y_spare/2) +
                                ((int)rand_num(&random_seed)%y_spare);

                            image_synth(img, (int)im_width, (int)im_height,
                                        (int)(im_bitsperpixel/8),
                                        scale, centre_x, centre_y,
                                        width, height, 1, img2);
                            (*images)[no_of_images] = img2;
                        }
                        else {
                            COUNTDOWN(i, no_of_images) {
                                if ((*images)[i] != NULL)
                                    free((*images)[i]);
                            }
                            free(*classifications);
                            free(*images);
                            return -6;
                        }

                        CHARALLOC(classification, 256);
                        if (!classification) {
                            COUNTDOWN(i, no_of_images+1) {
                                if ((*images)[i] != NULL)
                                    free((*images)[i]);
                            }
                            free(*classifications);
                            free(*images);
                            return -7;
                        }

                        (*classifications)[no_of_images] = classification;

                        no_of_images++;
                    }
                }
            }
            free(namelist[ctr]);
        }
        free(namelist);
    }

    /* assign a class number to each image */
    bp_classifications_to_numbers(no_of_images,
                                  *classifications,
                                  (*classification_number));

    return no_of_images;
}

/**
 * @brief Plots a number of mono images within a single image
 * @param images Array of images (1 byte per pixel)
 * @param no_of_images The number of images in the array
 * @param image_width Standardised image width within the array
 * @param image_height Standardised image height within the array
 * @param filename Filename to save as
 */
/* plots mono images */
void bp_plot_images(unsigned char **images,
                    int no_of_images,
                    int image_width, int image_height,
                    char * filename)
{
    /* allocate memory for the image */
    unsigned char * img;

    UCHARALLOC(img, image_width*image_height*no_of_images*3);
    if (!img)
        return;

    COUNTDOWN(i, no_of_images) {
        COUNTDOWN(y, image_height) {
            COUNTDOWN(x, image_width) {
                int n1 = ((y+(i*image_height))*image_width + x)*3;
                int n2 = (y*image_width) + x;
                img[n1] = images[i][n2];
                img[n1+1] = img[n1];
                img[n1+2] = img[n1];
            }
        }
    }

    /* write the image to file */
    deeplearn_write_png_file(filename,
                             image_width, image_height*no_of_images,
                             24, img);

    /* free the image memory */
    free(img);
}

/**
 * @brief Resizes an image, if trying to convolve high resolution images
 *        is too hard
 * @param img Image array
 * @param image_width Width of the image
 * @param image_height Height of the image
 * @param image_depth Depth of the image
 * @param result Resized image array
 * @param result_width Output image width
 * @param result_height Output image height
 * @param result_depth Output image depth
 * @returns zero on success
 */
int image_resize(unsigned char img[],
                 int image_width, int image_height, int image_depth,
                 unsigned char result[],
                 int result_width, int result_height, int result_depth)
{
    if ((image_depth != result_depth) && (result_depth != 1))
        return 1;

    COUNTDOWN(y, result_height) {
        int y_orig = y * image_height / result_height;
        COUNTDOWN(x, result_width) {
            int x_orig = x * image_width / result_width;
            int n = (y*result_width + x) * result_depth;
            int n_orig = (y_orig*image_width + x_orig) * image_depth;
            if (result_depth == 1) {
                int sum = 0;
                COUNTDOWN(d, image_depth)
                    sum += (int)img[n_orig + d];
                result[n] = (unsigned char)(sum / image_depth);
            }
            else {
                if (result_depth == image_depth) {
                    COUNTDOWN(d, image_depth)
                        result[n+d] = img[n_orig + d];
                }
            }
        }
    }
    return 0;
}

/**
 * @brief Scales and translates an image.
 *        This can be used to generate synthetic training images
 * @param img Image array
 * @param image_width Width of the image
 * @param image_height Height of the image
 * @param image_depth Depth of the image
 * @param scale scaling factor where 1.0 is 100% of original
 * @param centre_x x centre of scaling in pixels in input image
 * @param centre_y y centre of scaling in pixels in input image
 * @param result_width Width of the scaled image
 * @param result_height Height of the scaled image
 * @param result_depth Depth of the scaled image
 * @param result returned scaled image
 */
void image_synth(unsigned char img[],
                 int image_width, int image_height, int image_depth,
                 float scale, int centre_x, int centre_y,
                 int result_width, int result_height, int result_depth,
                 unsigned char result[])
{
    COUNTDOWN(y, result_height) {
        int y_original = y * image_height / result_height;
        int scaled_y = centre_y + (int)(scale*(y_original - centre_y));
        if ((scaled_y >= 0) && (scaled_y < image_height)) {
            int n = y*result_width*result_depth;
            COUNTDOWN(x, result_width) {
                int x_original = x * image_width / result_width;
                int scaled_x = centre_x + (int)(scale*(x_original - centre_x));
                if ((scaled_x >= 0) && (scaled_x < image_width)) {
                    if (result_depth == image_depth) {
                        COUNTDOWN(d, result_depth) {
                            result[n + d] =
                                img[((scaled_y*image_width) + scaled_x)*
                                    image_depth + d];
                        }
                    }
                    else {
                        int sum = 0;
                        COUNTDOWN(d, image_depth) {
                            sum +=
                                img[((scaled_y*image_width) + scaled_x)*
                                    image_depth + d];
                        }
                        result[n] = (unsigned char)(sum/image_depth);
                    }
                }
                n += result_depth;
            }
        }
    }
}
