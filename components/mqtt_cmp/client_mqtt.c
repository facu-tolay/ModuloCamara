#include "client_mqtt.h"

static const char *TAG = "mqtt_client";
esp_mqtt_client_handle_t client = NULL;
char topic_robot_id[MQQT_TOPIC_LEN] = {0};
char topic_robot_road[MQQT_TOPIC_LEN] = {0};
char topic_robot_ok[MQQT_TOPIC_LEN] = {0};

esp_mqtt_client_handle_t mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .host = BROKER_HOST,
        .port = BROKER_PORT,
        .client_id = ID,
        .keepalive = 20,
        .disable_clean_session = true,
        .lwt_topic = "/topic/lwt",
        .lwt_msg = ID,
        .lwt_msg_len = 5
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL));
    ESP_ERROR_CHECK(esp_mqtt_client_start(client));

    return client;
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    int msg_id = 0;

    switch ((esp_mqtt_event_id_t)event_id)
    {
        case MQTT_EVENT_CONNECTED:
            esp_mqtt_client_publish(client, "/topic/cam", "MQTT_EVENT_CONNECTED", 0, 0, 0);
            break;
        case MQTT_EVENT_DISCONNECTED:
            esp_mqtt_client_publish(client, "/topic/cam", "MQTT_EVENT_DISCONNECTED", 0, 0, 0);
            break;
        // case MQTT_EVENT_PUBLISHED:
        //     esp_mqtt_client_publish(client, "/topic/cam", "MQTT_EVENT_PUBLISHED", 0, 0, 0);
        //     break;
        case MQTT_EVENT_SUBSCRIBED:
            esp_mqtt_client_publish(client, "/topic/cam", "MQTT_EVENT_SUBSCRIBED", 0, 0, 0);
            break;
        case MQTT_EVENT_DATA:
            esp_mqtt_client_publish(client, "/topic/cam", "MQTT_EVENT_DATA", 0, 0, 0);
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}