#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char **argv) {
    FILE *filePointer = NULL;
    size_t bufferLength = 0;
    char *buffer = NULL;
    char *filename = "data.src";
    int option;
    size_t stringIndex = 0;

    // Parse command line options
    while ((option = getopt(argc, argv, "i:")) != -1) {
        switch (option) {
            case 'i':
                filename = optarg;
                break;
            case ':':
            case '?':
                exit(1);
                break; // Extra sanity check; avoids compiler warning
        }
    }

    // Open the file
    filePointer = fopen(filename, "r");
    if (filePointer == NULL) {
        fprintf(stderr, "couldn't open data file\n");
        exit(1);
    }

    // Initialize string index
    stringIndex = 1;
    while (getdelim(&buffer, &bufferLength, '\0', filePointer) != -1) {
        // Print index and string, handling special case for strings starting with '\a'
        printf("<%zu>\n%s\n", stringIndex, *buffer == '\a' ? buffer + 1 : buffer);
        ++stringIndex;
    }

    // Close the file
    fclose(filePointer);
    filePointer = NULL;

    // Note: The following block is kept for documentation purposes but is not executed
    #if 0
    free(buffer);
    buffer = NULL;
    rewind(filePointer);

    if (getdelim(&buffer, &bufferLength, EOF, filePointer) != -1) {
        printf("\n34: %s\n", buffer + 34);
    }

    free(buffer);
    buffer = NULL;
    #endif

    return 0;
}