/**
 * @file ili9225_mock.h
 * @author trung.la
 * @date November 13 2025
 * @brief Mock implementation of Pico SDK hardware functions for simulator
 * 
 * This file provides mock implementations of the Raspberry Pi Pico SDK
 * functions so that the real ili9225.c code can run in the simulator
 * without modification.
 * 
 * Copyright (c) 2025 trung.la
 */

#ifndef ILI9225_MOCK_H
#define ILI9225_MOCK_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Mock Pico SDK headers
#ifndef PICO_STDLIB_H
#define PICO_STDLIB_H

// Mock types and constants
typedef unsigned int uint;

// Mock sleep function
void sleep_ms(uint32_t ms);

#endif // PICO_STDLIB_H

// Mock hardware/spi.h
#ifndef _HARDWARE_SPI_H
#define _HARDWARE_SPI_H

// Mock SPI instance type
typedef struct {
    int mock_id;
} spi_inst_t;

// Mock SPI instances
extern spi_inst_t spi0_inst;
extern spi_inst_t spi1_inst;
#define spi0 (&spi0_inst)
#define spi1 (&spi1_inst)

// Mock SPI constants
#define SPI_CPOL_0 0
#define SPI_CPHA_0 0
#define SPI_MSB_FIRST 0

// Mock GPIO functions
typedef unsigned int uint;

#define GPIO_OUT 1
#define GPIO_IN 0
#define SPI_MSB_FIRST 0

int spi_init(spi_inst_t* spi, uint baudrate);
void spi_set_format(spi_inst_t* spi, uint data_bits, uint cpol, uint cpha, uint order);
int spi_write_blocking(spi_inst_t* spi, const uint8_t* src, size_t len);

#endif // _HARDWARE_SPI_H

// Mock hardware/gpio.h
#ifndef _HARDWARE_GPIO_H
#define _HARDWARE_GPIO_H

// Mock GPIO functions
#define GPIO_OUT 1
#define GPIO_IN 0
#define GPIO_FUNC_SPI 1

void gpio_init(uint pin);
void gpio_set_dir(uint pin, int dir);
void gpio_put(uint pin, int value);
void gpio_set_function(uint pin, int func);

#endif // _HARDWARE_GPIO_H

// Simulator bridge functions
void sim_mock_init(const char* host, int port);
void sim_mock_close(void);
void sim_mock_flush_framebuffer(void);

#endif // ILI9225_MOCK_H
