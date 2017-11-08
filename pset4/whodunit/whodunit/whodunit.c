#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

#define BMPFH sizeof(BITMAPFILEHEADER)
#define BMPIH sizeof(BITMAPINFOHEADER)
#define RGBTRI sizeof(RGBTRIPLE)

int main(int argc, char *argv[])
{
    //check for commandline arguments
    if(argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit clue.bmp verdict.bmp\n");
        return 1;
    }
    
    //store file names
    char *clue = argv[1];
    char *verdict = argv[2];
    
    FILE *fclue = fopen(clue, "r");
    
    if(fclue == NULL)
    {
        fprintf(stderr, "whodunit: cannot stat '%s': No such file or directory\n", clue);
        return 2;
    }
    
    FILE *fverdict = fopen(verdict, "w");
    
    if(fverdict == NULL)
    {
        fprintf(stderr, "whodunit: cannot create '%s'\n", verdict);
        return 3;
    }
    
    // reading fclue's BMP file header
    BITMAPFILEHEADER clue_bf;
    fread(&clue_bf, BMPFH, 1, fclue);
    
    // reading fclue's BMP info header
    BITMAPINFOHEADER clue_bi;
    fread(&clue_bi, BMPIH, 1, fclue);
    
    // ensuring file is a 24-bit uncompressed BMP image
    if (clue_bf.bfType != 0x4d42 || clue_bf.bfOffBits != 54 || clue_bi.biSize != 40 || clue_bi.biBitCount != 24 || clue_bi.biCompression != 0)
    {
        // closing open files
        fclose(fclue);
        fclose(fverdict);
        
        fprintf(stderr, "whodunit: This does not look like a 24-bit uncompressed BMP image\nwhodunit: Exiting with failure status due to previous errors\n");
        return 4;
    }
    
    // writing fclue's BMP file header to fverdict
    fwrite(&clue_bf, BMPFH, 1, fverdict);
    
    // writing fclue's BMP info header to fverdict
    fwrite(&clue_bi, BMPIH, 1, fverdict);
    
    // determine the padding for the scanlines i.e. TODO: (clue_bi.biWidth (in pixels) * RGBTRI (3 bytes) )
    //int clue_padding = (4 - ((clue_bi.biWidth * RGBTRI) % 4)) % 4;
    
    // On each scanline...
    for(int i = 0, clue_height = abs(clue_bi.biHeight); i != clue_height; i++)
    {
        // iterate from left to right...
        for(int j = 0; j != clue_bi.biWidth; j++)
        {
	    RGBTRIPLE clue_tri;
	    
            //continue reading fclue's colour triple i.e. copy pixel by pixel
	    fread(&clue_tri, RGBTRI, 1, fclue);
	    
	    // check for red colour
	    if(clue_tri.rgbtRed == 0xff)
	    {
	        clue_tri.rgbtBlue = 0x33;
	        clue_tri.rgbtGreen = 0x33;
	        clue_tri.rgbtRed = 0x33;
	        // overwrite pixel with grey
	        fwrite(&clue_tri, RGBTRI, 1, fverdict);
	    }
	    else
	    {
	        //continue writing fclue's colour triple i.e. write pixel by pixel
	        fwrite(&clue_tri, RGBTRI, 1, fverdict);
	    }
        }
    }
    
    fclose(fclue);
    fclose(fverdict);
    return 0;
}
