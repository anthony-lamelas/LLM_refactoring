#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

// Global variables
static int testmode;

// Main function
int main(int argc, char **argv) {
    char *filename = "data.src";
    FILE *filePointer = NULL;
    char *buffer = NULL;
    char *longestString = NULL;
    size_t bufferLength = 0;
    size_t totalStrings = 0;
    size_t maxLength = 0;
    int option;
    int exitCode = 0;

    // Process command line options
    while ((option = getopt(argc, argv, "i:t")) != -1) {
        switch (option) {
            case 'i':
                filename = optarg;
                break;
            case 't':
                testmode = 1;
                break;
            case '?':
                exit(1);
                break;
        }
    }

    errno = 0;
    filePointer = fopen(filename, "r");
    if (filePointer == NULL) {
        fprintf(stderr, "couldn't open data file %s: %s\n", filename, strerror(errno));
        exit(1);
    }

    // Read strings from file and find the longest one
    while (getdelim(&buffer, &bufferLength, '\0', filePointer) != -1) {
        ++totalStrings;
        size_t currentLength = strlen(buffer);

        if (currentLength >= BLOCKSIZE) {
            if (testmode) {
                exitCode = 666;
            } else {
                printf("WARNING: string %zu is >= BLOCKSIZE length: %zu >= %d\n", totalStrings, currentLength, BLOCKSIZE);
            }
        }

        if (currentLength > maxLength) {
            maxLength = currentLength;
            free(longestString);
            longestString = strdup(buffer);
        }
    }

    // Output the result
    if (longestString) {
        if (!testmode) {
            printf("out of a total of %zu strings, %zu is the longest string:\n%s\n\n", totalStrings, maxLength, longestString);
        }
        free(longestString);
    } else {
        fprintf(stderr, "error: no string found!\n");
        exit(666);
    }

    // Handle test mode exit
    if (testmode && exitCode != 0) {
        fprintf(stderr, "failed testmode: run '%s -i %s' (without -t) to see problematic strings!", argv[0], filename);
        exit(exitCode);
    }

    fclose(filePointer);
    free(buffer);

    return 0;
}