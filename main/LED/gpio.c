/*
 * gpio.c
 *
 *  Created on: Jun 28, 2024
 *      Author: chutc
 */
#include "gpio.h"

void configure_led(void)
{
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}
