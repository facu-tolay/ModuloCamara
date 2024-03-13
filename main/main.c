#include "include/main.h"

int take_picture(esp_mqtt_client_handle_t mqtt);

void app_main(void)
{
	int wifi_flag = wifi_initialize_station();
    char pic_data = '0';
    xQueueHandle picture_rcv_queue = xQueueCreate(1, sizeof(char));
	esp_mqtt_client_handle_t mqtt = mqtt_app_start(&picture_rcv_queue);

    if (init_camera() == ESP_OK)
    {
        while (1)
        {
            if(xQueueReceive(picture_rcv_queue, &pic_data, 1000) == pdTRUE)
            {
            // Tomar foto cuando recibo un mensaje
                take_picture(mqtt);
        	    // vTaskDelay(50);
            }
            // vTaskDelay(100);
        }
    }
    else
        printf("Camera Init Failed");
}

int take_picture(esp_mqtt_client_handle_t mqtt)
{
    camera_fb_t *pic = esp_camera_fb_get();
    esp_mqtt_client_publish(mqtt, "/topic/upload", (const char *)pic->buf, pic->len, 0, 0);
    esp_camera_fb_return(pic);

    return 0;
}