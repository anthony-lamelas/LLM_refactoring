#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#define BLOCKSIZE 512

static char padding[] = "ha_ha(){hell:goto hell;}main(){goto hell;hell:goto hell;ha_ha();main();goto hell;}";
static char comment1[] = "/*useless code below, goto end of file*/";
static char comment2[] = "/*useless code above, goto start of file*/";

static FILE *in = NULL, *out = NULL, *prog = NULL;
static char *buf = NULL, *p = NULL;
static size_t len = 0, count = 0, padlen = 0;
static char *data_src = "data.src";
static char *data = "data";
static char *data_asc_src = "data.asc.src";
static char *data_asc = "data.asc";
static char *data_scrnshot_src = "data.scrnshot.src";
static char *data_scrnshot = "data.scrnshot";
static char *prog_c_filename = "prog.c";
static char *prog_c = NULL;
static int l = 0, k = 0;

void make_data(char const *dst, char const *src) {
    // Close any open input file
    if (in) {
        fclose(in);
        in = NULL;
    }

    // Close any open output file
    if (out) {
        fclose(out);
        out = NULL;
    }

    // Open the input file
    errno = 0;
    in = fopen(src, "r");
    if (in == NULL) {
        fprintf(stderr, "%s: couldn't open data source file: %s\n", src, strerror(errno));
        exit(1);
    }

    // Open the output file
    errno = 0;
    out = fopen(dst, "w");
    if (out == NULL) {
        fprintf(stderr, "%s: couldn't open output data file: %s\n", dst, strerror(errno));
        fclose(in);
        in = NULL;
        fclose(prog);
        prog = NULL;
        free(prog_c);
        prog_c = NULL;
        exit(1);
    }

    // Write the first comment and padding
    l = fprintf(out, "%s", comment1);
    size_t i, j;
    for (i = l; i < BLOCKSIZE;) {
        for (j = 0; j < padlen; ++j) {
            if (j + i + 1 < BLOCKSIZE + 1) {
                fputc(padding[j], out);
            }
        }
        i += j;
    }

    // Process each string in the input file
    while (getdelim(&buf, &len, '\0', in) != -1) {
        if (*buf == '%') {
            fprintf(out, "%s", buf);
        } else if (*buf == '\a') {
            fprintf(out, "\b%s", buf + 1);
        } else {
            for (p = buf; *p; ++p) {
                fputc(*p, out);
            }
        }
        fputc('\0', out);

        for (i = strlen(buf) + 1; i < BLOCKSIZE;) {
            for (j = 0; j < padlen; ++j) {
                if (j + i < BLOCKSIZE) {
                    fputc(padding[j], out);
                }
            }
            i += j;
        }
    }

    // Add comments and padding at the end of the file
    fprintf(out, "\n%s%s\n", comment2, comment1);

    // Write the prog_c content with formatting
    for (count = 0, p = prog_c; *p; ++p) {
        if (*p == '\n') {
            if (count < 15) {
                fputc('\n', out);
            } else {
                fprintf(out, "\n%s%s%s\n", comment1, padding, comment2);
            }
            ++count;
        } else {
            fputc(*p, out);
        }
    }

    // Add the final comment
    fprintf(out, "%s", comment2);

    // Clean up
    fclose(in);
    in = NULL;
    fclose(out);
    out = NULL;
    free(buf);
    buf = NULL;
}

int main(int argc, char **argv) {
    // Process command line arguments
    while ((k = getopt(argc, argv, "i:I:o:O:p:dt")) != -1) {
        switch (k) {
            case 'i':
                data_src = optarg;
                break;
            case 'I':
                data_asc_src = optarg;
                break;
            case 'o':
                data = optarg;
                break;
            case 'O':
                data_asc = optarg;
                break;
            case 'p':
                prog_c_filename = optarg;
                break;
            case ':':
            case '?':
                exit(1);
                break;
        }
    }

    // Calculate the length of the padding string
    padlen = strlen(padding);

    // Open and read the prog.c file
    errno = 0;
    prog = fopen(prog_c_filename, "r");
    if (prog == NULL) {
        fprintf(stderr, "%s: couldn't open prog.c code file: %s\n", prog_c_filename, strerror(errno));
        fclose(in);
        in = NULL;
        exit(1);
    }

    errno = 0;
    if (getdelim(&prog_c, &len, EOF, prog) == -1) {
        fprintf(stderr, "couldn't read prog.c\n");
        fclose(in);
        in = NULL;
        fclose(prog);
        prog = NULL;
        exit(1);
    }

    // Process each file
    make_data(data, data_src);
    make_data(data_asc, data_asc_src);
    make_data(data_scrnshot, data_scrnshot_src);

    // Clean up
    fclose(prog);
    prog = NULL;
    return 0;
}