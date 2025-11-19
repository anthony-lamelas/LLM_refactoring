// Refactored code with clear comments
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <fcntl.h>

#define MAX_MEMORY_SIZE ((1 << 24) - 1)
#define BUFFER_SIZE 33

// Global variable to track exit condition
static int exit_condition = 0;

// Window procedure callback function
static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    exit_condition |= (msg == WM_DESTROY);
    return DefWindowProcA(hwnd, msg, w_param, l_param);
}

// Function to read file content into memory
void read_file_into_memory(FILE *file, unsigned *memory) {
    unsigned address = 0;
    int byte;
    while ((byte = fgetc(file)) != EOF && address <= MAX_MEMORY_SIZE) {
        memory[address / 4] |= byte << (address % 4 * 8);
        address++;
    }
    fclose(file);
}

// Main function
int main(int argc, char **argv) {
    FILE *file = (argc > 1) ? fopen(argv[1], "rb") : NULL;
    void *handles[8] = { stdin, stdout, stderr };
    unsigned *memory = calloc(MAX_MEMORY_SIZE + 4, sizeof(unsigned));
    char buffer[BUFFER_SIZE] = { 0 };
    LARGE_INTEGER freq, counter;
    HWND window_handle = 0;
    HDC device_context = 0;
    MSG msg;
    double frequency_factor;

    // Initialize the timer and frequency factor
    QueryPerformanceFrequency(&freq);
    frequency_factor = 1000.0 / freq.QuadPart;
    timeBeginPeriod(1);

    if (memory && file) {
        read_file_into_memory(file, memory);
        memory[1] = ftell(file);

        unsigned pc = 0, a, b = 0, c, d, n = 0, k = MAX_MEMORY_SIZE;

        while (1) {
            a = memory[pc & k];
            pc += 4;
            c = b;
            d = memory[pc & k];

            switch (a >> 9) {
                case 1: // I/O operations
                    b = handles[c & 7] ? (c < 0 ? fgetc(handles[c & 7]) : fputc(b, handles[c & 7])) : -1;
                    break;
                case 2: // Timing operation
                    QueryPerformanceCounter(&counter);
                    b = counter.QuadPart * frequency_factor;
                    break;
                case 3: // Sleep operation
                    Sleep(b);
                    break;
                case 4: // String operations
                    buffer[c & 31] = (b && b - 48 > 9 && (b | 32) - 97 > 25 && b - 46) ? 95 : b;
                    break;
                case 5: // File operations
                    b = (handles[c & 7]) ? (c < 0 ? fclose(handles[c & 7]), file = NULL : fopen(buffer, b ? "wb" : "rb")) : -1;
                    break;
                case 6: // File seek/tell
                    b = (handles[c & 7]) ? (c < 0 ? ftell(handles[c & 7]) : fseek(handles[c & 7], (int)b, c / 8)) : -1;
                    break;
                case 7: // Window creation
                    if (!window_handle && ~(-c & -b) >> 12) {
                        WNDCLASSA wnd_class = { 0, window_proc, 0, 0, 0, 0, 0, 0, 0, buffer };
                        RECT rect = { 0, 0, c, b };
                        RegisterClassA(&wnd_class);
                        AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0);
                        device_context = GetDC(window_handle = CreateWindowExA(0, buffer, buffer, WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, NULL, NULL));
                    }
                    b = window_handle ? 10 : 0;
                    break;
                default:
                    return a ? 1 : c;
            }

            n = a & 127;
            if (n) {
                c = 0;
                d = n > 2 ? memory[pc & k] + n - 65 : pc;
                d = (n - 2 ? c = d % 4 * 8, d / 4 : d) & k;
                n = -1;
                n >>= a >> 4 & 24;
                if (c) {
                    memory[pc] = (memory[pc] | n << c) ^ (~b & n) << c;
                }
                pc++;
                if (c) {
                    memory[pc] = (memory[pc] | n >> (32 - c)) ^ (~b & n) >> (32 - c);
                }
            } else {
                memory[0] += b ? d : 0;
            }
        }
    }

    return 1;
}