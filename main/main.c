#include "include/main.h"

int take_picture(esp_mqtt_client_handle_t mqtt);

void app_main(void)
{
	if (wifi_initialize_station() != ESP_OK)
        exit(1);
    esp_mqtt_client_handle_t mqtt = mqtt_app_start();
    vTaskDelay(100/portTICK_PERIOD_MS);

    if (init_camera() == ESP_OK)
    {
        while (1)
        {
            // Tomar foto cada 500ms
            vTaskDelay(100/portTICK_PERIOD_MS);
            take_picture(mqtt);
        }
    }
    else
        printf("Camera Init Failed");
}

int take_picture(esp_mqtt_client_handle_t mqtt)
{
    camera_fb_t *pic = esp_camera_fb_get();
    esp_camera_fb_return(pic);
    vTaskDelay(400/portTICK_PERIOD_MS);
    if (esp_mqtt_client_publish(mqtt, "/topic/upload", (const char *)pic->buf, pic->len, 0, 0) == ESP_OK)
        ESP_LOGI("esp32-cam", "Foto enviada - %zu Bytes", pic->len);

    return 0;
}