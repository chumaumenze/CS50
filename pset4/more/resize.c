/*
 * resize.c
 * 
 * This program resizes a 24-bit bitmap image.
 *
 * Usage: ./resize size small.bmp large.bmp
 *
 * Copyright (c) 2017 Chuma Umenze
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "bmp.h"

#define BMPFH sizeof(BITMAPFILEHEADER)
#define BMPIH sizeof(BITMAPINFOHEADER)
#define RGBTRI sizeof(RGBTRIPLE)

bool resize(float size, char *small, char *large);

int main(int argc, char *argv[])
{
    //check for commandline arguments
    if(argc != 4)
    {
        fprintf(stderr, "Usage: ./resize size original.bmp modified.bmp\n");
        return 1;
    }
    
    //store size and filenames
    float size = fabs( atof(argv[1]) );
    char *original = argv[2];
    char *modified = argv[3];

    // hold return value of resize funtion
    bool stats = false;

    if (size == 0.0)
    {
        stats = resize(ceil(size + .1), original, modified);
    }
    else
    {
        stats = resize(ceil(size), original, modified);
    }

    /*if(size == 1.0)
    {
        stats = resize(ceil(size), original, modified);
    }
    else
    {
        stats = resize(ceil(size), original, "temp.bmp");
    }

    if(stats && size != 1.0)
    {
        resize(size, "temp.bmp", modified);
    }*/
    
    return (stats) ? 0 : 1;
}


/************************************ Custom Functions ************************************/


bool resize(float size, char *small, char *large)
{
    if(size < 0.0 || size > 100.0)
    {
        fprintf(stderr, "resize: The size parameter must be a float greater than 0.0 but less than 100.0\n");
        return false;
    }
    
    FILE *fsmall = fopen(small, "r");
    
    if(fsmall == NULL)
    {
        fprintf(stderr, "resize: cannot stat '%s': No such file or directory\n", small);
        return false;
    }
    
    FILE *flarge = fopen(large, "w");
    
    if(flarge == NULL)
    {
        fprintf(stderr, "resize: cannot create '%s'\n", large);
        return false;
    }
    
    // reading fsmall's BMP file header
    BITMAPFILEHEADER large_bf;

    fread(&large_bf, BMPFH, 1, fsmall);
    
    // reading fsmall's BMP info header
    BITMAPINFOHEADER large_bi;

    fread(&large_bi, BMPIH, 1, fsmall);
    
    // ensuring file is a 24-bit uncompressed BMP image
    if (large_bf.bfType != 0x4d42 || large_bf.bfOffBits != 54 || large_bi.biSize != 40 || large_bi.biBitCount != 24 || large_bi.biCompression != 0)
    {
        // closing open files
        fclose(fsmall);
        fclose(flarge);
        
        fprintf(stderr, "resize: This does not look like a 24-bit uncompressed BMP image\nresize: Exiting with failure status due to previous errors\n");
        return false;
    }
    
    // determine the padding used in small image... if any
    int small_padding = (4 - ((large_bi.biWidth * RGBTRI) % 4)) % 4;

    // determine if padding is required for the large image
    int large_padding = (4 - ((int)(large_bi.biWidth * size * RGBTRI) % 4)) % 4;

    //update large_bf and large_bi headers before writing
    large_bi.biWidth *= size;
    large_bi.biHeight *= size;
    large_bi.biSizeImage = ((RGBTRI * large_bi.biWidth) + large_padding) * abs(large_bi.biHeight);
    large_bf.bfSize = large_bi.biSizeImage + BMPFH + BMPIH;
    
    // writing fsmall's BMP file header to flarge
    fwrite(&large_bf, BMPFH, 1, flarge);
    
    // writing fsmall's BMP info header to flarge
    fwrite(&large_bi, BMPIH, 1, flarge);


    /****************************/

    // Declaring height and width variables for both small and large image
    int fsmall_height = abs(large_bi.biHeight) / size;
    int fsmall_width = large_bi.biWidth / size;
    // int flarge_height = large_bi.biHeight;
    // int flarge_width = large_bi.biWidth;

    //size flarge row in bytes
    int flarge_row = (RGBTRI * fsmall_width) * size; 

    // Array to hold pixels in fsmall * n times
    RGBTRIPLE *temp_triple = (RGBTRIPLE *) malloc(flarge_row);
    
    for (int i = 0; i < fsmall_height; i++)
    {
        RGBTRIPLE pixel;
        for (int j = 0; j < fsmall_width; j++)
        {
            // copy a pixel
            fread(&pixel, RGBTRI, 1, fsmall);

            // write pixel to array n times
            for (int dup = (j * size); dup < (j * size) + size; dup++)
            {
                *(temp_triple + dup) = pixel;
            }
        }

        fseek(fsmall, small_padding, SEEK_CUR);

        // paste array / new row to flarge and add padding if needed
        for (int k = 0; k < size; k++)
        {
            fwrite(temp_triple, (flarge_row), 1, flarge);

            // add padding
            for (int m = 0; m < large_padding; m++)
            {
                fputc(0x00, flarge);
            }
        }
    }

    free(temp_triple);


    /****************************/

    fclose(fsmall);
    fclose(flarge);
    return true;
}