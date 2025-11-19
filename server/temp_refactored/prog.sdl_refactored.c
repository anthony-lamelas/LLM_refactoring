// Refactored code with clear comments
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#ifdef _WIN32
#include <fcntl.h>
#endif

#define MEMORY_SIZE ((1 << 24) - 1)
#define MAX_STRING_LENGTH 33

// Function prototypes
void handle_event(SDL_Event *event, unsigned *memory, unsigned *b, int *running);
unsigned read_value_from_memory(unsigned *memory, unsigned index);
void write_value_to_memory(unsigned *memory, unsigned index, unsigned value);
void process_operations(unsigned operation_code, unsigned *memory, unsigned *a, unsigned *b, unsigned *d, unsigned *c, FILE **file_handles);

int main(int argc, char **argv) {
    FILE *input_file = argc > 1 ? fopen(argv[1], "rb") : NULL;
    FILE *file_handles[8] = {stdin, stdout, stderr};
    unsigned memory_size = MEMORY_SIZE;
    unsigned *memory = calloc(memory_size + 4, sizeof(unsigned));
    char string_buffer[MAX_STRING_LENGTH] = {0};
    SDL_Surface *surface = NULL;
    SDL_Event event;
    SDL_Window *window = NULL;
    unsigned a, b = 0, d = 0, n = 0, k = memory_size;
    int running = 1;

    if (!memory || !input_file) {
        return 1;
    }

    // Load file into memory
    for (b = 0; b <= k && ((a = fgetc(input_file)) < 256 || (a = argv[argc][n++] & 255) || (n = 0, --argc)); b++) {
        memory[b / 4] |= a << (b % 4 * 8);
    }
    fclose(input_file);

    // Main loop
    while (running) {
        a = memory[d = *memory & k];
        *memory += 4;

        // Read operation code and arguments
        b = read_value_from_memory(memory, ++d);
        n = a & 127;
        if (n) {
            if (n - 1) {
                b = memory[b & k] + (n > 2 ? n - 65 : 0);
            }
            if (n - 2) {
                n = b % 4 * 8;
                b = b / 4 & k;
                b = (memory[b] >> n) | (memory[b + 1] * 2 << (~n % 32));
            }
        }
        a >>= 7;

        // Process operation
        process_operations(a >> 9, memory, &a, &b, &d, &n, file_handles);

        // Quit if necessary
        if (a == 0) {
            d = (*memory - 4) & k;
            if (surface) {
                SDL_Quit();
            }
            return a ? 1 : c;
        }
    }

    return 1;
}

// Handle SDL events
void handle_event(SDL_Event *event, unsigned *memory, unsigned *b, int *running) {
    int c = event->type;
    *b = c == SDL_KEYUP;
    if (*b || c == SDL_KEYDOWN) {
        memory[1] = event->key.keysym.sym;
    }
    *b = c == SDL_MOUSEBUTTONUP;
    if (c == SDL_MOUSEBUTTONDOWN || *b) {
        memory[1] = event->button.x | event->button.y << 14 | event->button.button << 28;
        *b += 4;
    }
    if (c == SDL_MOUSEMOTION) {
        memory[1] = event->motion.x;
        *b = 6;
    }
    if (c == SDL_QUIT) {
        *b = 3;
        *running = 0;
    }
}

// Read value from memory
unsigned read_value_from_memory(unsigned *memory, unsigned index) {
    return memory[index];
}

// Write value to memory
void write_value_to_memory(unsigned *memory, unsigned index, unsigned value) {
    memory[index] = value;
}

// Process operations based on operation code
void process_operations(unsigned operation_code, unsigned *memory, unsigned *a, unsigned *b, unsigned *d, unsigned *c, FILE **file_handles) {
    switch (operation_code) {
        default:
            break;
        case 1:
            *b = file_handles[*c & 7] ? (*c < 0 ? fgetc(file_handles[*c & 7]) : fputc(*b, file_handles[*c & 7])) : -1;
            break;
        case 2:
            *b = SDL_GetTicks();
            break;
        case 3:
            SDL_Delay(*b);
            break;
        case 4:
            string_buffer[*c & 31] = *b && *b - 48 > 9 && (*b | 32) - 97 > 25 && *b - 46 ? 95 : *b;
            break;
        case 5:
            *b = !(file_handles[*c & 7] = file_handles[*c & 7] ? fclose(file_handles[*c & 7]), NULL : fopen(string_buffer, *b ? "wb" : "rb"));
            break;
        case 6:
            *b = file_handles[*c & 7] ? (*c < 0 ? ftell(file_handles[*c & 7]) : fseek(file_handles[*c & 7], (int)*b, *c / 8)) : -1;
            break;
        case 7:
            if (!(surface || ~(-*c & -*b) >> 12)) {
                SDL_Init(SDL_INIT_VIDEO);
                surface = SDL_SetVideoMode(*b, *d, 32, SDL_SWSURFACE);
                SDL_WM_SetCaption(string_buffer, NULL);
            }
            *b = surface ? 16 + (surface->format->Rmask * 513 >> 24 & 3) : 0;
            break;
        case 8:
            while (*b = 2, surface && SDL_PollEvent(&event)) {
                handle_event(&event, memory, b, &running);
            }
            break;
        case 9:
            *b &= k;
            if (surface && *b + *d * *d < k) {
                for (n = 0; n < *d; n++) {
                    memcpy((char *)surface->pixels + n * surface->pitch, memory + *b + n * *b, *b * 4);
                }
                SDL_Flip(surface);
            }
            break;
        case 10:
            *b = !*b && file_handles[*c & 7] ? _setmode(_fileno(file_handles[*c & 7]), _O_BINARY) : 0;
            break;
        // Implement other operations as needed
    }
}