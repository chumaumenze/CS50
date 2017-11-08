/*
 * resize.c
 * 
 * This program resizes a 24-bit bitmap image.
 *
 * Usage: ./resize size input.bmp output.bmp
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

bool resize(float size, char *input, char *output);

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

    stats = resize(size, original, modified);

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


bool resize(float size, char *input, char *output)
{
    if(size < 0.1 || size > 100.0)
    {
        fprintf(stderr, "resize: The size parameter must be a float greater than 0.0 but less than 100.0\n");
        return false;
    }

    printf("size is %.3f\n", size);
    
    FILE *finput = fopen(input, "r");
    
    if(finput == NULL)
    {
        fprintf(stderr, "resize: cannot stat '%s': No such file or directory\n", input);
        return false;
    }
    
    FILE *foutput = fopen(output, "w");
    
    if(foutput == NULL)
    {
        fprintf(stderr, "resize: cannot create '%s'\n", output);
        return false;
    }
    
    // reading finput's BMP file header
    BITMAPFILEHEADER output_bf;

    fread(&output_bf, BMPFH, 1, finput);
    
    // reading finput's BMP info header
    BITMAPINFOHEADER output_bi;

    fread(&output_bi, BMPIH, 1, finput);
    
    // ensuring file is a 24-bit uncompressed BMP image
    if (output_bf.bfType != 0x4d42 || output_bf.bfOffBits != 54 || output_bi.biSize != 40 || output_bi.biBitCount != 24 || output_bi.biCompression != 0)
    {
        // closing open files
        fclose(finput);
        fclose(foutput);
        
        fprintf(stderr, "resize: This does not look like a 24-bit uncompressed BMP image\nresize: Exiting with failure status due to previous errors\n");
        return false;
    }
    
    // determine the padding used in input image... if any
    int input_padding = (4 - ((output_bi.biWidth * RGBTRI) % 4)) % 4;

    printf("Amount of padding used in input image is %d\n", input_padding);

    // determine if padding is required for the output image
    int output_padding = (4 - ((int)(output_bi.biWidth * size * RGBTRI) % 4)) % 4;

    printf("Amount of padding required for the output image is %d\n", output_padding);

    // don't reduce image size if image is just a single pixel in width or height
    if ( size < 1.0 && (output_bi.biWidth == 1 || abs(output_bi.biHeight) == 1) )
    {
        // setting to 1, copies the image
        size = 1.0;
    }

    //update output_bf and output_bi headers before writing
    output_bi.biWidth *= size;
    output_bi.biHeight *= size;
    output_bi.biSizeImage = ((RGBTRI * output_bi.biWidth) + output_padding) * abs(output_bi.biHeight);
    output_bf.bfSize = output_bi.biSizeImage + BMPFH + BMPIH;

    printf("writing to output image file the following header details:\nbiWidth: %d\nbiHeight: %d\nbiSizeImage: %d\nbfSize: %d\n", output_bi.biWidth, output_bi.biHeight, output_bi.biSizeImage, output_bf.bfSize);
    
    // writing finput's BMP file header to foutput
    fwrite(&output_bf, BMPFH, 1, foutput);
    
    // writing finput's BMP info header to foutput
    fwrite(&output_bi, BMPIH, 1, foutput);


    /****************************/

    // Declaring height and width variables for both input and output image
    int input_height = abs(output_bi.biHeight) / size;
    int input_width = output_bi.biWidth / size;
    // int output_height = output_bi.biHeight;
    // int output_width = output_bi.biWidth;

    // determine if we are reducing image
    bool reduce = (output_bi.biWidth < input_width);

    //size of foutput's row (in bytes)
    int foutput_row = (RGBTRI * input_width) * size; 

    printf("\nAbout original image:\ninput_height: %d, input_width: %d, foutput_row: %d\n", input_height, input_width, foutput_row);

    // Array to hold pixels in finput * n times
    RGBTRIPLE *temp_triple = (RGBTRIPLE *) malloc(foutput_row);
    if (temp_triple == NULL)
    {
        fclose(finput);
        fclose(foutput);
        fprintf(stderr, "resize: Unable to allocate memory\n");
        return 1;
    }
    
    printf("\nProceeding to RGBTRIPLE...\n");
    for (int i = 0; i < input_height; i++)
    {
        RGBTRIPLE pixel;
        printf("Row %d\n", i % input_height);

        // Per each row (in the input file)
        for (int j = 0; j < input_width; j++)
        {
            // copy a pixel
            fread(&pixel, RGBTRI, 1, finput);

            printf("pixel:%d/%d/%d.\t", pixel.rgbtBlue, pixel.rgbtGreen, pixel.rgbtRed);

            // determine if we are reducing or increasing image
            if (reduce)
            {
                // write pixel to array n times
                for (int dup = (j * size); dup < (j * size) + size; dup++)
                {
                    *(temp_triple + dup) = pixel;
                    printf("(temp_triple + %d):%d/%d/%d\n", dup, (temp_triple + dup)->rgbtBlue, (temp_triple + dup)->rgbtGreen, (temp_triple + dup)->rgbtRed);
                }

                /** skip a few pixels (in the input file) **/
                fseek(finput, RGBTRI, SEEK_CUR);
                /** adjust counter **/
                j++;
            }
            else
            {
                // write pixel to array n times
                for (int dup = (j * size); dup < (j * size) + size; dup++)
                {
                    *(temp_triple + dup) = pixel;
                    printf("(temp_triple + %d):%d/%d/%d\n", dup, (temp_triple + dup)->rgbtBlue, (temp_triple + dup)->rgbtGreen, (temp_triple + dup)->rgbtRed);
                }
            }
            
        }

        // skip padding of the input file
        fseek(finput, input_padding, SEEK_CUR);

        // paste array / new row to foutput and add padding if needed
        printf("Writing array to foutput\n\n");
        for (int k = 0; k < size; k++)
        {
            fwrite(temp_triple, (foutput_row), 1, foutput);

            // add padding
            for (int m = 0; m < output_padding; m++)
            {
                fputc(0x00, foutput);
            }
        }

        // determine if we are reducing or increasing image
        if (reduce)
        {
            /* skip some rows in input file */
            fseek(finput, input_width, SEEK_CUR);
            /* adjust counter */
            //i++;
        }
    }

    printf("Freeing memory...\n");
    free(temp_triple);


    /****************************/

    printf("Closing open files\n");
    fclose(finput);
    fclose(foutput);
    return true;
}