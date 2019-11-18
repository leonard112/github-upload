// resizes a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "bmp.h"

void copy_scale_scanline(FILE *inptr, FILE *outptr, int old_width, int multiplier, int divisor, int old_padding, int padding, int pix_max);

int main(int argc, char *argv[])
{
    if (argc != 4) // ensure proper usage
    {
        fprintf(stderr, "Usage: copy scale infile outfile\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[1]); i++) //iterate through chars in argv[1] to make sure argv[1] is a number
    {
        if (argv[1][i] == '.') //if char is a decimal point.
        {
            break;
        }
        else if (isdigit(argv[1][i])) //if char is a number digit.
        {
            break;
        }
        else //invalid, scale must be number
        {
            fprintf(stderr, "Usage: scale argument must be a postive number\n");
            return 2;
        }
    }

    char arg2[5]; //store what should be file extension portion of argv[2] here
    memset(arg2, '\0', sizeof(arg2));
    strncpy(arg2, argv[2] + strlen(argv[2]) - 4, 4);

    char arg3[5]; //store what should be file extension portion of argv[4] here
    memset(arg3, '\0', sizeof(arg3));
    strncpy(arg3, argv[3] + strlen(argv[3]) - 4, 4);

    if (strcmp(arg2,".bmp") != 0 || strcmp(arg3,".bmp") != 0) //make sure argv[2] and argv[3] are files names with .bmp extension
    {
        fprintf(stderr, "Usage: file arguments must be be file names with .bmp file extensions.\n");
        return 3;
    }

    float scale = strtof(argv[1],NULL); //remember scale

    if (scale < 0.0 || scale > 100.0) //make sure scale is between 0 and 100.
    {
        fprintf(stderr, "Usage: scale argument must be between 0.0 and 100.0\n");
        return 4;
    }

    //save multipier portion and decimal poertion of scale
    int multiplier = (int)scale;
    float dec_portion = fmod(scale, 1.0);

    int divisor; //store the dcimal protion as a fraction
    if (dec_portion != 0) //if decimal portion is not zero
    {
        divisor = (int)1/dec_portion;
    }
    else
    {
        divisor = 0;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 5;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 6;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 7;
    }

    int old_width = bi.biWidth; //store old width
    int old_height = bi.biHeight; //store old height
    old_height = abs(old_height); //change old_height to absolute value

    //scale bi.biWidth and bi.biHeight
    if (divisor != 0)//if divisor not zero
    {
        bi.biWidth = bi.biWidth * multiplier + bi.biWidth/divisor;
        bi.biHeight = bi.biHeight * multiplier + bi.biHeight/divisor;
    }
    else //if divisor is zero
    {
        bi.biWidth = bi.biWidth * multiplier;
        bi.biHeight = bi.biHeight * multiplier;
    }

    int old_padding = (4 - (old_width * sizeof(RGBTRIPLE)) % 4) % 4; //old padding
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; //new padding

    bi.biSizeImage = ((bi.biWidth*sizeof(RGBTRIPLE))+padding) * abs(bi.biHeight); //update bisizeimage
    bf.bfSize = bi.biSizeImage + bf.bfOffBits; //update bfsize

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr); // write outfile's BITMAPFILEHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr); // write outfile's BITMAPINFOHEADER

    int counter = 0; //keeps track of how many scanlines are written.
    int scanline_max = abs(bi.biHeight); //maximum scanlines allowed to be written to file.

    for (int i = 0; i < old_height; i++) // iterate over infile's scanlines
    {
        for (int j = 0; j < multiplier; j++) //print the scanline the number of time specified by the multiplier.
        {
            if (counter < scanline_max) // if maximum number of scanlines not reached.
            {
                copy_scale_scanline(inptr, outptr, old_width, multiplier, divisor, old_padding, padding, bi.biWidth);
                counter++;
            }
        }

        if (divisor != 0 && counter < scanline_max) //if divisor is not zero and maximum num of scanlines not reached.
        {
            if (i % divisor == 0) //if there is no remainder
            {
                copy_scale_scanline(inptr, outptr, old_width, multiplier, divisor, old_padding, padding, bi.biWidth);
                counter++;
            }
        }

        fseek(inptr, (old_padding + old_width*sizeof(RGBTRIPLE)), SEEK_CUR); //advance to next line.
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

//copy and scale scanline from inptr to outptr. When done move SEEK_CUR back to initial position.
void copy_scale_scanline(FILE *inptr, FILE *outptr, int old_width, int multiplier, int divisor, int old_padding, int padding, int pix_max)
{
    int counter = 0; //keeps track of how many pixels are written

    for (int i = 0; i < old_width; i++) //iterate over current scanline's pixels
    {
        RGBTRIPLE triple; // temporary storage

        fread(&triple, sizeof(RGBTRIPLE), 1, inptr); // read RGB triple from infile

        for (int j = 0; j < multiplier; j++) //create amount of identical pixel specified by multiplier
        {
            if (counter < pix_max) //if scanline not full
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                counter++;
            }
        }

        if (divisor != 0 && counter < pix_max) //if divisor is not zero and scanline not full.
        {
            if (i % divisor == 0) //if there is no remainder
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                counter++;
            }
        }
    }

    fseek(inptr, -old_width*sizeof(RGBTRIPLE), SEEK_CUR); //go back to beginning of scanline.

    for (int k = 0; k < padding; k++)// add padding to new bitmap
    {
        fputc(0x00, outptr);
    }
}