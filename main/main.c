#include "include/main.h"

static const char *TAG = "HTTP_CLIENT";

void app_main(void)
{
	int wifi_flag = wifi_initialize_station();
    char pic_data[50] = {0};
    xQueueHandle picture_rcv_queue = xQueueCreate(1, sizeof(char));
	esp_mqtt_client_handle_t mqtt = mqtt_app_start(&picture_rcv_queue);
    size_t jpg_size = 0;
    jpg_data_t jpg_image = {0};
    esp_netif_init();
    // esp_event_loop_create_default();
    // xTaskCreate(&http_test_task, "http_test_task", 1024, NULL, 5, NULL);

    char output_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};   // Buffer to store response of http request
    int content_length = 0;
    esp_http_client_config_t config = {0};
        // .url = "http://"CONFIG_EXAMPLE_HTTP_ENDPOINT"/get",
    
    esp_http_client_handle_t client = esp_http_client_init(&config);


    // Create a buffer for the JPG in psram
    jpg_image.buf = (uint8_t *) heap_caps_malloc(100000, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (init_camera() == ESP_OK)
    {
        // while (1)
        // {
        //     if(xQueueReceive(picture_rcv_queue, &message, 0) == pdTRUE)
        //     {
            // Tomar foto cuando recibo un mensaje
                if (take_picture(jpg_image.buf, &jpg_size) == ESP_OK){
                    // sprintf(pic_data, "%d-%d-%d-%d-%d-%d", jpg_image.buf[0], jpg_image.buf[1],
                    //                                 jpg_image.buf[2], jpg_image.buf[3],
                    //                                 jpg_image.buf[4], jpg_image.buf[5]);
                    // esp_mqtt_client_publish(mqtt, "/topic/cam", pic_data, 0, 0, 0);

                    // POST Request
                    const char *post_data = "{\"field1\":\"value1\"}";
                    esp_http_client_set_url(client, "http://"CONFIG_EXAMPLE_HTTP_ENDPOINT"/post");
                    esp_http_client_set_method(client, HTTP_METHOD_POST);
                    esp_http_client_set_header(client, "Content-Type", "application/json");
                    esp_err_t err = esp_http_client_open(client, strlen(post_data));
                    esp_mqtt_client_publish(mqtt, "/topic/cam", "Failed to open HTTP connection", 0, 0, 0);
                    if (err != ESP_OK) {
                        // ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
                        esp_mqtt_client_publish(mqtt, "/topic/cam", "Failed to open HTTP connection", 0, 0, 0);
                    } else {
                        int wlen = esp_http_client_write(client, post_data, strlen(post_data));
                        if (wlen < 0)
                            esp_mqtt_client_publish(mqtt, "/topic/cam", "Write failed", 0, 0, 0);
                    }
                    //     content_length = esp_http_client_fetch_headers(client);
                    //     if (content_length < 0) {
                    //         ESP_LOGE(TAG, "HTTP client fetch headers failed");
                    //     } else {
                    //         int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
                    //         if (data_read >= 0) {
                    //             ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %d",
                    //             esp_http_client_get_status_code(client),
                    //             esp_http_client_get_content_length(client));
                    //             ESP_LOG_BUFFER_HEX(TAG, output_buffer, strlen(output_buffer));
                    //         } else {
                    //             ESP_LOGE(TAG, "Failed to read response");
                    //         }
                    //     }
                    // }
                    // esp_http_client_cleanup(client);
                }
                else
                    esp_mqtt_client_publish(mqtt, "/topic/cam", "FAIL", 0, 0, 0);
                // use pic->buf to access the image

                // Convert the RAW image into JPG
                // The parameter "31" is the JPG quality. Higher is better.
                // fmt2jpg(pic->buf, pic->len, pic->width, pic->height, pic->format, 31, &jpg_buf, &jpg_size);
                // free_picture(&pic);
            // }
        	// vTaskDelay(50);
        // }
    }
    else
        esp_mqtt_client_publish(mqtt, "/topic/cam", "Camera Init Failed", 0, 0, 0);

}

// static void http_test_task(void *pvParameters)
// {

//     char output_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};   // Buffer to store response of http request
//     int content_length = 0;
//     esp_http_client_config_t config = {0};
//         // .url = "http://"CONFIG_EXAMPLE_HTTP_ENDPOINT"/get",
    
//     esp_http_client_handle_t client = esp_http_client_init(&config);

//     // POST Request
//     const char *post_data = "{\"field1\":\"value1\"}";
//     esp_http_client_set_url(client, "http://"CONFIG_EXAMPLE_HTTP_ENDPOINT"/post");
//     esp_http_client_set_method(client, HTTP_METHOD_POST);
//     esp_http_client_set_header(client, "Content-Type", "application/json");
//     esp_err_t err = esp_http_client_open(client, strlen(post_data));
//     if (err != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
//     } else {
//         int wlen = esp_http_client_write(client, post_data, strlen(post_data));
//         if (wlen < 0) {
//             ESP_LOGE(TAG, "Write failed");
//         }
//         content_length = esp_http_client_fetch_headers(client);
//         if (content_length < 0) {
//             ESP_LOGE(TAG, "HTTP client fetch headers failed");
//         } else {
//             int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
//             if (data_read >= 0) {
//                 ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %d",
//                 esp_http_client_get_status_code(client),
//                 esp_http_client_get_content_length(client));
//                 ESP_LOG_BUFFER_HEX(TAG, output_buffer, strlen(output_buffer));
//             } else {
//                 ESP_LOGE(TAG, "Failed to read response");
//             }
//         }
//     }
//     esp_http_client_cleanup(client);
// }