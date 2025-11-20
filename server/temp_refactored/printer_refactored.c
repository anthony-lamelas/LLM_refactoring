#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#define Z(x) ((x) * BLOCKSIZE)
#define V(x) puts(&y[Z(x)])

static char *y = NULL;
static char *filename = "data";
static size_t len = 0;
static int option = 0;

int main(int argc, char **argv) {
    FILE *file_ptr = NULL;

    // Parse command-line options
    while ((option = getopt(argc, argv, "i:")) != -1) {
        switch (option) {
            case 'i':
                filename = optarg;
                break;
            case ':':
            case '?':
                // Exit on missing option argument or unknown option
                exit(EXIT_FAILURE);
                break;
        }
    }

    errno = 0;
    file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        fprintf(stderr, "Couldn't open data file %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Read the entire file into memory
    if (getdelim(&y, &len, EOF, file_ptr) != -1) {
        // Include the generated strings validation code
        #include "V.c"
    }

    // Clean up resources
    free(y);
    y = NULL;
    fclose(file_ptr);
    file_ptr = NULL;

    return 0;
}