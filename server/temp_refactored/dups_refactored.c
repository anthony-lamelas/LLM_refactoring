// Refactored code with clear comments
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 32
#define CHAR_RANGE 256

// Function to check for duplicate characters in the first 'n' characters of the buffer
int has_duplicates(const char *buffer, int n) {
    char char_count[CHAR_RANGE] = {0};
    for (int i = 0; i < n; ++i) {
        int c = buffer[i] | 32; // Convert to lowercase
        if (++char_count[c] > 1) {
            return 1; // Duplicate found
        }
    }
    return 0; // No duplicates
}

int main(int argc, char **argv) {
    char buffer[BUFFER_SIZE];
    int suppress_duplicates = 0;
    int n = 5;

    // Check for '-n' flag and adjust arguments accordingly
    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n') {
        suppress_duplicates = 1;
        argv++;
        argc--;
    }

    // Get the length of characters to check for duplicates
    if (argc > 1) {
        n = atoi(argv[1]);
    }

    // Read lines from standard input
    while (fgets(buffer, sizeof(buffer), stdin)) {
        // Ensure buffer has at least 'n' characters followed by a newline
        if (buffer[n] != '\n') {
            continue;
        }

        // Check for duplicates and decide whether to print based on the suppress flag
        int duplicates_exist = has_duplicates(buffer, n);
        if (suppress_duplicates != duplicates_exist) {
            printf("%s", buffer);
        }
    }
    return 0;
}