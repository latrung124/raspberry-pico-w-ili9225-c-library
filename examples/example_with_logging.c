/**
 * @file example_with_logging.c
 * @brief Example demonstrating ILI9225 driver with logging
 * 
 * This example shows how to use the logging module with the ILI9225 driver.
 */

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ili9225.h"
#include "log.h"

// Pin definitions
#define PIN_SCK   2
#define PIN_MOSI  3
#define PIN_MISO  4
#define PIN_CS    5
#define PIN_DC    6
#define PIN_RESET 7

int main() {
    // Initialize logging first
    log_init();
    
    // Set log level based on your needs
    // LOG_LEVEL_ERROR  - Only errors
    // LOG_LEVEL_WARN   - Warnings and errors
    // LOG_LEVEL_INFO   - General info (default)
    // LOG_LEVEL_DEBUG  - Detailed debugging
    // LOG_LEVEL_TRACE  - Very verbose
    log_set_level(LOG_LEVEL_DEBUG);
    
    LOG_INFO("=== ILI9225 LCD Example with Logging ===");
    
    // Initialize the display
    ili9225_config_t lcd_config;
    ili9225_init(&lcd_config, 
                 spi0,
                 PIN_SCK, PIN_MOSI, PIN_MISO,
                 PIN_CS, PIN_DC, PIN_RESET,
                 176, 220,  // 176x220 display
                 ILI9225_PORTRAIT);
    
    LOG_INFO("Display initialized, starting drawing operations");
    
    // Fill screen with black
    LOG_DEBUG("Filling screen with black (0x0000)");
    ili9225_fill_screen(&lcd_config, 0x0000);
    sleep_ms(500);
    
    // Fill screen with white
    LOG_DEBUG("Filling screen with white (0xFFFF)");
    ili9225_fill_screen(&lcd_config, 0xFFFF);
    sleep_ms(500);
    
    // Draw some pixels
    LOG_DEBUG("Drawing colored pixels");
    ili9225_draw_pixel(&lcd_config, 88, 110, 0xF800);  // Red pixel at center
    ili9225_draw_pixel(&lcd_config, 10, 10, 0x07E0);   // Green pixel
    ili9225_draw_pixel(&lcd_config, 165, 10, 0x001F);  // Blue pixel
    
    // Draw a rectangle
    LOG_DEBUG("Drawing rectangle");
    ili9225_draw_rect(&lcd_config, 20, 20, 50, 30, 0xF800);  // Red rectangle
    
    // Draw a filled rectangle
    LOG_DEBUG("Drawing filled rectangle");
    ili9225_fill_rect(&lcd_config, 80, 20, 50, 30, 0x07E0);  // Green filled rectangle
    
    // Draw a circle
    LOG_DEBUG("Drawing circle");
    ili9225_draw_circle(&lcd_config, 88, 110, 40, 0x001F);  // Blue circle
    
    // Draw a line
    LOG_DEBUG("Drawing line");
    ili9225_draw_line(&lcd_config, 10, 200, 165, 200, 0xFFE0);  // Yellow line
    
    LOG_INFO("Drawing operations complete");
    
    // Example: Hexdump some data
    uint8_t test_data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    log_hexdump(LOG_LEVEL_DEBUG, "Test Data", test_data, sizeof(test_data));
    
    // Example: Different log levels
    LOG_ERROR("This is an error message");
    LOG_WARN("This is a warning message");
    LOG_INFO("This is an info message");
    LOG_DEBUG("This is a debug message");
    LOG_TRACE("This is a trace message");
    
    // Example: Simple logging (without file/line info)
    LOG_INFO_SIMPLE("Simple info message");
    LOG_DEBUG_SIMPLE("Simple debug with value: %d", 42);
    
    LOG_INFO("Example finished, entering main loop");
    
    // Main loop
    while (true) {
        // You can change log level at runtime
        static uint32_t counter = 0;
        counter++;
        
        if (counter % 1000000 == 0) {
            LOG_INFO_SIMPLE("Still alive, counter: %lu", counter);
        }
        
        tight_loop_contents();
    }
    
    return 0;
}
