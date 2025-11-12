/**
 * @file ili9225.cpp
 * @author trung.la
 * @date November 12 2025
 * @brief Implementation file for ILI9225 LCD driver for Raspberry Pi Pico
 * 
 * Copyright (c) 2025 trung.la
 */

#include "ili9225.h"
#include "hardware/gpio.h"
#include "terminal6x8.h"

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

void ili9225_init(ili9225_config_t* config, spi_inst_t* spi, uint pin_sck, uint pin_mosi, uint pin_miso,
                  uint pin_cs, uint pin_dc, uint pin_reset,
                  uint16_t width, uint16_t height, ili9225_rotation_t rotation) {
    if (!config) return;
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

    // Reset
    gpio_put(config->pin_reset, 0);
    sleep_ms(10);
    gpio_put(config->pin_reset, 1);
    sleep_ms(120);

    // Initialization sequence
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

    ili9225_set_rotation(config, ILI9225_PORTRAIT);
}

void ili9225_set_rotation(ili9225_config_t* config, ili9225_rotation_t rotation) {
    config->rotation = rotation;
    ili9225_write_command(config, ILI9225_ENTRY_MODE);
    switch (rotation) {
        case ILI9225_PORTRAIT:
            ili9225_write_data16(config, ILI9225_ENTRY_MODE_PORTRAIT);
            break;
        case ILI9225_LANDSCAPE:
            ili9225_write_data16(config, ILI9225_ENTRY_MODE_LANDSCAPE);
            break;
        case ILI9225_PORTRAIT_REV:
            ili9225_write_data16(config, ILI9225_ENTRY_MODE_PORTRAIT_REV);
            break;
        case ILI9225_LANDSCAPE_REV:
            ili9225_write_data16(config, ILI9225_ENTRY_MODE_LANDSCAPE_REV);
            break;
        default:
            ili9225_write_data16(config, ILI9225_ENTRY_MODE_PORTRAIT);
            break;
    }
}

void ili9225_fill_screen(ili9225_config_t* config, uint16_t color) {
    // TODO: Implement fill screen function for ILI9225
}

void ili9225_draw_pixel(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t color) {
    // TODO: Implement draw pixel function for ILI9225
}

void ili9225_draw_line(ili9225_config_t* config, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    // TODO: Implement draw line function for ILI9225
}

void ili9225_draw_rect(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // TODO: Implement draw rectangle function for ILI9225
}

void ili9225_fill_rect(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // TODO: Implement fill rectangle function for ILI9225
}

void ili9225_draw_circle(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
    // TODO: Implement draw circle function for ILI9225
}

void ili9225_fill_circle(ili9225_config_t* config, uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
    // TODO: Implement fill circle function for ILI9225
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

void ili9225_draw_text(ili9225_config_t* config, uint16_t x, uint16_t y, const char* text, uint16_t color, uint8_t size) {
    // TODO: Implement draw text function for ILI9225
}

void ili9225_draw_char(ili9225_config_t* config, uint16_t x, uint16_t y, char c, uint16_t color, uint8_t size) {
    // TODO: Implement draw character function for ILI9225
}

void ili9225_draw_bitmap(ili9225_config_t* config, uint16_t x, uint16_t y,
                     const uint8_t* bitmap, uint16_t w, uint16_t h) {
    // TODO: Implement draw bitmap function for ILI9225
}
