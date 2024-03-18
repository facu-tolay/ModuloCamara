#include "include/main.h"

int take_picture(esp_mqtt_client_handle_t mqtt);

void app_main(void)
{
	if (wifi_initialize_station() != ESP_OK)
        exit(1);
    char pic_data = '0';
    xQueueHandle picture_rcv_queue = xQueueCreate(1, sizeof(char));
    esp_mqtt_client_handle_t mqtt = mqtt_app_start(&picture_rcv_queue);
    vTaskDelay(50);

    if (init_camera() == ESP_OK)
    {
        while (1)
        {
            if(xQueueReceive(picture_rcv_queue, &pic_data, 100) == pdTRUE)
            {
            // Tomar foto cuando recibo un mensaje
                take_picture(mqtt);
        	    // vTaskDelay(50);
            }
            vTaskDelay(50);
        }
    }
    else
        printf("Camera Init Failed");
}

int take_picture(esp_mqtt_client_handle_t mqtt)
{
    camera_fb_t *pic = esp_camera_fb_get();
    if (esp_mqtt_client_publish(mqtt, "/topic/upload", (const char *)pic->buf, pic->len, 0, 0) == ESP_OK)
        ESP_LOGI("esp32-cam", "Foto enviada - %zu Bytes", pic->len);
    // vTaskDelay(50);
    esp_camera_fb_return(pic);

    return 0;
}