#include "client_mqtt.h"

static const char *TAG = "mqtt_client";
esp_mqtt_client_handle_t client = NULL;

esp_mqtt_client_handle_t mqtt_app_start(xQueueHandle* receive_queue)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .host = CONFIG_BROKER_HOST,
        .port = CONFIG_BROKER_PORT,
        .client_id = CONFIG_ID,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, (void*)receive_queue));
    ESP_ERROR_CHECK(esp_mqtt_client_start(client));

    return client;
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    int msg_id = 0;
    xQueueHandle *receive_queue = (xQueueHandle*)handler_args;

    switch ((esp_mqtt_event_id_t)event_id)
    {
        case MQTT_EVENT_CONNECTED:
            if ((msg_id = esp_mqtt_client_subscribe(client, "/topic/capture", 0)) == ESP_FAIL)
                ESP_LOGE(TAG, "error in subscribe topic, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGE(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGE(TAG, "MQTT_EVENT_PUBLISHED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGE(TAG, "MQTT_EVENT_SUBSCRIBED");
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGE(TAG, "MQTT_EVENT_DATA");
            if (xQueueSend(*receive_queue, (void*)"1", 0) != pdTRUE)
                ESP_LOGE(TAG, "error in send motor values");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}