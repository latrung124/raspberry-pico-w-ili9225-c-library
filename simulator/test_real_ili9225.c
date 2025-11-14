/**
 * @file test_real_ili9225.c
 * @brief Test the ACTUAL ili9225.c implementation with simulator
 * 
 * This file tests your real ili9225.c drawing functions by mocking
 * the hardware layer and redirecting output to the simulator.
 */

#include <stdio.h>
#include <unistd.h>

// Mock the hardware first
#include "ili9225_mock.h"

// Now include the real ILI9225 header
// (the implementation will be linked separately)
#define ILI9225_DEBUG_LOGGING  // Enable logging in ili9225.c
#include "../src/ili9225.h"

// RGB565 colors
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F

void test_real_pixels(ili9225_config_t* lcd) {
    printf("\n=== Testing REAL ili9225_draw_pixel() ===\n");
    
    ili9225_fill_screen(lcd, COLOR_WHITE);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Draw diagonal line using real function
    for (int i = 0; i < 100; i++) {
        ili9225_draw_pixel(lcd, i, i, COLOR_RED);
    }
    sim_mock_flush_framebuffer();
    sleep(1);
}

void test_real_lines(ili9225_config_t* lcd) {
    printf("\n=== Testing REAL ili9225_draw_line() ===\n");
    
    ili9225_fill_screen(lcd, COLOR_BLACK);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_line(lcd, 10, 10, 166, 10, COLOR_RED);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_line(lcd, 10, 30, 166, 50, COLOR_GREEN);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_line(lcd, 10, 70, 100, 150, COLOR_BLUE);
    sim_mock_flush_framebuffer();
    sleep(1);
}

void test_real_rectangles(ili9225_config_t* lcd) {
    printf("\n=== Testing REAL ili9225_draw_rect() & ili9225_fill_rect() ===\n");
    
    ili9225_fill_screen(lcd, COLOR_BLACK);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_rect(lcd, 20, 20, 50, 40, COLOR_RED);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_fill_rect(lcd, 80, 20, 50, 40, COLOR_GREEN);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_rect(lcd, 20, 80, 50, 40, COLOR_BLUE);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_fill_rect(lcd, 80, 80, 50, 40, COLOR_YELLOW);
    sim_mock_flush_framebuffer();
    sleep(1);
}

void test_real_circles(ili9225_config_t* lcd) {
    printf("\n=== Testing REAL ili9225_draw_circle() & ili9225_fill_circle() ===\n");
    
    ili9225_fill_screen(lcd, COLOR_WHITE);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Test outline circles
    ili9225_draw_circle(lcd, 88, 60, 30, COLOR_RED);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_circle(lcd, 88, 60, 40, COLOR_GREEN);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_circle(lcd, 88, 60, 50, COLOR_BLUE);
    sim_mock_flush_framebuffer();
    sleep(2);
    
    // Test filled circles
    ili9225_fill_screen(lcd, COLOR_BLACK);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_fill_circle(lcd, 40, 50, 30, COLOR_RED);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_fill_circle(lcd, 88, 80, 25, COLOR_GREEN);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_fill_circle(lcd, 136, 50, 30, COLOR_BLUE);
    sim_mock_flush_framebuffer();
    sleep(2);
}

void test_real_text(ili9225_config_t* lcd) {
    printf("\n=== Testing REAL ili9225_draw_text() & ili9225_draw_char() ===\n");
    
    ili9225_fill_screen(lcd, COLOR_BLACK);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_text(lcd, 10, 10, "ILI9225 Test", COLOR_WHITE, 2);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_text(lcd, 10, 40, "Real C Code!", COLOR_GREEN, 1);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_text(lcd, 10, 60, "Testing", COLOR_CYAN, 1);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Test individual characters
    ili9225_draw_char(lcd, 10, 90, 'A', COLOR_RED, 2);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_char(lcd, 30, 90, 'B', COLOR_GREEN, 2);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_char(lcd, 50, 90, 'C', COLOR_BLUE, 2);
    sim_mock_flush_framebuffer();
    sleep(2);
}

void test_real_complex(ili9225_config_t* lcd) {
    printf("\n=== Testing REAL Complex Drawing ===\n");
    
    ili9225_fill_screen(lcd, COLOR_WHITE);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Title bar
    ili9225_fill_rect(lcd, 0, 0, 176, 20, COLOR_BLUE);
    sim_mock_flush_framebuffer();
    ili9225_draw_text(lcd, 10, 5, "Real Code UI", COLOR_WHITE, 1);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Content area
    ili9225_draw_rect(lcd, 5, 25, 166, 150, COLOR_BLACK);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Draw shapes
    ili9225_fill_circle(lcd, 50, 80, 20, COLOR_RED);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_fill_rect(lcd, 100, 60, 40, 40, COLOR_GREEN);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_circle(lcd, 120, 120, 25, COLOR_BLUE);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Status bar
    ili9225_fill_rect(lcd, 0, 200, 176, 20, COLOR_BLACK);
    sim_mock_flush_framebuffer();
    ili9225_draw_text(lcd, 5, 205, "Status: OK", COLOR_GREEN, 1);
    sim_mock_flush_framebuffer();
    sleep(2);
}

void test_fill_screen_with_color(ili9225_config_t* lcd, uint16_t color) {
    printf("\n=== Testing REAL ili9225_fill_screen() with color 0x%04X ===\n", color);
    
    ili9225_fill_screen(lcd, color);
    sim_mock_flush_framebuffer();
    sleep(2);
}

int main(void) {
    printf("===========================================\n");
    printf("Testing REAL ili9225.c Implementation\n");
    printf("===========================================\n\n");
    
    // Initialize mock connection to simulator
    printf("Connecting to simulator...\n");
    sim_mock_init("127.0.0.1", 9225);
    sleep(1);
    
    // Create ILI9225 config using the REAL ili9225_init() function
    printf("\nInitializing REAL ili9225_init()...\n");
    ili9225_config_t lcd;
    ili9225_init(&lcd, spi0,
                 2, 3, 4,  // SCK, MOSI, MISO (mock pins)
                 5, 6, 7,  // CS, DC, RST (mock pins)
                 176, 220, // Width, Height
                 ILI9225_PORTRAIT);
    
    printf("\n===========================================\n");
    printf("Running Test Suite on REAL Functions\n");
    printf("===========================================\n");
    
    // Run tests using REAL ili9225.c functions
    // test_real_pixels(&lcd);
    // test_real_lines(&lcd);
    // test_real_rectangles(&lcd);
    // test_real_circles(&lcd);
    // test_real_text(&lcd);
    // test_real_complex(&lcd);
    test_fill_screen_with_color(&lcd, COLOR_BLACK);
    test_fill_screen_with_color(&lcd, COLOR_WHITE);
    test_fill_screen_with_color(&lcd, COLOR_RED);
    test_fill_screen_with_color(&lcd, COLOR_GREEN);
    test_fill_screen_with_color(&lcd, COLOR_BLUE);
    
    printf("\n===========================================\n");
    printf("All tests completed!\n");
    printf("Your REAL ili9225.c functions were tested\n");
    printf("Check the web browser to see the results\n");
    printf("===========================================\n\n");
    
    // Cleanup
    sim_mock_close();
    
    return 0;
}
