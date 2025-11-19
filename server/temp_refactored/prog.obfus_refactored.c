// Refactored code with clear comments
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define IS_UPPER(C) (isupper(C))
#define IS_LOWER(C) (islower(C))
#define IS_NON_ALPHA(C) (!isalpha(C))

typedef struct Node {
    char *key;
    char *value;
    char *extra;
    struct Node *next;
} Node;

Node *head = NULL;

// Function to reverse the alphabetic characters in a string
char *reverse_alphabet(char *s) {
    int index = 0;
    while (s[index]) {
        if (isupper(s[index])) {
            s[index] = 'Z' - (s[index] - 'A');
        }
        index++;
    }
    return s;
}

// Function to read dictionary from file and store in linked list
void load_dictionary() {
    FILE *file = fopen("dict", "r");
    if (file != NULL) {
        char *line = NULL;
        size_t len = 0;
        while (getline(&line, &len, file) > 0) {
            char *key = strtok(line, "\t");
            char *value = strtok(NULL, "\t");
            char *extra = strtok(NULL, "\t");

            if (value || extra) {
                Node *new_node = calloc(1, sizeof(Node));
                new_node->key = strdup(reverse_alphabet(key));
                new_node->value = strdup(reverse_alphabet(value));
                new_node->extra = extra ? strdup(reverse_alphabet(extra)) : "";
                new_node->next = head;
                head = new_node;
            }
        }
        free(line);
        fclose(file);
    } else {
        puts("Couldn't open dict");
    }
}

// Function to process the input and translate it using the dictionary
void process_input(char *input) {
    Node *current_node = head;
    FILE *file = NULL;
    size_t input_length = 0;

    if (input[0] == '+') {
        file = (input[1] == '-') ? stdin : fopen(input + 1, "r");
        if (file) {
            input = NULL;
            if (getdelim(&input, &input_length, EOF, file) < 1 || !input) {
                goto CLEANUP;
            }
        }
    }

    // Check for exact match in dictionary linked list
    for (; current_node; current_node = current_node->next) {
        if (!strcasecmp(current_node->key, input)) {
            int index = 0;
            while (current_node->value[index]) {
                putchar((index == 0 && IS_UPPER(input[0])) ? current_node->value[index] : tolower(current_node->value[index]));
                ++index;
            }
            goto CLEANUP;
        }
    }

    // Process each character in the input
    for (int i = 0; input[i]; ++i) {
        if (IS_NON_ALPHA(input[i])) {
            putchar(input[i]);
        } else {
            int is_lower = IS_LOWER(input[i]);
            for (int j = 0; j < sizeof(bioh) / sizeof(bioh[0]); ++j) {
                if (*bioh[j][0] == toupper(input[i])) {
                    printf("%s", i ? " " : "");
                    for (int k = 0; bioh[j][1][k]; ++k) {
                        putchar(is_lower ? tolower(bioh[j][1][k]) : toupper(bioh[j][1][k]));
                    }
                    break;
                }
            }
        }
    }

CLEANUP:
    putchar(' ');
    if (file) {
        if (file != stdin) fclose(file);
        free(input);
    }
}

// Main function to load dictionary and process command-line arguments
int main(int argc, char **argv) {
    load_dictionary();
    for (int i = 1; i < argc; i++) {
        process_input(argv[i]);
    }
    puts("");
    return 0;
}