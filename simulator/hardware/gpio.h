/**
 * Mock hardware/gpio.h for simulator
 */
#ifndef _HARDWARE_GPIO_H
#define _HARDWARE_GPIO_H

typedef unsigned int uint;

#define GPIO_OUT 1
#define GPIO_IN 0
#define GPIO_FUNC_SPI 1

void gpio_init(uint pin);
void gpio_set_dir(uint pin, int dir);
void gpio_put(uint pin, int value);
void gpio_set_function(uint pin, int func);

#endif // _HARDWARE_GPIO_H
