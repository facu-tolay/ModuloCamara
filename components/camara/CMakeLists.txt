idf_component_register(SRCS "camara.c"
                        INCLUDE_DIRS "include"
                        REQUIRES espressif__esp32-camera
                        REQUIRES mqtt
                        PRIV_REQUIRES nvs_flash
                                      esp_http_client)