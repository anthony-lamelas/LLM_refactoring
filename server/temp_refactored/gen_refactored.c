// Refactored code with clear comments
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define WORD_LENGTH 5
#define MAX_WORDS 78557
#define DEFAULT_GUESSES 6
#define BUFFER_SIZE 100

// Function to check the guessed word against the target word
// Prints feedback ('g' for correct position, 'y' for correct letter, 'b' for wrong letter)
static int check_guess(char *guess, char *target) {
    printf("%.5s ", guess);
    for (int i = 0; i < WORD_LENGTH; ++i) {
        if (guess[i] == target[i])
            putchar('g');
        else if (memchr(target, guess[i], WORD_LENGTH))
            putchar('y');
        else
            putchar('b');
    }
    putchar('\n');
    fflush(stdout);
    return memcmp(guess, target, WORD_LENGTH) == 0;
}

int main(int argc, char **argv) {
    char words[MAX_WORDS][WORD_LENGTH];
    char buffer[BUFFER_SIZE];
    int word_count = 0;
    int win = 0;
    int max_guesses = DEFAULT_GUESSES;

    // Check for correct usage
    if (argc < 2) {
        fprintf(stderr, "usage: %s dict [target]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Check for optional environment variable to set number of guesses
    char *env_guesses = getenv("GUESSES");
    if (env_guesses) {
        max_guesses = atoi(env_guesses);
    }

    // Open the dictionary file
    FILE *file_pointer = fopen(argv[1], "r");
    if (!file_pointer) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    // Read words from the dictionary file
    while (fgets(buffer, sizeof(buffer), file_pointer)) {
        if (buffer[WORD_LENGTH] != '\n')
            continue;
        memcpy(words[word_count++], buffer, WORD_LENGTH);
    }
    fclose(file_pointer);

    // Seed random number generator
    srandom(time(NULL) ^ getpid());

    // Determine the target word
    char *target = (argc == 3) ? argv[2] : words[random() % word_count];

    // Optionally print the target word if CHEAT environment variable is set
    if (getenv("CHEAT")) {
        printf("target: %.5s\n", target);
    }

    // Main game loop
    for (int attempt = 0; attempt < max_guesses; ) {
        printf("? ");
        fflush(stdout);
        if (!fgets(buffer, sizeof(buffer), stdin))
            break;
        if (buffer[WORD_LENGTH] != '\n') {
            printf("you must enter 5 letters\n");
            continue;
        }
        ++attempt;
        if ((win = check_guess(buffer, target)))
            break;
    }
    printf("%c %d %s: %.5s\n", win ? 'y' : 'n', max_guesses, win ? "good" : "lose", target);
    return EXIT_SUCCESS;
}