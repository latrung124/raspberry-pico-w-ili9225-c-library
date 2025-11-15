/**
 * @file test_real_ili9225.c
 * @brief Test the ACTUAL ili9225.c implementation with simulator
 * 
 * This file tests your real ili9225.c drawing functions by mocking
 * the hardware layer and redirecting output to the simulator.
 * 
 * Usage: ./test_real_ili9225 [options]
 * Options:
 *   --all              Run all tests (default)
 *   --pixels           Test pixel drawing
 *   --lines            Test line drawing
 *   --rectangles       Test rectangle drawing
 *   --circles          Test circle drawing
 *   --text             Test text rendering
 *   --complex          Test complex UI drawing
 *   --help             Show this help message
 * 
 * Examples:
 *   ./test_real_ili9225                    # Run all tests
 *   ./test_real_ili9225 --pixels --lines   # Run only pixel and line tests
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

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
    for (int i = 0; i < 220; i++) {
        ili9225_draw_pixel(lcd, i, i, COLOR_RED);
    }
    sim_mock_flush_framebuffer();
    sleep(1);
}

void test_cross_lines(ili9225_config_t* lcd) {
    printf("\n=== Testing REAL ili9225_draw_cross_lines() ===\n");
    
    ili9225_fill_screen(lcd, COLOR_WHITE);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Draw cross lines using real function
    for (int i = 0; i < 176; i++) {
        ili9225_draw_pixel(lcd, i, 110, COLOR_RED); // Horizontal
    }

    for (int i = 0; i < 220; i++) {
        ili9225_draw_pixel(lcd, 88, i, COLOR_BLUE); // Vertical
    }

    sim_mock_flush_framebuffer();
    sleep(1);
}

void test_diagonal_lines(ili9225_config_t* lcd) {
    printf("\n=== Testing REAL ili9225_draw_diagonal_lines() ===\n");
    
    ili9225_fill_screen(lcd, COLOR_WHITE);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Draw diagonal lines using real function
    for (int i = 0; i < 220; i++) {
        ili9225_draw_pixel(lcd, i, i, COLOR_GREEN);             // Top-left to bottom-right
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
    
    ili9225_draw_line(lcd, 10, 70, 40, 150, COLOR_BLUE);
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
    
    ili9225_draw_text(lcd, 10, 10, "ILI9225 Test", &font_5x8, COLOR_WHITE);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_text(lcd, 10, 40, "Real C Code!", &font_5x8, COLOR_GREEN);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_text(lcd, 10, 60, "Testing", &font_5x8, COLOR_CYAN);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Test individual characters
    ili9225_draw_char(lcd, 10, 90, 'A', &font_5x8, COLOR_RED);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_char(lcd, 30, 90, 'B', &font_5x8, COLOR_GREEN);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    ili9225_draw_char(lcd, 50, 90, 'C', &font_5x8, COLOR_BLUE);
    sim_mock_flush_framebuffer();
    sleep(2);
}

void test_temperature_text(ili9225_config_t* lcd) {
    printf("\n=== Testing REAL Temperature Text Display ===\n");
    
    ili9225_fill_screen(lcd, COLOR_BLACK);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Simulate temperature readings
    for (int temp = -10; temp <= 40; temp += 10) {
        char temp_str[16];
        snprintf(temp_str, sizeof(temp_str), "Temp: %d C", temp);
        
        ili9225_fill_rect(lcd, 0, 80, lcd->width, 20, COLOR_BLACK); // Clear area
        ili9225_draw_text(lcd, 10, 80, temp_str, &font_5x8, COLOR_YELLOW);
        sim_mock_flush_framebuffer();
        sleep(1);
    }
}

void test_real_complex(ili9225_config_t* lcd) {
    printf("\n=== Testing REAL Complex Drawing ===\n");
    
    ili9225_fill_screen(lcd, COLOR_WHITE);
    sim_mock_flush_framebuffer();
    sleep(1);
    
    // Title bar
    ili9225_fill_rect(lcd, 0, 0, 176, 20, COLOR_BLUE);
    sim_mock_flush_framebuffer();
    ili9225_draw_text(lcd, 10, 5, "Real Code UI", &font_5x8, COLOR_WHITE);
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
    ili9225_draw_text(lcd, 5, 205, "Status: OK", &font_5x8, COLOR_GREEN);
    sim_mock_flush_framebuffer();
    sleep(2);
}

void test_fill_screen_with_color(ili9225_config_t* lcd, uint16_t color) {
    printf("\n=== Testing REAL ili9225_fill_screen() with color 0x%04X ===\n", color);
    
    ili9225_fill_screen(lcd, color);
    sim_mock_flush_framebuffer();
    sleep(2);
}

void print_help(const char* program_name) {
    printf("Usage: %s [options]\n\n", program_name);
    printf("Test the real ili9225.c implementation with the web simulator.\n\n");
    printf("Options:\n");
    printf("  --all              Run all tests (default)\n");
    printf("  --pixels           Test pixel drawing (ili9225_draw_pixel)\n");
    printf("  --lines            Test line drawing (ili9225_draw_line)\n");
    printf("  --rectangles       Test rectangle drawing (ili9225_draw_rect, ili9225_fill_rect)\n");
    printf("  --circles          Test circle drawing (ili9225_draw_circle, ili9225_fill_circle)\n");
    printf("  --text             Test text rendering (ili9225_draw_text, ili9225_draw_char)\n");
    printf("  --temperature      Test temperature text display\n");
    printf("  --complex          Test complex UI drawing\n");
    printf("  --fill-screen COLOR Test fill screen with specified color\n");
    printf("                     COLOR can be: black, white, red, green, blue, yellow, cyan, magenta\n");
    printf("                     or a hex value like 0xF800\n");
    printf("  --help             Show this help message\n\n");
    printf("Examples:\n");
    printf("  %s                              # Run all tests\n", program_name);
    printf("  %s --pixels --lines             # Run only pixel and line tests\n", program_name);
    printf("  %s --fill-screen red            # Fill screen with red color\n", program_name);
    printf("  %s --circles --text             # Run circle and text tests\n", program_name);
    printf("\n");
}

uint16_t parse_color(const char* color_str) {
    // Check for color names
    if (strcmp(color_str, "black") == 0) return COLOR_BLACK;
    if (strcmp(color_str, "white") == 0) return COLOR_WHITE;
    if (strcmp(color_str, "red") == 0) return COLOR_RED;
    if (strcmp(color_str, "green") == 0) return COLOR_GREEN;
    if (strcmp(color_str, "blue") == 0) return COLOR_BLUE;
    if (strcmp(color_str, "yellow") == 0) return COLOR_YELLOW;
    if (strcmp(color_str, "cyan") == 0) return COLOR_CYAN;
    if (strcmp(color_str, "magenta") == 0) return COLOR_MAGENTA;
    
    // Try to parse as hex value
    if (strncmp(color_str, "0x", 2) == 0 || strncmp(color_str, "0X", 2) == 0) {
        unsigned int value;
        if (sscanf(color_str, "%x", &value) == 1) {
            return (uint16_t)value;
        }
    }
    
    fprintf(stderr, "Warning: Unknown color '%s', using black\n", color_str);
    return COLOR_BLACK;
}

int main(int argc, char* argv[]) {
    // Parse command-line arguments
    bool run_all = true;
    bool run_pixels = false;
    bool run_lines = false;
    bool run_rectangles = false;
    bool run_circles = false;
    bool run_text = false;
    bool run_complex = false;
    bool run_fill_screen = false;
    bool run_temperature = false;
    bool run_cross_lines = false;
    bool run_diagonal_lines = false;
    uint16_t fill_color = COLOR_BLACK;
    
    // If any specific test is requested, disable run_all
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help(argv[0]);
            return 0;
        }
        
        if (strcmp(argv[i], "--all") == 0) {
            run_all = true;
        } else if (strcmp(argv[i], "--pixels") == 0) {
            run_pixels = true;
            run_all = false;
        } else if (strcmp(argv[i], "--lines") == 0) {
            run_lines = true;
            run_all = false;
        } else if (strcmp(argv[i], "--rectangles") == 0) {
            run_rectangles = true;
            run_all = false;
        } else if (strcmp(argv[i], "--circles") == 0) {
            run_circles = true;
            run_all = false;
        } else if (strcmp(argv[i], "--text") == 0) {
            run_text = true;
            run_all = false;
        } else if (strcmp(argv[i], "--complex") == 0) {
            run_complex = true;
            run_all = false;
        } else if (strcmp(argv[i], "--fill-screen") == 0) {
            run_fill_screen = true;
            run_all = false;
            if (i + 1 < argc) {
                fill_color = parse_color(argv[++i]);
            } else {
                fprintf(stderr, "Error: --fill-screen requires a color argument\n");
                return 1;
            }
        } else if (strcmp(argv[i], "--temperature") == 0) {
            run_temperature = true;
            run_all = false;
        } else if (strcmp(argv[i], "--cross-lines") == 0)  {
            run_cross_lines = true;
            run_all = false;
        } else if (strcmp(argv[i], "--diagonal-lines") == 0)  {
            run_diagonal_lines = true;
            run_all = false;
        } else
        {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            fprintf(stderr, "Use --help for usage information\n");
            return 1;
        }
    }
    
    // If run_all is still true, enable all tests
    if (run_all) {
        run_pixels = true;
        run_lines = true;
        run_rectangles = true;
        run_circles = true;
        run_text = true;
        run_complex = true;
        run_cross_lines = true;
        run_diagonal_lines = true;
    }
    
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
    
    // Run tests based on command-line options
    if (run_pixels) {
        test_real_pixels(&lcd);
    }
    
    if (run_lines) {
        test_real_lines(&lcd);
    }
    
    if (run_rectangles) {
        test_real_rectangles(&lcd);
    }
    
    if (run_circles) {
        test_real_circles(&lcd);
    }
    
    if (run_text) {
        test_real_text(&lcd);
    }
    
    if (run_complex) {
        test_real_complex(&lcd);
    }
    
    if (run_fill_screen) {
        test_fill_screen_with_color(&lcd, fill_color);
    }

    if (run_temperature) {
        test_temperature_text(&lcd);
    }

    if (run_cross_lines) {
        test_cross_lines(&lcd);
    }

    if (run_diagonal_lines) {
        test_diagonal_lines(&lcd);
    }
    
    printf("\n===========================================\n");
    printf("All tests completed!\n");
    printf("Your REAL ili9225.c functions were tested\n");
    printf("Check the web browser to see the results\n");
    printf("===========================================\n\n");
    
    // Cleanup
    sim_mock_close();
    
    return 0;
}
