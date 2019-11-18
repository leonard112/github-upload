//recover deleted jpg files from a forensic image

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2) // make sure a command line argument is entered
    {
        fprintf(stderr, "Exactly one forensic image command line argument must be entered.\n");
        return 1;
    }

    char *infile = argv[1];

    FILE *inptr = fopen(infile, "r"); // open input file for reading

    if (inptr == NULL) //if input file could not be opened
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    int counter = 0; //used to name recovered jpeg files.
    char filename[20]; //stores the name of a jpeg file

    unsigned char block[512]; //store a memory block of 512 bytes
    fread(&block, 1, 512, inptr); //read in memory block

    while (feof(inptr) == 0) //while the end of the file has not been reached.
    {
        //if the first 4 bytes of block is ff d8 ff (e0-ef)
        if ((int)block[0] == 255 && (int)block[1] == 216 && (int)block[2] == 255 && (int)block[3] >= 224 && (int)block[3] <= 239)
        {
            //create new file name
            if (counter < 10)
            {
                sprintf(filename, "00%d.jpg", counter);
            }
            else
            {
                sprintf(filename, "0%d.jpg", counter);
            }

            char *outfile = filename;  //name outfile

            FILE *outptr = fopen(outfile, "w"); //create outfile and open for writing

            if (outptr == NULL) //if output file could not be created
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", outfile);
                return 3;
            }

            fwrite(&block, 1, 512, outptr); //write block to output file
            fread(&block, 1, 512, inptr); //read next block

            //if next block is not end of file and next block doesnt start with ff d8 ff (e0-ef)
            while (!((int)block[0] == 255 && (int)block[1] == 216 && (int)block[2] == 255 && (int)block[3] >= 224 && (int)block[3] <= 239) &&
                    feof(inptr) == 0)
            {
                fwrite(&block, 1, 512, outptr); //write block to output file
                fread(&block, 1, 512, inptr); //read memory block
            }

            fclose(outptr); //close output file when done writing
            counter++;
        }
        else //read in another block
        {
            fread(&block, 1, 512, inptr); //read memory block
        }
    }

    fclose(inptr); // close infile

    return 0;
}