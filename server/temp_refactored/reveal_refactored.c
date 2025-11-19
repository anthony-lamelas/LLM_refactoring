#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Function to process each line of input
void process_line(char *line, int *is_processing) {
    int line_length = strlen(line);

    // Check if the line is eligible for processing
    if (line_length > 3 && !isspace(line[0])) {
        char character = line[2];

        // Determine if processing should start or stop
        if (character == 'i') {
            *is_processing = 1;
        }
        if (character == 'm') {
            *is_processing = 0;
        }

        // Continue if processing is not active
        if (!*is_processing) {
            return;
        }

        // Check for special character '*' and adjust character
        if (line[2] == '*') {
            character = line[3];
        }

        // Output the character if it's an alphabet
        if (isalpha(character)) {
            putchar(character);
        }
    } else if (*is_processing && line_length > 6 && line[6] == ']') {
        // Output the character at position 4 if conditions are met
        putchar(line[4]);
    }
}

int main(void) {
    char buffer[128];
    int is_processing = 0;

    // Read lines from standard input
    while (fgets(buffer, sizeof(buffer), stdin)) {
        process_line(buffer, &is_processing);
    }

    // Print a newline at the end
    putchar('\n');
    return 0;
}