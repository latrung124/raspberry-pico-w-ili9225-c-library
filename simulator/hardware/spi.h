/**
 * Mock hardware/spi.h for simulator
 */
#ifndef _HARDWARE_SPI_H
#define _HARDWARE_SPI_H

#include <stdint.h>
#include <stddef.h>

typedef unsigned int uint;

typedef struct {
    int mock_id;
} spi_inst_t;

extern spi_inst_t spi0_inst;
extern spi_inst_t spi1_inst;
#define spi0 (&spi0_inst)
#define spi1 (&spi1_inst)

#define SPI_CPOL_0 0
#define SPI_CPHA_0 0
#define SPI_MSB_FIRST 0

int spi_init(spi_inst_t* spi, uint baudrate);
void spi_set_format(spi_inst_t* spi, uint data_bits, uint cpol, uint cpha, uint order);
int spi_write_blocking(spi_inst_t* spi, const uint8_t* src, size_t len);

#endif // _HARDWARE_SPI_H
