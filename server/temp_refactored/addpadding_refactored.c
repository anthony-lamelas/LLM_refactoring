// Refactored code with clear comments
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

// Constants
#define BLOCKSIZE 512
#define PADDING_STRING "ha_ha(){hell:goto hell;}main(){goto hell;hell:goto hell;ha_ha();main();goto hell;}"
#define COMMENT_START "/*useless code below, goto end of file*/"
#define COMMENT_END "/*useless code above, goto start of file*/"

// Global variables
static FILE *input_file = NULL, *output_file = NULL, *prog_file = NULL;
static char *buffer = NULL, *prog_content = NULL;
static size_t buffer_length = 0, prog_length = 0, padding_length = 0;
static char *data_src = "data.src";
static char *data_dst = "data";
static char *data_asc_src = "data.asc.src";
static char *data_asc_dst = "data.asc";
static char *data_scrnshot_src = "data.scrnshot.src";
static char *data_scrnshot_dst = "data.scrnshot";
static char *prog_filename = "prog.c";

// Function prototypes
void add_padding_to_data(const char *dst, const char *src);
void close_and_free_resources();
void handle_error(const char *message, const char *filename);

// Add padding to each string in the data file
void add_padding_to_data(const char *dst, const char *src) {
    input_file = fopen(src, "r");
    if (input_file == NULL) {
        handle_error("couldn't open data source file", src);
    }

    output_file = fopen(dst, "w");
    if (output_file == NULL) {
        handle_error("couldn't open output data file", dst);
        fclose(input_file);
        return;
    }

    // Write initial comment and padding
    int comment_length = fprintf(output_file, "%s", COMMENT_START);
    for (size_t i = comment_length; i < BLOCKSIZE;) {
        for (size_t j = 0; j < padding_length && i + j < BLOCKSIZE; ++j) {
            fputc(PADDING_STRING[j], output_file);
        }
        i += padding_length;
    }

    // Process each string in the input file
    while (getdelim(&buffer, &buffer_length, '\0', input_file) != -1) {
        if (*buffer == '%') {
            fprintf(output_file, "%s", buffer);
        } else if (*buffer == '\a') {
            fprintf(output_file, "\b%s", buffer + 1);
        } else {
            fputs(buffer, output_file);
        }
        fputc('\0', output_file);

        for (size_t i = strlen(buffer) + 1; i < BLOCKSIZE;) {
            for (size_t j = 0; j < padding_length && i + j < BLOCKSIZE; ++j) {
                fputc(PADDING_STRING[j], output_file);
            }
            i += padding_length;
        }
    }

    // Write ending comments and prog content
    fprintf(output_file, "\n%s%s\n", COMMENT_END, COMMENT_START);

    size_t count = 0;
    for (char *p = prog_content; *p; ++p) {
        if (*p == '\n') {
            if (count < 15) {
                fputc('\n', output_file);
            } else {
                fprintf(output_file, "\n%s%s%s\n", COMMENT_START, PADDING_STRING, COMMENT_END);
            }
            ++count;
        } else {
            fputc(*p, output_file);
        }
    }

    fprintf(output_file, "%s", COMMENT_END);

    fclose(input_file);
    fclose(output_file);
}

// Handle errors by printing error message and freeing resources
void handle_error(const char *message, const char *filename) {
    fprintf(stderr, "%s: %s: %s\n", filename, message, strerror(errno));
    close_and_free_resources();
    exit(1);
}

// Close all open files and free allocated memory
void close_and_free_resources() {
    if (input_file) {
        fclose(input_file);
        input_file = NULL;
    }
    if (output_file) {
        fclose(output_file);
        output_file = NULL;
    }
    if (prog_file) {
        fclose(prog_file);
        prog_file = NULL;
    }
    if (buffer) {
        free(buffer);
        buffer = NULL;
    }
    if (prog_content) {
        free(prog_content);
        prog_content = NULL;
    }
}

int main(int argc, char **argv) {
    int option;
    while ((option = getopt(argc, argv, "i:I:o:O:p:")) != -1) {
        switch (option) {
            case 'i':
                data_src = optarg;
                break;
            case 'I':
                data_asc_src = optarg;
                break;
            case 'o':
                data_dst = optarg;
                break;
            case 'O':
                data_asc_dst = optarg;
                break;
            case 'p':
                prog_filename = optarg;
                break;
            case ':':
            case '?':
                exit(1);
                break;
        }
    }

    // Load prog_file content
    prog_file = fopen(prog_filename, "r");
    if (prog_file == NULL) {
        handle_error("couldn't open prog.c code file", prog_filename);
    }

    if (getdelim(&prog_content, &prog_length, EOF, prog_file) == -1) {
        handle_error("couldn't read prog.c", prog_filename);
    }

    padding_length = strlen(PADDING_STRING);

    // Create padded data files
    add_padding_to_data(data_dst, data_src);
    add_padding_to_data(data_asc_dst, data_asc_src);
    add_padding_to_data(data_scrnshot_dst, data_scrnshot_src);

    // Free resources
    close_and_free_resources();
    return 0;
}