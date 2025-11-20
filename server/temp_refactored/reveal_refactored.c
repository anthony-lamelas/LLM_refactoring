#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(void) {
    int is_output_enabled = 0; // Flag to control output
    char buffer[128]; // Buffer to store input lines

    // Read lines from standard input
    while (fgets(buffer, sizeof(buffer), stdin)) {
        int length = strlen(buffer); // Get the length of the line

        // Check if the line is suitable for processing
        if (length > 3 && !isspace(buffer[0])) {
            char control_char = buffer[2];

            // Check control character to set output flag
            if (control_char == 'i') {
                is_output_enabled = 1;
            }
            if (control_char == 'm') {
                is_output_enabled = 0;
            }

            // If output is not enabled, skip to next line
            if (!is_output_enabled) {
                continue;
            }

            // Determine the character to possibly output
            if (buffer[2] == '*') {
                control_char = buffer[3];
            }

            // Output the character if it is an alphabetic character
            if (isalpha(control_char)) {
                putchar(control_char);
            }
        } else if (is_output_enabled && length > 6 && buffer[6] == ']') {
            // Output the character at position 4 if conditions meet
            putchar(buffer[4]);
        }
    }

    putchar('\n'); // Print a newline at the end
    return 0;
}