/*
 * wifi.h
 *
 *  Created on: Jun 26, 2024
 *      Author: chutc
 */

#ifndef MAIN_WIFI_WIFI_H_
#define MAIN_WIFI_WIFI_H_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

void wifi_init_sta(void);



#endif /* MAIN_WIFI_WIFI_H_ */
