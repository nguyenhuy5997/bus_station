#include <stdio.h>
#include "freertos/ringbuf.h"
#include "nmea/nmea_parser.h"
#include "WiFi/wifi.h"
#include "mqtt/mqtt.h"
#include "position/position.h"
#include "position/mp3_uart.h"
#define TAG_GNSS "GNSS"
#define TAG_WIFI "WiFi"

extern esp_mqtt_client_handle_t client_emqx;
extern RingbufHandle_t client_thingsboard;
extern RingbufHandle_t gps_buf_handle;

static void gps_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    gps_t *gps = NULL;
    switch (event_id) {
    case GPS_UPDATE:
        gps = (gps_t *)event_data;
        /* print information parsed from GPS statements */
//        ESP_LOGI(TAG_GNSS, "%d/%d/%d %d:%d:%d => \r\n"
//                 "\t\t\t\t\t\tlatitude   = %.05f\r\n"
//                 "\t\t\t\t\t\tlongitude = %.05f\r\n"
//                 "\t\t\t\t\t\taltitude   = %.02fm\r\n"
//                 "\t\t\t\t\t\tspeed      = %fm/s",
//                 gps->date.year + YEAR_BASE, gps->date.month, gps->date.day,
//                 gps->tim.hour + TIME_ZONE, gps->tim.minute, gps->tim.second,
//                 gps->latitude, gps->longitude, gps->altitude, gps->speed);
        xRingbufferSendFromISR(gps_buf_handle, gps, sizeof(gps_t), (BaseType_t *)10);
        break;
    case GPS_UNKNOWN:
        ESP_LOGW(TAG_GNSS, "Unknown statement:%s", (char *)event_data);
        break;
    default:
        break;
    }
}

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_init_sta();
    /* NMEA parser configuration */
    nmea_parser_config_t config = NMEA_PARSER_CONFIG_DEFAULT();
    /* init NMEA parser library */
    nmea_parser_handle_t nmea_hdl = nmea_parser_init(&config);
    /* register event handler for NMEA parser library */
    nmea_parser_add_handler(nmea_hdl, gps_event_handler, NULL);
    gps_buf_handle = xRingbufferCreate(1024, RINGBUF_TYPE_NOSPLIT);
    init_uart_mp3();
    xTaskCreate(bus_router, "bus router", 8192, NULL, 3, NULL);
}
