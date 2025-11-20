#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *filePointer = fopen("data.src", "r");
    char *buffer = NULL;
    size_t bufferLength = 0;
    char *currentPosition = NULL;
    size_t index = 0, stringLength = 0, charIndex = 0;

    // Check if the file opened successfully
    if (filePointer == NULL) {
        fprintf(stderr, "Couldn't open data file\n");
        exit(1);
    }

    // Read the file content into buffer until the end of the file
    if (getdelim(&buffer, &bufferLength, EOF, filePointer) != -1) {
        currentPosition = buffer;

        // Iterate over the buffer content
        while (index < bufferLength) {
            stringLength = strlen(currentPosition);
            charIndex = 0;

            // Print each character of the current string
            while (charIndex < stringLength) {
                putchar(*(currentPosition + charIndex));
                ++charIndex;
            }

            // Move to the next string in the buffer
            index += stringLength + 1;
            currentPosition += stringLength + 1;
        }
    }

    // Free the allocated buffer and close the file
    free(buffer);
    buffer = NULL;
    fclose(filePointer);
    filePointer = NULL;

    return 0;
}