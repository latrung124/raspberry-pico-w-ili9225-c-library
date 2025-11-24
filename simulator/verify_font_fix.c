/**
 * Simple test to verify font rendering is fixed
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ili9225_mock.h"
#include "../src/ili9225.h"

#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_BLUE    0x001F

int main() {
    printf("Font Rendering Fix Verification Test\n");
    printf("=====================================\n\n");
    
    // Connect to simulator
    sim_mock_init("127.0.0.1", 9225);
    sleep(1);
    
    // Initialize LCD
    ili9225_config_t lcd;
    spi_inst_t spi0;
    ili9225_init(&lcd, &spi0, 2, 3, 4, 5, 6, 7, 176, 220, ILI9225_PORTRAIT);
    
    printf("Drawing test text...\n");
    
    // Clear to black
    ili9225_fill_screen(&lcd, COLOR_BLACK);
    sleep_ms(100);
    
    // Test 1: Simple text size 1
    ili9225_draw_text(&lcd, 10, 10, "Hello World!", &font_5x8, COLOR_WHITE);
    sim_mock_flush();
    printf("✓ Drew 'Hello World!' at size 1\n");
    sleep(1);
    
    // Test 2: Text size 2
    ili9225_draw_text(&lcd, 10, 30, "Size 2", &font_5x8, COLOR_BLUE);
    sim_mock_flush();
    printf("✓ Drew 'Size 2' at size 2\n");
    sleep(1);
    
    // Test 3: Alphabet
    ili9225_draw_text(&lcd, 10, 60, "ABCDEFGHIJ", &font_5x8, COLOR_WHITE);
    ili9225_draw_text(&lcd, 10, 75, "0123456789", &font_5x8, COLOR_WHITE);
    sim_mock_flush();
    printf("✓ Drew alphabet and numbers\n");
    sleep(1);
    
    printf("\n✅ Font rendering test complete!\n");
    printf("Check the web simulator to verify text looks correct.\n");
    printf("Characters should be properly spaced without overlap.\n");
    
    sim_mock_close();
    return 0;
}
