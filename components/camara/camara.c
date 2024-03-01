/**
 * This example takes a picture every 5s and print its size on serial monitor.
 */

// =============================== SETUP ======================================

// 1. Board setup (Uncomment):
// #define BOARD_WROVER_KIT
// #define BOARD_ESP32CAM_AITHINKER

/**
 * 2. Kconfig setup
 *
 * If you have a Kconfig file, copy the content from
 *  https://github.com/espressif/esp32-camera/blob/master/Kconfig into it.
 * In case you haven't, copy and paste this Kconfig file inside the src directory.
 * This Kconfig file has definitions that allows more control over the camera and
 * how it will be initialized.
 */

/**
 * 3. Enable PSRAM on sdkconfig:
 *
 * CONFIG_ESP32_SPIRAM_SUPPORT=y
 *
 * More info on
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html#config-esp32-spiram-support
 */

// ================================ CODE ======================================
#include "include/camara.h"

// static const char *TAG = "example:take_picture";

// #if ESP_CAMERA_SUPPORTED
// static camera_config_t camera_config = {
//     .pin_pwdn = CAM_PIN_PWDN,
//     .pin_reset = CAM_PIN_RESET,
//     .pin_xclk = CAM_PIN_XCLK,
//     .pin_sccb_sda = CAM_PIN_SIOD,
//     .pin_sccb_scl = CAM_PIN_SIOC,

//     .pin_d7 = CAM_PIN_D7,
//     .pin_d6 = CAM_PIN_D6,
//     .pin_d5 = CAM_PIN_D5,
//     .pin_d4 = CAM_PIN_D4,
//     .pin_d3 = CAM_PIN_D3,
//     .pin_d2 = CAM_PIN_D2,
//     .pin_d1 = CAM_PIN_D1,
//     .pin_d0 = CAM_PIN_D0,
//     .pin_vsync = CAM_PIN_VSYNC,
//     .pin_href = CAM_PIN_HREF,
//     .pin_pclk = CAM_PIN_PCLK,

//     //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
//     .xclk_freq_hz = 20000000,
//     .ledc_timer = LEDC_TIMER_0,
//     .ledc_channel = LEDC_CHANNEL_0,

//     .pixel_format = PIXFORMAT_RGB565, //YUV422,GRAYSCALE,RGB565,JPEG
//     .frame_size = FRAMESIZE_UXGA,    //QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

//     .jpeg_quality = 12, //0-63, for OV series camera sensors, lower number means higher quality
//     .fb_count = 1,       //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
//     .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
// };

// static esp_err_t init_camera(esp_mqtt_client_handle_t client)
// {
//     if(CAM_PIN_PWDN != -1){
//         // pinMode(CAM_PIN_PWDN, OUTPUT);
//         // digitalWrite(CAM_PIN_PWDN, LOW);
// 		gpio_set_level(CAM_PIN_PWDN, 0);
//     }

//     //initialize the camera
//     esp_err_t err = esp_camera_init(&camera_config);
//     if (err != ESP_OK)
//     {
//         esp_mqtt_client_publish(client, "/topic/cam", "Camera Init Failed", 0, 0, 0);
//         return err;
//     }

//     return ESP_OK;
// }
// #endif

// void take_picture(esp_mqtt_client_handle_t client)
// {
// #if ESP_CAMERA_SUPPORTED
//     if(ESP_OK != init_camera(client)) {
//         return;
//     }

//     while (1)
//     {
//         esp_mqtt_client_publish(client, "/topic/cam", "Taking picture...", 0, 0, 0);
//         camera_fb_t *pic = esp_camera_fb_get();

//         // use pic->buf to access the image
//         esp_mqtt_client_publish(client, "/topic/cam", "Picture taken! Its size was:", 0, 0, 0);
//         esp_camera_fb_return(pic);

//         vTaskDelay(5000 / portTICK_RATE_MS);
//     }
// #else
//     esp_mqtt_client_publish(client, "/topic/cam", "Camera support is not available for this chip", 0, 0, 0);
//     return;
// #endif
// }