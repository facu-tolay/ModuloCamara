#include <stdio.h>
#include "esp_netif.h"
#include "freertos/task.h"

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 2048
#define CONFIG_EXAMPLE_HTTP_ENDPOINT "192.168.0.40:8080"

static void http_test_task(void *pvParameters);


#include "../../components/wifi/include/wifi.h"
#include "../../components/camara/include/camara.h"
#include "../../components/mqtt_cmp/client_mqtt.h"