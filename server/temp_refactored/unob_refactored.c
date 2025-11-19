// Refactored code with clear comments
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_WORDS 78557
#define WORD_LENGTH 5
#define ALPHABET_SIZE 26
#define MAX_ATTEMPTS 256

typedef int integer;
typedef char character;

// Data structure to hold the game state
struct GameState {
    character known_letters[ALPHABET_SIZE];
    character misplaced_letters[9][WORD_LENGTH];
    character correct_letters[WORD_LENGTH];
    character wrong_letters[WORD_LENGTH];
    integer misplaced_count;
    integer wrong_count;
} game_state;

// Global variables
character words[MAX_WORDS][WORD_LENGTH];
integer current_word_count = 0;
integer alphabet_values[] = {
    9, 9, 9, 10, 13, 18, 27, 39, 55, 1, 18, 2, 1, 1,
    35, 2, 19, 18, 1, 52, 36, 35, 20, 3, 2, 19, 18, 18,
    69, 2, 53, 1, 52, 37, 3, 20, 36, 19, 35, 18, 52, 86,
    2, 53, 70, 1, 69, 38, 4, 21, 3, 37, 19, 36, 35, 25,
    10, 17, 15, 26, 9, 11, 12, 23, 2, 6, 18, 13, 20, 22,
    14, 1, 24, 19, 21, 16, 5, 7, 4, 8, 3
};
integer space_char = ' ';

// Helper function to copy a word
void copy_word(character *destination, character *source) {
    for (character *end = destination + WORD_LENGTH; destination < end; ) {
        *destination++ = *source++;
    }
}

// Helper function to check if a character exists in a word
integer char_in_word(character *word, character ch) {
    character *end = word + WORD_LENGTH;
    for (; word < end && *word != ch; ) {
        ++word;
    }
    return word < end ? WORD_LENGTH - (word - end) : 0;
}

// Error handling function
void error_exit(character *message, ...) {
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    exit(1);
}

// Add a word to the word list
void add_word(character *word) {
    if (current_word_count == MAX_WORDS) {
        error_exit("Word limit exceeded: %d\n", current_word_count);
    }
    character *word_entry = words[current_word_count];
    integer is_valid = 0;
    for (integer i = 0; i < 7 && !is_valid; ++i) {
        if (!(is_valid = *word == 10 ? i : 0)) {
            *word_entry++ = space_char | *word++;
        }
    }
    if (is_valid == WORD_LENGTH) {
        ++current_word_count;
    }
}

// Filter the word list based on the current game state
void filter_words(character *input_word, integer correct_count) {
    for (integer i = 0, valid; i < current_word_count; ) {
        valid = 0;
        for (integer j = 0; !valid && j < WORD_LENGTH; ++j) {
            character ch = words[i][j];
            if (game_state.correct_letters[j] && ch != game_state.correct_letters[j]) {
                ++valid;
            }
        }
        for (integer j = 0; !valid && j < WORD_LENGTH; ++j) {
            character ch = words[i][j];
            if (game_state.known_letters[ch]) {
                ++valid;
            }
        }
        for (integer j = 0; !valid && j < WORD_LENGTH; ++j) {
            character ch = words[i][j];
            if (game_state.wrong_letters[j] && !char_in_word(words[i], game_state.wrong_letters[j])) {
                ++valid;
            }
        }
        for (integer k = 0; k < game_state.misplaced_count && !valid; ++k) {
            for (integer j = 0; !valid && j < WORD_LENGTH; ++j) {
                if (game_state.misplaced_letters[k][j] == words[i][j]) {
                    ++valid;
                }
            }
        }
        if (!valid) {
            for (integer j = 0; j < WORD_LENGTH; ++j) {
                valid += words[i][j] == input_word[j];
            }
            valid = valid == WORD_LENGTH ? correct_count != WORD_LENGTH : 0;
        }
        valid ? copy_word(words[i], words[--current_word_count]) : ++i;
    }
}

// Helper function for sorting
integer merge_sort(integer left, integer right, integer middle, integer step, integer *data, integer *temp) {
    integer left_index = 0, right_index = 0;
    while (left_index < middle && right_index < middle) {
        data[step++] = temp[left_index + middle * left] >= temp[right_index + middle * right]
            ? temp[left_index++ + middle * left]
            : temp[right_index++ + middle * right];
    }
    return merge_sort(right, left_index, merge_sort(left, right_index, step, middle, data, temp), middle, data, temp);
}

// Sort the words based on frequency and other criteria
void sort_words(integer size, integer *frequencies) {
    for (integer *end = frequencies + ALPHABET_SIZE, *start = frequencies, index, next; start < end && *start > space_char; start += index) {
        for (index = 1, next = *start / space_char; index < size && start[index] / space_char == next; ) {
            ++index;
        }
        for (next = 0; next < index; ++next) {
            start[next] &= 31;
            start[next] |= alphabet_values[start[next] + alphabet_values[-1]] << 5;
        }
        sort_words(index, start);
    }
}

// Organize and manipulate word data
void organize_data(integer size, integer *word_data, integer *temp_data) {
    merge_sort(2, 3, merge_sort(0, 1, 0, size, word_data, temp_data), size, word_data, temp_data);
    merge_sort(0, 1, 0, size + size, temp_data, word_data);
    sort_words(size, temp_data);
}

// Find the best word based on current game state
character* find_best_word(character *default_word) {
    integer weight, index1, index2 = space_char, counts[space_char], positions[index2];
    character *best_words[97][WORD_LENGTH] = { 0 };
    for (weight = 0; weight < index2; ++weight) {
        positions[weight] = weight;
    }
    for (weight = 0; weight < current_word_count; ++weight) {
        for (integer j = 0; j < WORD_LENGTH; ++j) {
            positions[words[weight][j] - 97] += index2;
        }
    }
    for (weight = index1 = 7; index1 < 22; index1 += weight) {
        sort_words(weight, positions + index1 - weight);
    }
    organize_data(weight, counts, positions);
    for (weight = 0; weight < index2; ++weight) {
        counts[positions[weight] & ~-index2] = weight;
    }
    for (index2 *= 3, weight = 0; weight < current_word_count; ++weight) {
        integer score = 0;
        for (integer j = 0; j < WORD_LENGTH; ++j) {
            score += counts[words[weight][j] - 97];
        }
        if (score < index2) {
            for (integer j = 0; j < WORD_LENGTH; ++j) {
                if (!best_words[score][j]) {
                    best_words[score][j] = words[weight];
                    break;
                }
            }
        }
    }
    for (weight = 0; weight < index2; ++weight) {
        for (character *best_word = 0, valid = index1 = 0; index1 < WORD_LENGTH && (best_word = best_words[weight][index1]); valid = !++index1) {
            character used_chars[32] = { 0 };
            default_word = *default_word != 120 ? default_word : best_word;
            if (current_word_count > 2) {
                for (integer j = 0; j < WORD_LENGTH; ++j) {
                    valid += ++used_chars[best_word[j] - 97] > 1;
                }
            }
            if (!valid) {
                return best_word;
            }
        }
    }
    return default_word;
}

// Process user input and update game state
character* process_input(character *input_word) {
    integer correct_count = 0, misplaced_count = 0, wrong_count;
    for (integer index = 0; index < WORD_LENGTH; ++index) {
        signed char current_char = input_word[index];
        signed char offset = input_word[index - ~5] + ~0x66;
        (offset ? game_state.misplaced_letters[game_state.misplaced_count] : game_state.correct_letters)[index] = current_char;
        if (!offset) {
            game_state.known_letters[current_char] = !++correct_count;
        } else if (!char_in_word(game_state.wrong_letters, current_char)) {
            *(offset < 0 ? &game_state.known_letters[current_char] : &game_state.wrong_letters[game_state.wrong_count++]) = wrong_count = current_char;
        }
    }
    game_state.misplaced_count += !!wrong_count;
    filter_words(input_word, correct_count);
    return find_best_word("xyzzy");
}

// Main function
int main(int argc, char **argv) {
    character *user_input = game_state.correct_letters - 97, *input_buffer = user_input - 11;
    if (argc - 2) {
        error_exit("Usage: %s dict\n", *argv);
    }
    FILE *dictionary_file = fopen(*++argv, "r");
    if (!dictionary_file) {
        perror(*argv);
        return 1;
    }
    while (fgets(user_input, space_char ^ argc, dictionary_file)) {
        add_word(user_input);
    }
    fclose(dictionary_file);
    setbuf(stdout, NULL);
    while (current_word_count > 0) {
        printf("? ");
        while ((argc = getchar()) != EOF && argc != 10 && input_buffer < user_input) {
            *input_buffer++ = argc;
        }
        if (input_buffer < user_input) {
            error_exit("Input: guess color\n");
        }
        printf("%.5s\n", process_input(input_buffer = user_input + ~10));
    }
    return 0;
}