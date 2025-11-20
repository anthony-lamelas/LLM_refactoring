#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

int main(int argc, char **argv) {
    FILE *filePointer = NULL;
    size_t bufferLength = 0;
    char *buffer = NULL;
    char *filename = "data.src";
    int option;
    int byte;
    size_t stringIndex = 0;

    // Parse command-line options
    while ((option = getopt(argc, argv, "i:")) != -1) {
        switch (option) {
            case 'i':
                filename = optarg;
                break;
            case ':':
            case '?':
                exit(1);
                break; // This is an extra sanity check
        }
    }

    errno = 0;
    filePointer = fopen(filename, "r");
    if (filePointer == NULL) {
        fprintf(stderr, "couldn't open data file: %s: %s\n", filename, strerror(errno));
        exit(1);
    }

    // Initialize string index to 1 due to file padding
    stringIndex = 1;
    while (getdelim(&buffer, &bufferLength, '\0', filePointer) != -1) {
        printf("<%zu>\n", stringIndex);

        for (int index = 0; buffer[index]; ++index) {
            byte = buffer[index];

            // Check and skip multibyte characters
            if ((byte & 0xE0) == 0xC0) {
                ++index;
            } else if ((byte & 0xF0) == 0xE0) {
                index += 2;
            } else if ((byte & 0xF8) == 0xF0) {
                index += 3;
            } else {
                putchar(byte);
            }
        }
        ++stringIndex;
    }

    fclose(filePointer);
    filePointer = NULL;

    return 0;
}