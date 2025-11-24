/**
 * Mock pico/stdlib.h for simulator
 */
#ifndef PICO_STDLIB_H
#define PICO_STDLIB_H

#include <stdint.h>
#include <stdbool.h>

typedef unsigned int uint;

void sleep_ms(uint32_t ms);

#endif // PICO_STDLIB_H
