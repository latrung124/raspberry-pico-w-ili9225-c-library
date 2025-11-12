/**
 * @file: ili9225.h
 * @author trung.la
 * @date November 12 2025
 * @brief Header file for ILI9225 LCD driver for Raspberry Pi Pico
 * 
 * Copyright (c) 2025 trung.la
 */

#ifndef ILI9225_H
#define ILI9225_H

#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ili9225_defs.h"
#include "utilities/utils.h"

// Structure to hold ILI9225 configuration
typedef struct {
    spi_inst_t* spi_inst; // SPI instance (e.g., spi0 or spi1)
    uint pin_sck;   // SPI clock pin
    uint pin_mosi;  // SPI MOSI pin
    uint pin_miso; // SPI MISO pin (if used)
    uint pin_cs;    // Chip select pin
    uint pin_dc;    // Data/Command control pin
    uint pin_reset; // Reset pin

    uint16_t width;    // Display width in pixels
    uint16_t height;   // Display height in pixels
    ili9225_rotation_t rotation; // Initial rotation
} ili9225_config_t;


// Public API

/**
 * @brief Initialize the ILI9225 LCD display with individual parameters
 * 
 * @param config Configuration
 * @param spi SPI instance (e.g., spi0 or spi1)
 * @param pin_sck SPI clock pin
 * @param pin_mosi SPI MOSI pin
 * @param pin_miso SPI MISO pin (if used)
 * @param pin_cs Chip select pin
 * @param pin_dc Data/Command control pin
 * @param pin_reset Reset pin
 * @param width Display width in pixels
 * @param height Display height in pixels
 * @param rotation Initial rotation
 * @return void
 */
void ili9225_init(ili9225_config_t* config, spi_inst_t* spi, uint pin_sck, uint pin_mosi, uint pin_miso,
                  uint pin_cs, uint pin_dc, uint pin_reset,
                  uint16_t width, uint16_t height, ili9225_rotation_t rotation);

/**
 * @brief Set the rotation of the ILI9225 LCD display
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param rotation Desired rotation (0, 90, 180, 270 degrees)
 * @return void
 */
void ili9225_set_rotation(ili9225_config_t* config, ili9225_rotation_t rotation);

/**
 * @brief Fill the entire screen with a specific color
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param color 16-bit color value to fill the screen with
 * @return void
 */
void ili9225_fill_screen(ili9225_config_t* config, uint16_t color);

/**
 * @brief Draw a pixel at specified coordinates with a specific color
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color 16-bit color value of the pixel
 * @return void
 */
void ili9225_draw_pixel(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Draw a line from (x0, y0) to (x1, y1) with a specific color
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x0 Starting X coordinate
 * @param y0 Starting Y coordinate
 * @param x1 Ending X coordinate
 * @param y1 Ending Y coordinate
 * @param color 16-bit color value of the line
 * @return void
 */
void ili9225_draw_line(ili9225_config_t* config, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/**
 * @brief Draw a rectangle at specified coordinates with width, height, and color
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x X coordinate of the top-left corner
 * @param y Y coordinate of the top-left corner
 * @param w Width of the rectangle
 * @param h Height of the rectangle
 * @param color 16-bit color value of the rectangle
 * @return void
 */
void ili9225_draw_rect(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * @brief Fill a rectangle at specified coordinates with width, height, and color
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x X coordinate of the top-left corner
 * @param y Y coordinate of the top-left corner
 * @param w Width of the rectangle
 * @param h Height of the rectangle
 * @param color 16-bit color value to fill the rectangle with
 * @return void
 */
void ili9225_fill_rect(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * @brief Draw a circle at specified coordinates with radius and color
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x X coordinate of the center
 * @param y Y coordinate of the center
 * @param r Radius of the circle
 * @param color 16-bit color value of the circle
 * @return void
 */
void ili9225_draw_circle(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t r, uint16_t color);

/**
 * @brief Fill a circle at specified coordinates with radius and color
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x X coordinate of the center
 * @param y Y coordinate of the center
 * @param r Radius of the circle
 * @param color 16-bit color value to fill the circle with
 * @return void
 */
void ili9225_fill_circle(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t r, uint16_t color);

/**
 * @brief Draw a triangle given three vertices and a color
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x0 X coordinate of the first vertex
 * @param y0 Y coordinate of the first vertex
 * @param x1 X coordinate of the second vertex
 * @param y1 Y coordinate of the second vertex
 * @param x2 X coordinate of the third vertex
 * @param y2 Y coordinate of the third vertex
 * @param color 16-bit color value of the triangle
 * @return void
 */
void ili9225_draw_triangle(ili9225_config_t* config, uint16_t x0, uint16_t y0,
                   uint16_t x1, uint16_t y1,
                   uint16_t x2, uint16_t y2,
                   uint16_t color);

/**
 * @brief Fill a triangle given three vertices and a color
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x0 X coordinate of the first vertex
 * @param y0 Y coordinate of the first vertex
 * @param x1 X coordinate of the second vertex
 * @param y1 Y coordinate of the second vertex
 * @param x2 X coordinate of the third vertex
 * @param y2 Y coordinate of the third vertex
 * @param color 16-bit color value to fill the triangle with
 * @return void
 */
void ili9225_fill_triangle(ili9225_config_t* config, uint16_t x0, uint16_t y0,
                   uint16_t x1, uint16_t y1,
                   uint16_t x2, uint16_t y2,
                   uint16_t color);

/**
 * @brief Draw text at specified coordinates with color and size
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x X coordinate of the top-left corner of the text
 * @param y Y coordinate of the top-left corner of the text
 * @param text Null-terminated string to draw
 * @param color 16-bit color value of the text
 * @param size Text size (scaling factor)
 * @return void
 */
void ili9225_draw_text(ili9225_config_t* config, uint16_t x, uint16_t y,
               const char* text, uint16_t color, uint8_t size);

/**
 * @brief Draw a single character at specified coordinates with color and size
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x X coordinate of the top-left corner of the character
 * @param y Y coordinate of the top-left corner of the character
 * @param c Character to draw
 * @param color 16-bit color value of the character
 * @param size Character size (scaling factor)
 * @return void
 */
void ili9225_draw_char(ili9225_config_t* config, uint16_t x, uint16_t y,
               char c, uint16_t color, uint8_t size);

/**
 * @brief Draw a bitmap image at specified coordinates
 * 
 * @param config Pointer to ili9225_config_t structure
 * @param x X coordinate of the top-left corner of the bitmap
 * @param y Y coordinate of the top-left corner of the bitmap
 * @param bitmap Pointer to the bitmap data (array of 16-bit color values)
 * @param w Width of the bitmap
 * @param h Height of the bitmap
 * @return void
 */
void ili9225_draw_bitmap(ili9225_config_t* config, uint16_t x, uint16_t y,
                 const uint8_t* bitmap, uint16_t w, uint16_t h);

#endif // ILI9225_H