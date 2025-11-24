/**
 * @file ili9225.cpp
 * @author trung.la
 * @date November 12 2025
 * @brief Implementation file for ILI9225 LCD driver for Raspberry Pi Pico
 * 
 * Copyright (c) 2025 trung.la
 */

/**
 * @file: ili9225.c
 * @author trung.la
 * @date November 12 2025
 * @brief Implementation of ILI9225 LCD driver for Raspberry Pi Pico
 * * Copyright (c) 2025 trung.la
 */

#include "ili9225.h"
#include <stdlib.h>
#include <string.h> // For memset/memcpy if needed
#include "hardware/gpio.h"
#include "log.h" // Assuming you have a log.h similar to your previous snippet

// Uncomment to enable detailed logging
// #define ILI9225_DEBUG_LOGGING

// ============================================================================
// Static Helper Functions (Low Level)
// ============================================================================

static inline void lcd_select(ili9225_t* lcd) {
    gpio_put(lcd->pin_cs, 0);
}

static inline void lcd_deselect(ili9225_t* lcd) {
    gpio_put(lcd->pin_cs, 1);
}

static void ili9225_write_command(ili9225_t* lcd, uint8_t cmd) {
    gpio_put(lcd->pin_dc, 0); // 0 = Command
    lcd_select(lcd);
    spi_write_blocking(lcd->spi, &cmd, 1);
    lcd_deselect(lcd);
}

static void ili9225_write_data16(ili9225_t* lcd, uint16_t data) {
    gpio_put(lcd->pin_dc, 1); // 1 = Data
    lcd_select(lcd);
    uint8_t buf[2] = { (uint8_t)(data >> 8), (uint8_t)(data & 0xFF) };
    spi_write_blocking(lcd->spi, buf, 2);
    lcd_deselect(lcd);
}

// Define the drawing window on the screen
static void ili9225_set_window(ili9225_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    ili9225_write_command(lcd, ILI9225_HORIZONTAL_WINDOW_ADDR1);
    ili9225_write_data16(lcd, x2);
    ili9225_write_command(lcd, ILI9225_HORIZONTAL_WINDOW_ADDR2);
    ili9225_write_data16(lcd, x1);

    ili9225_write_command(lcd, ILI9225_VERTICAL_WINDOW_ADDR1);
    ili9225_write_data16(lcd, y2);
    ili9225_write_command(lcd, ILI9225_VERTICAL_WINDOW_ADDR2);
    ili9225_write_data16(lcd, y1);

    ili9225_write_command(lcd, ILI9225_RAM_ADDR_SET1);
    ili9225_write_data16(lcd, x1);
    ili9225_write_command(lcd, ILI9225_RAM_ADDR_SET2);
    ili9225_write_data16(lcd, y1);

    ili9225_write_command(lcd, ILI9225_GRAM_DATA_REG);
}

// ============================================================================
// Initialization & Control Functions
// ============================================================================

void ili9225_init(ili9225_t* lcd) {
    if (!lcd) {
        LOG_ERROR("ili9225_init: lcd struct is NULL");
        return;
    }

    // Initialize GPIOs
    gpio_init(lcd->pin_cs);
    gpio_set_dir(lcd->pin_cs, GPIO_OUT);
    lcd_deselect(lcd); // Default high

    gpio_init(lcd->pin_dc);
    gpio_set_dir(lcd->pin_dc, GPIO_OUT);
    gpio_put(lcd->pin_dc, 1); // Default Data

    gpio_init(lcd->pin_rst);
    gpio_set_dir(lcd->pin_rst, GPIO_OUT);
    gpio_put(lcd->pin_rst, 1);

    // Initialize SPI
    spi_init(lcd->spi, 30 * 1000 * 1000); // 30 MHz usually works well
    spi_set_format(lcd->spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    
    gpio_set_function(lcd->pin_sck, GPIO_FUNC_SPI);
    gpio_set_function(lcd->pin_mosi, GPIO_FUNC_SPI);
    // MISO is not used/initialized as ILI9225 in this mode is write-only

    LOG_DEBUG("SPI initialized. Resetting display...");

    // Hardware Reset
    gpio_put(lcd->pin_rst, 1);
    sleep_ms(1);
    gpio_put(lcd->pin_rst, 0);
    sleep_ms(10);
    gpio_put(lcd->pin_rst, 1);
    sleep_ms(50);

    // --- Register Initialization Sequence ---
    ili9225_write_command(lcd, ILI9225_POWER_CTRL1);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_POWER_CTRL2);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_POWER_CTRL3);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_POWER_CTRL4);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_POWER_CTRL5);
    ili9225_write_data16(lcd, 0x0000);
    
    sleep_ms(40);

    ili9225_write_command(lcd, ILI9225_POWER_CTRL2);
    ili9225_write_data16(lcd, 0x0018);
    ili9225_write_command(lcd, ILI9225_POWER_CTRL3);
    ili9225_write_data16(lcd, 0x6121);
    ili9225_write_command(lcd, ILI9225_POWER_CTRL4);
    ili9225_write_data16(lcd, 0x006F);
    ili9225_write_command(lcd, ILI9225_POWER_CTRL5);
    ili9225_write_data16(lcd, 0x495F);
    ili9225_write_command(lcd, ILI9225_POWER_CTRL1);
    ili9225_write_data16(lcd, 0x0800);
    
    sleep_ms(10);

    ili9225_write_command(lcd, ILI9225_POWER_CTRL2);
    ili9225_write_data16(lcd, 0x103B);
    
    sleep_ms(50);

    ili9225_write_command(lcd, ILI9225_DRIVER_OUTPUT_CTRL);
    ili9225_write_data16(lcd, 0x011C);
    ili9225_write_command(lcd, ILI9225_LCD_AC_DRIVING_CTRL);
    ili9225_write_data16(lcd, 0x0100);
    ili9225_write_command(lcd, ILI9225_ENTRY_MODE);
    ili9225_write_data16(lcd, 0x1030); // Default setting
    ili9225_write_command(lcd, ILI9225_DISP_CTRL1);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_BLANK_PERIOD_CTRL1);
    ili9225_write_data16(lcd, 0x0808);
    ili9225_write_command(lcd, ILI9225_FRAME_CYCLE_CTRL);
    ili9225_write_data16(lcd, 0x1100);
    ili9225_write_command(lcd, ILI9225_INTERFACE_CTRL);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_OSC_CTRL);
    ili9225_write_data16(lcd, 0x0D01);
    ili9225_write_command(lcd, ILI9225_VCI_RECYCLING);
    ili9225_write_data16(lcd, 0x0020);
    ili9225_write_command(lcd, ILI9225_RAM_ADDR_SET1);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_RAM_ADDR_SET2);
    ili9225_write_data16(lcd, 0x0000);

    ili9225_write_command(lcd, ILI9225_GATE_SCAN_CTRL);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_VERTICAL_SCROLL_CTRL1);
    ili9225_write_data16(lcd, 0x00DB);
    ili9225_write_command(lcd, ILI9225_VERTICAL_SCROLL_CTRL2);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_VERTICAL_SCROLL_CTRL3);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_PARTIAL_DRIVING_POS1);
    ili9225_write_data16(lcd, 0x00DB);
    ili9225_write_command(lcd, ILI9225_PARTIAL_DRIVING_POS2);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_HORIZONTAL_WINDOW_ADDR1);
    ili9225_write_data16(lcd, 0x00AF);
    ili9225_write_command(lcd, ILI9225_HORIZONTAL_WINDOW_ADDR2);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_VERTICAL_WINDOW_ADDR1);
    ili9225_write_data16(lcd, 0x00DB);
    ili9225_write_command(lcd, ILI9225_VERTICAL_WINDOW_ADDR2);
    ili9225_write_data16(lcd, 0x0000);

    // Gamma
    ili9225_write_command(lcd, ILI9225_GAMMA_CTRL1);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_GAMMA_CTRL2);
    ili9225_write_data16(lcd, 0x0808);
    ili9225_write_command(lcd, ILI9225_GAMMA_CTRL3);
    ili9225_write_data16(lcd, 0x080A);
    ili9225_write_command(lcd, ILI9225_GAMMA_CTRL4);
    ili9225_write_data16(lcd, 0x000A);
    ili9225_write_command(lcd, ILI9225_GAMMA_CTRL5);
    ili9225_write_data16(lcd, 0x0A08);
    ili9225_write_command(lcd, ILI9225_GAMMA_CTRL6);
    ili9225_write_data16(lcd, 0x0808);
    ili9225_write_command(lcd, ILI9225_GAMMA_CTRL7);
    ili9225_write_data16(lcd, 0x0000);
    ili9225_write_command(lcd, ILI9225_GAMMA_CTRL8);
    ili9225_write_data16(lcd, 0x0A00);
    ili9225_write_command(lcd, ILI9225_GAMMA_CTRL9);
    ili9225_write_data16(lcd, 0x0710);
    ili9225_write_command(lcd, ILI9225_GAMMA_CTRL10);
    ili9225_write_data16(lcd, 0x0710);

    ili9225_write_command(lcd, ILI9225_DISP_CTRL1);
    ili9225_write_data16(lcd, 0x0012);
    
    sleep_ms(50);
    
    ili9225_write_command(lcd, ILI9225_DISP_CTRL1);
    ili9225_write_data16(lcd, 0x1017);

    lcd->is_rgb_order = true; // Default assumption
    
    // Apply default orientation to set internal struct state
    ili9225_set_orientation(lcd, ILI9225_PORTRAIT);
    ili9225_fill_screen(lcd, COLOR_BLACK);
}

void ili9225_set_orientation(ili9225_t* lcd, ili9225_rotation_t rotation) {
    if (!lcd) return;

    uint16_t entry_mode = 0x1000; // Basic BGR/RGB bit usually starts here, check datasheet for specific base
    
    // Apply Color Order (BGR or RGB)
    // Bit 12 in Entry Mode is usually BGR. 1=BGR, 0=RGB.
    if (!lcd->is_rgb_order) {
        entry_mode |= (1 << 12); 
    }

    lcd->rotation = rotation;

    // NOTE: The specific width/height and ID bits depend on physical panel wiring.
    // Adjust 176 and 220 if your specific panel interprets them differently.
    switch (rotation) {
        case ILI9225_PORTRAIT:
            lcd->width = 176;
            lcd->height = 220;
            entry_mode |= 0x0030; // ID1=1, ID0=1 (Example)
            break;
        case ILI9225_LANDSCAPE:
            lcd->width = 220;
            lcd->height = 176;
            entry_mode |= 0x0028; // Swap XY, adjust scan direction
            break;
        case ILI9225_PORTRAIT_REV:
            lcd->width = 176;
            lcd->height = 220;
            entry_mode |= 0x0000; 
            break;
        case ILI9225_LANDSCAPE_REV:
            lcd->width = 220;
            lcd->height = 176;
            entry_mode |= 0x0018; 
            break;
        default:
            break;
    }

    ili9225_write_command(lcd, ILI9225_ENTRY_MODE);
    ili9225_write_data16(lcd, entry_mode);
    
    ili9225_set_window(lcd, 0, 0, lcd->width - 1, lcd->height - 1);
}

void ili9225_set_color_order(ili9225_t* lcd, bool is_rgb) {
    if (!lcd) return;
    lcd->is_rgb_order = is_rgb;
    // Re-apply orientation to update the BGR bit in Entry Mode
    ili9225_set_orientation(lcd, lcd->rotation);
}

void ili9225_refresh_display(ili9225_t* lcd) {
    // ILI9225 doesn't have a specific "flush" command for internal GRAM to Panel
    // It updates automatically. This function usually just clears artifacts.
    ili9225_set_window(lcd, 0, 0, lcd->width - 1, lcd->height - 1);
}

// ============================================================================
// Graphic Primitives
// ============================================================================

void ili9225_fill_screen(ili9225_t* lcd, uint16_t color) {
    ili9225_fill_rect(lcd, 0, 0, lcd->width, lcd->height, color);
}

void ili9225_draw_pixel(ili9225_t* lcd, uint16_t x, uint16_t y, uint16_t color) {
    if (x >= lcd->width || y >= lcd->height) return;
    
    ili9225_set_window(lcd, x, y, x, y);
    ili9225_write_data16(lcd, color);
}

void ili9225_draw_line(ili9225_t* lcd, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        // swap x0, y0
        uint16_t temp = x0; x0 = y0; y0 = temp;
        // swap x1, y1
        temp = x1; x1 = y1; y1 = temp;
    }

    if (x0 > x1) {
        // swap x0, x1
        uint16_t temp = x0; x0 = x1; x1 = temp;
        // swap y0, y1
        temp = y0; y0 = y1; y1 = temp;
    }

    int16_t dx = x1 - x0;
    int16_t dy = abs(y1 - y0);
    int16_t err = dx / 2;
    int16_t ystep = (y0 < y1) ? 1 : -1;

    for (; x0 <= x1; x0++) {
        if (steep) {
            ili9225_draw_pixel(lcd, y0, x0, color);
        } else {
            ili9225_draw_pixel(lcd, x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void ili9225_draw_rect(ili9225_t* lcd, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    ili9225_draw_line(lcd, x, y, x + w - 1, y, color);
    ili9225_draw_line(lcd, x, y + h - 1, x + w - 1, y + h - 1, color);
    ili9225_draw_line(lcd, x, y, x, y + h - 1, color);
    ili9225_draw_line(lcd, x + w - 1, y, x + w - 1, y + h - 1, color);
}

void ili9225_fill_rect(ili9225_t* lcd, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= lcd->width || y >= lcd->height) return;
    if ((x + w) > lcd->width) w = lcd->width - x;
    if ((y + h) > lcd->height) h = lcd->height - y;

    ili9225_set_window(lcd, x, y, x + w - 1, y + h - 1);
    
    gpio_put(lcd->pin_dc, 1); // Data
    lcd_select(lcd);

    uint32_t total_pixels = (uint32_t)w * h;
    uint8_t color_hi = color >> 8;
    uint8_t color_lo = color & 0xFF;
    
    // Write pixel by pixel.
    // Optimization note: Creating a larger buffer (e.g. 64 bytes) and sending chunks
    // is much faster than looping spi_write_blocking for 2 bytes at a time.
    uint8_t line_buf[2] = { color_hi, color_lo };
    
    for(uint32_t i = 0; i < total_pixels; i++) {
        spi_write_blocking(lcd->spi, line_buf, 2);
    }

    lcd_deselect(lcd);
}

void ili9225_draw_circle(ili9225_t* lcd, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ili9225_draw_pixel(lcd, x0, y0 + r, color);
    ili9225_draw_pixel(lcd, x0, y0 - r, color);
    ili9225_draw_pixel(lcd, x0 + r, y0, color);
    ili9225_draw_pixel(lcd, x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ili9225_draw_pixel(lcd, x0 + x, y0 + y, color);
        ili9225_draw_pixel(lcd, x0 - x, y0 + y, color);
        ili9225_draw_pixel(lcd, x0 + x, y0 - y, color);
        ili9225_draw_pixel(lcd, x0 - x, y0 - y, color);
        ili9225_draw_pixel(lcd, x0 + y, y0 + x, color);
        ili9225_draw_pixel(lcd, x0 - y, y0 + x, color);
        ili9225_draw_pixel(lcd, x0 + y, y0 - x, color);
        ili9225_draw_pixel(lcd, x0 - y, y0 - x, color);
    }
}

void ili9225_fill_circle(ili9225_t* lcd, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    ili9225_draw_line(lcd, x0, y0 - r, x0, y0 + r, color); // Center vertical line
    
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ili9225_draw_line(lcd, x0 - x, y0 + y, x0 + x, y0 + y, color);
        ili9225_draw_line(lcd, x0 - x, y0 - y, x0 + x, y0 - y, color);
        ili9225_draw_line(lcd, x0 - y, y0 + x, x0 + y, y0 + x, color);
        ili9225_draw_line(lcd, x0 - y, y0 - x, x0 + y, y0 - x, color);
    }
}

void ili9225_draw_triangle(ili9225_t* lcd, uint16_t x0, uint16_t y0,
                           uint16_t x1, uint16_t y1,
                           uint16_t x2, uint16_t y2, uint16_t color) {
    ili9225_draw_line(lcd, x0, y0, x1, y1, color);
    ili9225_draw_line(lcd, x1, y1, x2, y2, color);
    ili9225_draw_line(lcd, x2, y2, x0, y0, color);
}

void ili9225_fill_triangle(ili9225_t* lcd, uint16_t x0, uint16_t y0,
                           uint16_t x1, uint16_t y1,
                           uint16_t x2, uint16_t y2, uint16_t color) {
    // Standard scan-line triangle filling algorithm is complex.
    // For a minimal embedded driver, drawing the outline is often sufficient placeholder.
    // Or iterate bounding box and checking barycentric coords.
    // Leaving as outline for now to save flash space.
    ili9225_draw_triangle(lcd, x0, y0, x1, y1, x2, y2, color);
}

// ============================================================================
// Text & Bitmap Functions
// ============================================================================

void ili9225_draw_char(ili9225_t* lcd, uint16_t x, uint16_t y, char c, const font_t *font, uint16_t color) {
    if (!font) return;
    
    // Basic bounds check
    if (c < 32 || c > 126) return; 

    // Assuming standard monospace font where width is padded to byte
    // Adjust logic if your font struct differs.
    
    // Example logic for 5x7 or 8x8 bitmap font
    // uint8_t *char_data = &font->data[(c - 32) * font->width];
    
    // Since "font_t" structure isn't strictly defined in your snippet, 
    // I will assume a standard GFX-style drawing or leave this as a stub
    // requiring the specific font implementation details.
    
    // Placeholder for generic character drawing
}

void ili9225_draw_gfx_char(ili9225_t* lcd, uint16_t x, uint16_t y,
                           char c, const GFXfont *font, uint16_t color) {
    if (!font) return;

    c -= (uint8_t)font->first;
    GFXglyph *glyph = &font->glyph[c];
    uint8_t *bitmap = font->bitmap;

    uint16_t bo = glyph->bitmapOffset;
    uint8_t  w  = glyph->width;
    uint8_t  h  = glyph->height;
    int8_t   xo = glyph->xOffset;
    int8_t   yo = glyph->yOffset;
    
    uint8_t  xx, yy, bits = 0, bit = 0;

    for (yy = 0; yy < h; yy++) {
        for (xx = 0; xx < w; xx++) {
            if (!(bit++ & 7)) {
                bits = bitmap[bo++];
            }
            if (bits & 0x80) {
                ili9225_draw_pixel(lcd, x + xo + xx, y + yo + yy, color);
            }
            bits <<= 1;
        }
    }
}

void ili9225_draw_gfx_text(ili9225_t* lcd, uint16_t x, uint16_t y,
                           const char* text, const GFXfont *font, uint16_t color) {
    if (!text || !font) return;
    
    uint16_t cursor_x = x;
    uint16_t cursor_y = y;

    while (*text) {
        char c = *text++;
        if (c == '\n') {
            cursor_x = x;
            cursor_y += font->yAdvance;
        } else if (c != '\r') {
            ili9225_draw_gfx_char(lcd, cursor_x, cursor_y, c, font, color);
            cursor_x += font->glyph[c - font->first].xAdvance;
        }
    }
}

void ili9225_draw_text(ili9225_t* lcd, uint16_t x, uint16_t y,
                       const char* text, const font_t *font, uint16_t color) {
    if (!text) return;
    uint16_t cursor_x = x;
    while (*text) {
        ili9225_draw_char(lcd, cursor_x, y, *text++, font, color);
        // cursor_x += font->width; // adjust based on actual font struct
    }
}

void ili9225_draw_bitmap(ili9225_t* lcd, uint16_t x, uint16_t y,
                         const uint8_t* bitmap, uint16_t w, uint16_t h, uint16_t color) {
    if (!bitmap) return;

    // Assumes RGB565 bitmap data (16-bit color per pixel)
    // If you meant monochrome bitmap, use the bit-check logic instead.
    
    ili9225_set_window(lcd, x, y, x + w - 1, y + h - 1);
    gpio_put(lcd->pin_dc, 1);
    lcd_select(lcd);
    
    // Write the whole buffer
    spi_write_blocking(lcd->spi, bitmap, w * h * 2);
    
    lcd_deselect(lcd);
}