// Refactored code with clear comments
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#define BLOCKSIZE 1 // Define BLOCKSIZE as per original intention, assuming 1 for illustration

/**
 * Print strings based on a block index.
 * 
 * @param block_index The index of the block to print.
 */
void print_string_at_block(int block_index) {
    puts(&data_buffer[block_index * BLOCKSIZE]);
}

static char *data_buffer; // Buffer to hold file data
static char *file_name = "data"; // Default file name
static size_t buffer_length;
static int option;

// Function prototypes
void process_command_line_arguments(int argc, char **argv);
void read_file_data(FILE *file_pointer);
void print_all_strings();

/**
 * Main function to execute the program.
 */
int main(int argc, char **argv) {
    FILE *file_pointer = NULL;

    // Process command line arguments
    process_command_line_arguments(argc, argv);

    // Open the file and handle errors
    errno = 0;
    file_pointer = fopen(file_name, "r");
    if (file_pointer == NULL) {
        fprintf(stderr, "Couldn't open data file %s: %s\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Read file data into buffer
    read_file_data(file_pointer);

    // Print all strings as per the original code logic
    print_all_strings();

    // Clean up resources
    free(data_buffer);
    data_buffer = NULL;
    fclose(file_pointer);
    file_pointer = NULL;

    return 0;
}

/**
 * Process command line arguments to set the file name.
 */
void process_command_line_arguments(int argc, char **argv) {
    while ((option = getopt(argc, argv, "i:")) != -1) {
        switch (option) {
            case 'i':
                file_name = optarg;
                break;
            case ':':
            case '?':
                exit(EXIT_FAILURE);
        }
    }
}

/**
 * Read data from the file into the buffer.
 */
void read_file_data(FILE *file_pointer) {
    if (getdelim(&data_buffer, &buffer_length, EOF, file_pointer) == -1) {
        fprintf(stderr, "Error reading data file\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Print all strings using the macro logic from the original program.
 */
void print_all_strings() {
    // Include the macro-like function equivalent to processing strings
    // from the included V.c script in original code
    #include "V.c"
}