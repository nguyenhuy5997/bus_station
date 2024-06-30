/*
 * gpio.h
 *
 *  Created on: Jun 28, 2024
 *      Author: chutc
 */

#ifndef MAIN_LED_GPIO_H_
#define MAIN_LED_GPIO_H_

#include "driver/gpio.h"
#include "sdkconfig.h"

#define BLINK_GPIO CONFIG_BLINK_GPIO

void configure_led(void);



#endif /* MAIN_LED_GPIO_H_ */
