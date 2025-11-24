/**
 * Test to understand the actual font width
 */
#include <stdio.h>
#include <stdint.h>

// 'A' from the font
static const uint8_t font_A[8] = {0x3C, 0x66, 0x6E, 0x7E, 0x66, 0x66, 0x66, 0x00};

int main() {
    printf("Font 'A' data: ");
    for (int i = 0; i < 8; i++) {
        printf("0x%02X ", font_A[i]);
    }
    printf("\n\n");
    
    printf("Rendering with 6 columns (correct for 6x8 font):\n");
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 6; col++) {  // Only 6 columns
            uint8_t byte_val = font_A[col];
            if (byte_val & (0x80 >> row)) {
                printf("█");
            } else {
                printf("·");
            }
        }
        printf("\n");
    }
    
    printf("\nRendering with 8 columns (what the C code does):\n");
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {  // 8 columns
            uint8_t byte_val = font_A[col];
            if (byte_val & (0x80 >> row)) {
                printf("█");
            } else {
                printf("·");
            }
        }
        printf("\n");
    }
    
    return 0;
}
