#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
int main(int argc, char *argv[])
{
    // Command-Line-Argument count takes = 2
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE");
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");

    // Check if the forensic image could be read / openned
    if (input_file == NULL)
    {
        printf("Unable to read file: %s\n", argv[1]);
        return 2;
    }

    // Read every 512byte into buffer array, to check 512bytes at a time, starting from 0
    int count = 0;
    int BLOCK_SIZE = 512;
    // Declare the buffer array to point
    unsigned char buffer[512];
    
    // Declare a the output pointer file
    FILE *output_file = NULL;
    // Allocate the required memory // remember to free filename memory at the end
    char *filename = malloc(8 * sizeof(char));

    while (fread(buffer, 1, BLOCK_SIZE, input_file) == BLOCK_SIZE)
    {

        // Check every 512byte memory block if the first 4 bytes are JPEG headers sequence: (00xff 0xd8 0xff 0xe-)
        // Search until you arrive at a new JPEG file. If so, close the first one and start writing the new one
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Print / write image JPEG file with the correct naming standard 001.jpg..etc
            sprintf(filename, "%03i.jpg", count);

            // Open output file 
            output_file = fopen(filename, "w");

            // Counter increase with every jpeg found
            count++;
        }

        if (output_file != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, output_file);
        }
    }

    free(filename);
    fclose(input_file);
    fclose(output_file);

}



