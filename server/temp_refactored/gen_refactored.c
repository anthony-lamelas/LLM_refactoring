#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/*
 * Wordle generator - text mode
 * This program works in coordination with the obfuscated code
 * to allow one to play more rapidly (e.g. test the algorithm)
 * or just to play along in text mode.
 *
 * This version is a pale imitation of the Real Wordle(tm).
 * If you want to get the real experience, please visit:
 *
 *	https://www.nytimes.com/games/wordle/index.html
 */

// Check function to evaluate the guessed word against the target word
static int check(char *guess, char *target) {
    printf("%.5s ", guess);
    for (int i = 0; i < 5; ++i) {
        if (guess[i] == target[i]) {
            putchar('g');
        } else if (memchr(target, guess[i], 5)) {
            putchar('y');
        } else {
            putchar('b');
        }
    }
    putchar('\n');
    fflush(stdout);
    return memcmp(guess, target, 5) == 0;
}

int main(int argc, char **argv) {
    char words[78557][5];
    char buffer[100];
    int wordCount = 0, win = 0;
    int maxGuesses = 6;

    // Check for correct usage
    if (argc < 2) {
        fprintf(stderr, "usage: %s dict [target]\n", argv[0]);
        return 1;
    }

    // Get the number of guesses from the environment variable if set
    if (getenv("GUESSES")) {
        maxGuesses = atoi(getenv("GUESSES"));
    }

    // Open the dictionary file
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror(argv[1]);
        exit(1);
    }

    // Read the words from the dictionary file
    while (fgets(buffer, sizeof buffer, file)) {
        if (buffer[5] != '\n') {
            continue;
        }
        memcpy(words[wordCount++], buffer, 5);
    }
    fclose(file);

    // Seed random number generator
    srandom(time(0) ^ getpid());

    // Determine the target word
    char *target = argc == 3 ? argv[2] : words[random() % wordCount];
    if (getenv("CHEAT")) {
        printf("target: %.5s\n", target);
    }

    // Main game loop
    for (int guessNumber = 0; guessNumber < maxGuesses;) {
        printf("? ");
        fflush(stdout);
        if (!fgets(buffer, sizeof buffer, stdin)) {
            break;
        }
        if (buffer[5] != '\n') {
            printf("you must enter 5 letters\n");
            continue;
        }
        ++guessNumber;
        if ((win = check(buffer, target))) {
            break;
        }
    }

    // Print the result
    printf("%c %d %s: %.5s\n", win ? 'y' : 'n', guessNumber, win ? "good" : "lose", target);
    return 0;
}