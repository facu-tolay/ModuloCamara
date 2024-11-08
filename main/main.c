#include "include/main.h"
#define MQTT_EVENT_CONNECTED 1

void take_picture(esp_mqtt_client_handle_t mqtt);
static const char *TAG = "esp32-cam";

void app_main(void)
{
    esp_mqtt_client_handle_t mqttc = NULL;

    vTaskDelay(1000/portTICK_PERIOD_MS);

    ESP_ERROR_CHECK(wifi_initialize_station());
    mqtt_app_start(&mqttc);
    ESP_ERROR_CHECK(init_camera());

    while (1)
    {
        // Tomar foto cada 500ms
        vTaskDelay(100/portTICK_PERIOD_MS);
        take_picture(mqttc);
    }
    exit(EXIT_FAILURE);
}

void take_picture(esp_mqtt_client_handle_t mqtt)
{
    camera_fb_t *pic = esp_camera_fb_get();
    esp_camera_fb_return(pic);
    vTaskDelay(400/portTICK_PERIOD_MS);

    if (esp_mqtt_client_publish(mqtt, "/topic/upload", (const char *)pic->buf, pic->len, 0, 0) == ESP_OK)
    {
        ESP_LOGI(TAG, "Foto enviada - %zu Bytes", pic->len);
    }
}