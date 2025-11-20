#include <stdio.h>

// Macro definition for the repetitive operation
#define V(num) printf("Value: %d\n", num)

// Main function
int main() {
    // Loop through numbers 1 to 172 and apply macro V
    for (int i = 1; i <= 172; i++) {
        V(i);
    }
    return 0;
}