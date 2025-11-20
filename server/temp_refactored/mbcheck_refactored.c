#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <stdint.h>

// Global variables
static unsigned loc = 0;
static char const *filename = "data.asc.src";
static uintmax_t unicode = 0;

// Main function
int main(int argc, char **argv) {
    FILE *fp = NULL;
    size_t len = 0;
    char *buffer = NULL;
    int option;
    int byte;
    size_t string_index = 1;  // Start indexing strings at 1
    uintmax_t codes;

    // Parse command line options
    while ((option = getopt(argc, argv, "i:l")) != -1) {
        switch (option) {
            case 'i':
                filename = optarg;
                break;
            case 'l':
                loc = 1;
                break;
            case ':':
            case '?':
                exit(1);
                break;
        }
    }

    // Open file
    errno = 0;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open data file %s: %s\n", filename, strerror(errno));
        exit(1);
    }

    // Read strings from file
    while (getdelim(&buffer, &len, '\0', fp) != -1) {
        codes = 0;
        for (size_t char_index = 0; buffer[char_index]; ++char_index) {
            byte = buffer[char_index];

            // Check for multibyte characters
            if ((byte & 0xF8) == 0xF0) {
                if (loc) printf("<%zu:%zu>\n", string_index, char_index);
                char_index += 3;
                ++unicode;
                ++codes;
            } else if ((byte & 0xF0) == 0xE0) {
                if (loc) printf("<%zu:%zu>\n", string_index, char_index);
                char_index += 2;
                ++unicode;
                ++codes;
            } else if ((byte & 0xE0) == 0xC0) {
                if (loc) printf("<%zu:%zu>\n", string_index, char_index);
                ++char_index;
                ++unicode;
                ++codes;
            }
        }

        // Report any found unicode characters
        if (codes > 0) {
            fprintf(stderr, "%4ju unicode character%s found in string %zu in file %s\n", 
                    codes, codes == 1 ? "" : "s", string_index, filename);
        }
        ++string_index;
    }

    // Close file
    fclose(fp);
    fp = NULL;

    // Final report of unicode characters
    printf("%ju unicode character%s found in %s\n", unicode, unicode == 1 ? "" : "s", filename);
    if (unicode > 0) {
        exit(666);
    }

    return 0;
}