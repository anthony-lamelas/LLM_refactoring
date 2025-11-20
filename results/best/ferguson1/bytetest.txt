#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

char *p = NULL, *y;

/*
 * Z calculates the block location.
 */
#define Z(x) ((x) * 1663)
#define V(x) { printf("<%zu>\n", (size_t)(x)); puts(&y[Z(x)]); }

int main(void) {
    FILE *filePointer = fopen("data", "r");
    size_t length = 0;

    errno = 0;
    if (filePointer == NULL) {
        fprintf(stderr, "Couldn't open data file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (getdelim(&y, &length, EOF, filePointer) != -1) {
        /*
         * Now we have to test every single string to make sure it's valid.
         * The makedata.sh script generates V.c.
         */
        #include "V.c"
    }

    free(y);
    y = NULL;
    fclose(filePointer);
    filePointer = NULL;

    return 0;
}