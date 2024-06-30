/*
 * mqtt.h
 *
 *  Created on: Jun 26, 2024
 *      Author: chutc
 */

#ifndef MAIN_MQTT_MQTT_H_
#define MAIN_MQTT_MQTT_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

void mqtt_app_start(void);

#endif /* MAIN_MQTT_MQTT_H_ */
