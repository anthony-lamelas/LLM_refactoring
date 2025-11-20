#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char buffer[32];

    // Check for the "-n" option and adjust argument count and pointer accordingly
    int no_duplicates_flag = (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n' && ++argv && --argc);
    int n = (argc > 1) ? atoi(argv[1]) : 5;

    // Process each line from the standard input
    while (fgets(buffer, sizeof(buffer), stdin)) {
        char char_count[256] = {0};  // Array to count occurrences of each character
        int has_duplicates = 0;      // Flag to indicate if duplicates are found

        // Check if the line length matches the expected length
        if (buffer[n] != '\n') {
            continue;
        }

        // Iterate over the first 'n' characters of the line
        for (int i = 0; i < n; ++i) {
            int char_index = buffer[i] |= 32;  // Convert to lowercase
            if (++char_count[char_index] > 1) {
                ++has_duplicates;  // Increment duplicate flag
            }
        }

        // Print the line based on the presence of duplicates and the no_duplicates_flag
        if (!!has_duplicates != no_duplicates_flag) {
            printf("%s", buffer);
        }
    }
    return 0;
}