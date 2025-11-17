/**
 * @file ili9225.cpp
 * @author trung.la
 * @date November 12 2025
 * @brief Implementation file for ILI9225 LCD driver for Raspberry Pi Pico
 * 
 * Copyright (c) 2025 trung.la
 */

#include "ili9225.h"

#include <stdlib.h>

#include "hardware/gpio.h"
#include "log.h"

// Uncomment to enable detailed logging
// #define ILI9225_DEBUG_LOGGING

static void ili9225_write_command(ili9225_config_t* config, uint8_t cmd) {
    gpio_put(config->pin_dc, ILI9225_CMD_GPIO);
    gpio_put(config->pin_cs, ILI9225_CS_LOW);
    spi_write_blocking(config->spi, &cmd, 1);
    gpio_put(config->pin_cs, ILI9225_CS_HIGH);
}

static void ili9225_write_data16(ili9225_config_t* config, uint16_t data) {
    gpio_put(config->pin_dc, ILI9225_DATA_GPIO);
    uint8_t buf[2] = {data >> 8, data & 0xFF};
    gpio_put(config->pin_cs, ILI9225_CS_LOW);
    spi_write_blocking(config->spi, buf, 2);
    gpio_put(config->pin_cs, ILI9225_CS_HIGH);
}

static void ili9225_set_window(ili9225_config_t *config, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    ili9225_write_command(config, ILI9225_HORIZONTAL_WINDOW_ADDR1);
    ili9225_write_data16(config, x2);
    ili9225_write_command(config, ILI9225_HORIZONTAL_WINDOW_ADDR2);
    ili9225_write_data16(config, x1);
    ili9225_write_command(config, ILI9225_VERTICAL_WINDOW_ADDR1);
    ili9225_write_data16(config, y2);
    ili9225_write_command(config, ILI9225_VERTICAL_WINDOW_ADDR2);
    ili9225_write_data16(config, y1);
    ili9225_write_command(config, ILI9225_RAM_ADDR_SET1);
    ili9225_write_data16(config, x1);
    ili9225_write_command(config, ILI9225_RAM_ADDR_SET2);
    ili9225_write_data16(config, y1);
    ili9225_write_command(config, ILI9225_GRAM_DATA_REG);
}

void ili9225_init(ili9225_config_t* config, spi_inst_t* spi, uint pin_sck, uint pin_mosi, uint pin_miso,
                  uint pin_cs, uint pin_dc, uint pin_reset,
                  uint16_t width, uint16_t height, ili9225_rotation_t rotation) {
    if (!config) {
        LOG_ERROR("Config is NULL");
        return;
    }
    
    LOG_INFO("Initializing ILI9225 LCD: %dx%d", width, height);
    LOG_DEBUG("SPI pins - SCK:%d MOSI:%d MISO:%d", pin_sck, pin_mosi, pin_miso);
    LOG_DEBUG("Control pins - CS:%d DC:%d RST:%d", pin_cs, pin_dc, pin_reset);
    
    config->spi = spi;
    config->pin_sck = pin_sck;
    config->pin_mosi = pin_mosi;
    config->pin_miso = pin_miso;
    config->pin_cs = pin_cs;
    config->pin_dc = pin_dc;
    config->pin_reset = pin_reset;
    config->width = width;
    config->height = height;
    config->rotation = rotation;

    gpio_init(config->pin_cs);
    gpio_set_dir(config->pin_cs, GPIO_OUT);
    gpio_put(config->pin_cs, ILI9225_CS_HIGH);

    gpio_init(config->pin_dc);
    gpio_set_dir(config->pin_dc, GPIO_OUT);
    gpio_put(config->pin_dc, ILI9225_DATA_GPIO);
    
    gpio_init(config->pin_reset);
    gpio_set_dir(config->pin_reset, GPIO_OUT);
    gpio_put(config->pin_reset, 1);

    // Initialize SPI
    spi_init(config->spi, 10000 * 1000); // 10 MHz
    spi_set_format(config->spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(config->pin_sck, GPIO_FUNC_SPI);
    gpio_set_function(config->pin_mosi, GPIO_FUNC_SPI);
    gpio_set_function(config->pin_miso, GPIO_FUNC_SPI);
    LOG_DEBUG("SPI initialized at 10 MHz");

    // Reset
    LOG_DEBUG("Performing hardware reset");
    gpio_put(config->pin_reset, 0);
    sleep_ms(10);
    gpio_put(config->pin_reset, 1);
    sleep_ms(120);

    // Initialization sequence
    LOG_DEBUG("Starting ILI9225 register initialization");
    ili9225_write_command(config, ILI9225_DISP_CTRL1);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_DISP_CTRL2);
    ili9225_write_data16(config, 0x0808);
    ili9225_write_command(config, ILI9225_FRAME_CYCLE_CTRL);
    ili9225_write_data16(config, 0x1100);
    ili9225_write_command(config, ILI9225_RGB_DISP_IF_CTRL1);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_OSC_CTRL);
    ili9225_write_data16(config, 0x0D01);
    sleep_ms(10);
    ili9225_write_command(config, ILI9225_VCI_RECYCLING);
    ili9225_write_data16(config, 0x0020);
    ili9225_write_command(config, ILI9225_RAM_ADDR_SET1);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_RAM_ADDR_SET2);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_POWER_CTRL1);
    ili9225_write_data16(config, 0x0A00);
    ili9225_write_command(config, ILI9225_POWER_CTRL2);
    ili9225_write_data16(config, 0x1038);
    sleep_ms(10);
    ili9225_write_command(config, ILI9225_POWER_CTRL3);
    ili9225_write_data16(config, 0x1121);
    ili9225_write_command(config, ILI9225_POWER_CTRL4);
    ili9225_write_data16(config, 0x0066);
    ili9225_write_command(config, ILI9225_POWER_CTRL5);
    ili9225_write_data16(config, 0x5F00);
    ili9225_write_command(config, ILI9225_GATE_SCAN_CTRL);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_VERTICAL_SCROLL_CTRL1);
    ili9225_write_data16(config, 0x00DB);
    ili9225_write_command(config, ILI9225_VERTICAL_SCROLL_CTRL2);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_VERTICAL_SCROLL_CTRL3);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_PARTIAL_DRIVING_POS1);
    ili9225_write_data16(config, 0x00DB);
    ili9225_write_command(config, ILI9225_PARTIAL_DRIVING_POS2);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_HORIZONTAL_WINDOW_ADDR1);
    ili9225_write_data16(config, 0x00AF);
    ili9225_write_command(config, ILI9225_HORIZONTAL_WINDOW_ADDR2);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_VERTICAL_WINDOW_ADDR1);
    ili9225_write_data16(config, 0x00DB);
    ili9225_write_command(config, ILI9225_VERTICAL_WINDOW_ADDR2);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_GAMMA_CTRL1);
    ili9225_write_data16(config, 0x0000);
    ili9225_write_command(config, ILI9225_GAMMA_CTRL2);
    ili9225_write_data16(config, 0x0807);
    ili9225_write_command(config, ILI9225_GAMMA_CTRL3);
    ili9225_write_data16(config, 0x0A02);
    ili9225_write_command(config, ILI9225_GAMMA_CTRL4);
    ili9225_write_data16(config, 0x020A);
    ili9225_write_command(config, ILI9225_GAMMA_CTRL5);
    ili9225_write_data16(config, 0x0708);
    ili9225_write_command(config, ILI9225_GAMMA_CTRL6);
    ili9225_write_data16(config, 0x0205);
    ili9225_write_command(config, ILI9225_GAMMA_CTRL7);
    ili9225_write_data16(config, 0x0006);
    ili9225_write_command(config, ILI9225_GAMMA_CTRL8);
    ili9225_write_data16(config, 0x0502);
    ili9225_write_command(config, ILI9225_GAMMA_CTRL9);
    ili9225_write_data16(config, 0x0404);
    ili9225_write_command(config, ILI9225_GAMMA_CTRL10);
    ili9225_write_data16(config, 0x040B);
    ili9225_write_command(config, ILI9225_DISP_CTRL1);
    ili9225_write_data16(config, 0x0012);
    sleep_ms(10);
    ili9225_write_command(config, ILI9225_DISP_CTRL1);
    ili9225_write_data16(config, 0x0017);

    LOG_INFO("ILI9225 initialization complete");

    ili9225_set_rotation(config, ILI9225_PORTRAIT);
}

void ili9225_set_rotation(ili9225_config_t* config, ili9225_rotation_t rotation) {
    if (!config) {
        LOG_ERROR("set_rotation: config is NULL");
        return;
    }

    if (config->rotation == rotation) {
        return; // No change
    }

    uint16_t entry_mode = ENTRY_MODE_RGB; // default RGB

    config->rotation = rotation;
    switch (rotation) {
        case ILI9225_PORTRAIT:            // AM=0, ID=0b10
            entry_mode |= ENTRY_MODE_ID1; // ID1 = 1, ID0 = 0
            break;
        case ILI9225_LANDSCAPE:          // AM=1, ID=0b10
            entry_mode |= ENTRY_MODE_AM | ENTRY_MODE_ID1; // AM=1, ID1=1, ID0=0
            break;
        case ILI9225_PORTRAIT_REV:       // AM=0, ID=0b00
            // ID1=0, ID0=0
            break;
        case ILI9225_LANDSCAPE_REV:     // AM=1, ID=0b00
            entry_mode |= ENTRY_MODE_AM; // AM=1, ID1=0, ID0=0
            break;
        case ILI9225_PORTRAIT_MIRROR:    // AM=0, ID=0b11
            entry_mode |= ENTRY_MODE_ID1 | ENTRY_MODE_ID0; // ID1=1, ID0=1
            break;
        case ILI9225_LANDSCAPE_MIRROR:  // AM=1, ID=0b11
            entry_mode |= ENTRY_MODE_AM | ENTRY_MODE_ID1 | ENTRY_MODE_ID0; // AM=1, ID1=1, ID0=1
            break;
        case ILI9225_PORTRAIT_REV_MIRROR:// AM=0, ID=0b01
            entry_mode |= ENTRY_MODE_ID0; // ID1=0, ID0=1
            break;
        case ILI9225_LANDSCAPE_REV_MIRROR:// AM=1, ID=0b01
            entry_mode |= ENTRY_MODE_AM | ENTRY_MODE_ID0; // AM=1, ID1=0, ID0=1
            break;
        default:
            break;
    }
    ili9225_write_command(config, ILI9225_ENTRY_MODE);
    ili9225_write_data16(config, entry_mode);

    ili9225_refresh_display(config);
}

void ili9225_fill_screen(ili9225_config_t* config, uint16_t color) {
    ili9225_fill_rect(config, 0, 0, config->width, config->height, color);
}

void ili9225_draw_pixel(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t color) {
    if (!config || x >= config->width || y >= config->height) {
#ifdef ILI9225_DEBUG_LOGGING
        if (!config) {
            LOG_ERROR("draw_pixel: config is NULL");
        } else {
            LOG_TRACE("draw_pixel: out of bounds (%d, %d) - display is %dx%d", 
                     x, y, config->width, config->height);
        }
#endif
        return;
    }
    ili9225_set_window(config, x, y, x, y);
    ili9225_write_data16(config, color);
}

void ili9225_draw_line(ili9225_config_t* config, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    uint16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        uint16_t temp = x0;
        x0 = y0;
        y0 = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
    }

    if (x0 > x1) {
        uint16_t temp = x0;
        x0 = x1;
        x1 = temp;
        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    uint16_t dx = x1 - x0;
    uint16_t dy = abs(y1 - y0);
    uint16_t err = dx / 2;
    int16_t ystep = (y0 < y1) ? 1 : -1;
    for (; x0 <= x1; x0++) {
        if (steep) {
            ili9225_draw_pixel(config, y0, x0, color);
        } else {
            ili9225_draw_pixel(config, x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void ili9225_draw_rect(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    ili9225_draw_line(config, x, y, x + w, y, color);
    ili9225_draw_line(config, x, y, x, y + h, color);
    ili9225_draw_line(config, x + w, y, x + w, y + h, color);
    ili9225_draw_line(config, x, y + h, x + w, y + h, color);
}

void ili9225_fill_rect(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (!config) {
        LOG_ERROR("fill_rect: config is NULL");
        return;
    }

    if (x >= config->width || y >= config->height) {
        LOG_TRACE("fill_rect: starting point out of bounds (%d, %d) - display is %dx%d", 
                 x, y, config->width, config->height);
        return;
    }

    uint16_t x_end = (x + w - 1 >= config->width) ? (config->width - 1) : (x + w - 1);
    uint16_t y_end = (y + h - 1 >= config->height) ? (config->height - 1) : (y + h - 1);
    
    ili9225_set_window(config, x, y, x_end, y_end);
    gpio_put(config->pin_dc, ILI9225_DATA_GPIO);
    gpio_put(config->pin_cs, ILI9225_CS_LOW);

    uint32_t pixel_count = (uint32_t)(x_end - x + 1) * (y_end - y + 1);
    for (uint32_t i = 0; i < pixel_count; ++i) {
        uint8_t buf[2] = {color >> 8, color & 0xFF};
        spi_write_blocking(config->spi, buf, 2);
    }
    gpio_put(config->pin_cs, ILI9225_CS_HIGH);
}

void ili9225_draw_circle(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
    if (!config) {
        LOG_ERROR("draw_circle: config is NULL");
        return;
    }
    if (r == 0 || x < r || y < r || x + r >= config->width || y + r >= config->height
        || x - r >= config->width || y - r >= config->height) {
        LOG_TRACE("draw_circle: invalid parameters (x:%d, y:%d, r:%d) for display %dx%d",
            x, y, r, config->width, config->height);
        return;
    }
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x0 = 0;
    int16_t y0 = r;

    ili9225_draw_pixel(config, x, y + r, color);
    ili9225_draw_pixel(config, x, y - r, color);
    ili9225_draw_pixel(config, x + r, y, color);
    ili9225_draw_pixel(config, x - r, y, color);
    while (x0 < y0) {
        if (f >= 0) {
            y0--;
            ddF_y += 2;
            f += ddF_y;
        }
        x0++;
        ddF_x += 2;
        f += ddF_x;

        ili9225_draw_pixel(config, x + x0, y + y0, color);
        ili9225_draw_pixel(config, x - x0, y + y0, color);
        ili9225_draw_pixel(config, x + x0, y - y0, color);
        ili9225_draw_pixel(config, x - x0, y - y0, color);
        ili9225_draw_pixel(config, x + y0, y + x0, color);
        ili9225_draw_pixel(config, x - y0, y + x0, color);
        ili9225_draw_pixel(config, x + y0, y - x0, color);
        ili9225_draw_pixel(config, x - y0, y - x0, color);
    }
}

void ili9225_fill_circle(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
    if (!config) {
        LOG_ERROR("fill_circle: config is NULL");
        return;
    }
    if (r == 0) return;
    
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x0 = 0;
    int16_t y0 = r;

    // Draw vertical line through center
    ili9225_draw_line(config, x, y - r, x, y + r, color);
    
    while (x0 < y0) {
        if (f >= 0) {
            y0--;
            ddF_y += 2;
            f += ddF_y;
        }
        x0++;
        ddF_x += 2;
        f += ddF_x;

        // Draw horizontal lines to fill the circle
        ili9225_draw_line(config, x - x0, y + y0, x + x0, y + y0, color);
        ili9225_draw_line(config, x - x0, y - y0, x + x0, y - y0, color);
        ili9225_draw_line(config, x - y0, y + x0, x + y0, y + x0, color);
        ili9225_draw_line(config, x - y0, y - x0, x + y0, y - x0, color);
    }
}

void ili9225_draw_triangle(ili9225_config_t* config, uint16_t x0, uint16_t y0,
                           uint16_t x1, uint16_t y1,
                           uint16_t x2, uint16_t y2,
                           uint16_t color) {
}

void ili9225_fill_triangle(ili9225_config_t* config, uint16_t x0, uint16_t y0,
                           uint16_t x1, uint16_t y1,
                           uint16_t x2, uint16_t y2,
                           uint16_t color) {
    // TODO: Implement fill triangle function for ILI9225
}

void ili9225_draw_text(ili9225_config_t* config, uint16_t x, uint16_t y, const char* text, const font_t *font, uint16_t color) {
    if (!config || !text || !font) {
#ifdef ILI9225_DEBUG_LOGGING
        if (!config) {
            LOG_ERROR("draw_text: config is NULL");
        }
        if (!text) {
            LOG_ERROR("draw_text: text is NULL");
        }
        if (!font) {
            LOG_ERROR("draw_text: font is NULL");
        }
#endif
        return;
    }

    uint16_t current_x = x;
    
    while (*text) {
        ili9225_draw_char(config, current_x, y, *text, font, color);
        current_x += font->width + 1; // +1 for spacing between characters
        text++;
    }
}

void ili9225_draw_char(ili9225_config_t *config, uint16_t x, uint16_t y, char c, const font_t *font, uint16_t color) {
    if (!config || !font) {
#ifdef ILI9225_DEBUG_LOGGING
        if (!config) {
            LOG_ERROR("draw_char: config is NULL");
        }
        if (!font) {
            LOG_ERROR("draw_char: font is NULL");
        }
#endif
        return;
    }

    // Check if character is in font range
    if (c < font->first_char || c > font->last_char) {
        return;
    }
    
    // Calculate character index in font data
    uint16_t index = (c - font->first_char) * font->width;
    
    // Draw each pixel of the character
    for (uint8_t row = 0; row < font->height; row++) {
        for (uint8_t col = 0; col < font->width; col++) {
            // Get the pixel value from font data
            uint8_t pixel_data = font->data[index + col];
            uint8_t pixel_value = (pixel_data >> (7 - row)) & 0x01;
            
            if (pixel_value) {
                ili9225_draw_pixel(config, x + col, y + row, color);
            }
        }
    }
}

void ili9225_draw_bitmap(ili9225_config_t* config, uint16_t x, uint16_t y,
                     const uint8_t* bitmap, uint16_t w, uint16_t h, uint16_t color) {
    if (!config || !bitmap) {
#ifdef ILI9225_DEBUG_LOGGING
        if (!config) {
            LOG_ERROR("draw_bitmap: config is NULL");
        }
        if (!bitmap) {
            LOG_ERROR("draw_bitmap: bitmap is NULL");
        }
#endif
        return;
    }

    uint16_t byte_width = (w + 7) / 8;
    for (uint16_t j = 0; j < h; j++) {
        for (uint16_t i = 0; i < w; i++) {
            if (bitmap[j * byte_width + i / 8] & (128 >> (i & 7))) {
                ili9225_draw_pixel(config, x + i, y + j, color);
            }
        }
    }
}

void ili9225_set_color_order(ili9225_config_t* config, bool is_rgb) {
    static bool current_is_rgb = true;

    if (current_is_rgb == is_rgb) {
        return; // No change needed
    }

    if (!config) {
        LOG_ERROR("set_color_order: config is NULL");
        return;
    }

    uint16_t entry_mode = is_rgb ? ENTRY_MODE_RGB : ENTRY_MODE_BGR;

    switch (config->rotation) {
        case ILI9225_PORTRAIT:
            entry_mode |= ENTRY_MODE_ID1; // ID1 = 1, ID0 = 0
            break;
        case ILI9225_LANDSCAPE:
            entry_mode |= ENTRY_MODE_AM | ENTRY_MODE_ID1; // AM=1, ID1=1, ID0=0
            break;
        case ILI9225_PORTRAIT_REV:
            // ID1=0, ID0=0
            break;
        case ILI9225_LANDSCAPE_REV:
            entry_mode |= ENTRY_MODE_AM; // AM=1, ID1=0, ID0=0
            break;
        case ILI9225_PORTRAIT_MIRROR:
            entry_mode |= ENTRY_MODE_ID1 | ENTRY_MODE_ID0; // ID1=1, ID0=1
            break;
        case ILI9225_LANDSCAPE_MIRROR:
            entry_mode |= ENTRY_MODE_AM | ENTRY_MODE_ID1 | ENTRY_MODE_ID0; // AM=1, ID1=1, ID0=1
            break;
        case ILI9225_PORTRAIT_REV_MIRROR:
            entry_mode |= ENTRY_MODE_ID0; // ID1=0, ID0=1
            break;
        case ILI9225_LANDSCAPE_REV_MIRROR:
            entry_mode |= ENTRY_MODE_AM | ENTRY_MODE_ID0; // AM=1, ID1=0, ID0=1
            break;
        default:
            break;
    }

    ili9225_write_command(config, ILI9225_ENTRY_MODE);
    ili9225_write_data16(config, entry_mode);

    current_is_rgb = is_rgb;

    ili9225_refresh_display(config);
}

void ili9225_refresh_display(ili9225_config_t* config) {
    if (!config) {
        LOG_ERROR("refresh_display: config is NULL");
        return;
    }

    ili9225_fill_screen(config, COLOR_BLACK);
}