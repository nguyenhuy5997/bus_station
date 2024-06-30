/*
 * mqtt.c
 *
 *  Created on: Jun 26, 2024
 *      Author: chutc
 */
#include "mqtt.h"

static const char *TAG = "MQTT_EXAMPLE";
esp_mqtt_client_handle_t client_thingsboard;
esp_mqtt_client_handle_t client_emqx;
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;

    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
		.credentials.username = "sih1m1f3gkvxs5h5vv96",
        .credentials.client_id = "sih1m1f3gkvxs5h5vv96",
		.credentials.authentication.password = "sih1m1f3gkvxs5h5vv96",
    };
    esp_mqtt_client_config_t mqtt_cfg_2 = {
    	.broker.address.uri	 ="mqtt://broker.emqx.io:1883"
    };
    client_thingsboard = esp_mqtt_client_init(&mqtt_cfg);
    client_emqx = esp_mqtt_client_init(&mqtt_cfg_2);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client_thingsboard, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_register_event(client_emqx, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client_thingsboard);
    esp_mqtt_client_start(client_emqx);
}

