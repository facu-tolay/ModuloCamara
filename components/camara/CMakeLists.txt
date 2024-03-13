idf_component_register(SRCS "camara.c"
                        INCLUDE_DIRS "include"
                        REQUIRES espressif__esp32-camera
                        PRIV_REQUIRES nvs_flash)