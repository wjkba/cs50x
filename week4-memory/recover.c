#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover [image]\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");

    uint8_t buffer[512];
    char filename[8];

    FILE *img = NULL;
    int counter = 0;
    bool writingJPEG = false;

    while (fread(buffer, 1, 512, input) == 512)
    {
        bool newJPEGFound = (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
                             (buffer[3] & 0xf0) == 0xe0);

        if (newJPEGFound)
        {
            if (writingJPEG)
            {
                fclose(img);
                counter++;
            }
            else
                writingJPEG = true;
            sprintf(filename, "%03d.jpg", counter);
            img = fopen(filename, "w");
            fwrite(buffer, 512, 1, img);
        }
        else if (writingJPEG)
        {
            fwrite(buffer, 512, 1, img);
        }
    }

    if (writingJPEG)
        fclose(img);

    fclose(input);

    return 0;
}
