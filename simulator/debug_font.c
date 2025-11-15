/**
 * Debug font rendering to understand the issue
 */
#include <stdio.h>
#include <stdint.h>

// Copy the font data
static const uint8_t font6x8[96][8] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // space
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18}, // !
    // ... (we'll just test 'A')
};

// Test 'A' character
static const uint8_t font_A[8] = {0x3C, 0x66, 0x6E, 0x7E, 0x66, 0x66, 0x66, 0x00};

void print_char_bitmap(const uint8_t* char_data) {
    printf("Character bitmap (8 columns x 8 rows):\n");
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            uint8_t line = char_data[col];
            // Check if bit 7-row is set
            if (line & (0x80 >> row)) {
                printf("█");
            } else {
                printf("·");
            }
        }
        printf("\n");
    }
}

int main() {
    printf("Testing font rendering logic for 'A':\n");
    printf("Font data for 'A': ");
    for (int i = 0; i < 8; i++) {
        printf("0x%02X ", font_A[i]);
    }
    printf("\n\n");
    
    print_char_bitmap(font_A);
    
    printf("\n\nHow ili9225_draw_text processes it:\n");
    printf("for (uint8_t col = 0; col < 8; col++) {\n");
    printf("    uint8_t line = font6x8[c][col];\n");
    printf("    for (uint8_t row = 0; row < 8; row++) {\n");
    printf("        if (line & 0x80) { draw_pixel(cur_x + col, cur_y + row, color); }\n");
    printf("        line <<= 1;\n");
    printf("    }\n");
    printf("}\n\n");
    
    // Simulate the actual drawing
    printf("Drawing simulation:\n");
    for (int col = 0; col < 8; col++) {
        printf("Column %d (0x%02X): ", col, font_A[col]);
        uint8_t line = font_A[col];
        for (int row = 0; row < 8; row++) {
            if (line & 0x80) {
                printf("█");
            } else {
                printf("·");
            }
            line <<= 1;
        }
        printf("\n");
    }
    
    return 0;
}
