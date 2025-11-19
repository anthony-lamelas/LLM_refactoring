// Refactored code with clear comments
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <errno.h>

// Function prototypes
void process_file(FILE *file);
void print_string_locations(char *buffer);

int main(int argc, char **argv) {
    FILE *file = NULL;
    char *filename = "data.src";
    int option;

    // Parse command-line options
    while ((option = getopt(argc, argv, "i:")) != -1) {
        switch (option) {
            case 'i':
                filename = optarg;
                break;
            case ':':
            case '?':
                fprintf(stderr, "Usage: %s [-i filename]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Open the file
    errno = 0;
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Couldn't open data file: %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Process the file
    process_file(file);

    fclose(file);

    return EXIT_SUCCESS;
}

// Process the file and print string locations
void process_file(FILE *file) {
    size_t length = 0;
    char *buffer = NULL;
    size_t line_number = 1;

    // Read the file one string at a time
    while (getdelim(&buffer, &length, '\0', file) != -1) {
        printf("<%zu>\n", line_number);
        print_string_locations(buffer);
        line_number++;
    }

    free(buffer);
}

// Print the location of each character in the string, ignoring multibyte characters
void print_string_locations(char *buffer) {
    size_t index = 0;
    int byte;

    while ((byte = buffer[index]) != '\0') {
        // Check for multibyte characters and adjust index accordingly
        if ((byte & 0xE0) == 0xC0) {
            index++;
        } else if ((byte & 0xF0) == 0xE0) {
            index += 2;
        } else if ((byte & 0xF8) == 0xF0) {
            index += 3;
        } else {
            putchar(byte);
        }
        index++;
    }
}