idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "include"
                    REQUIRES wifi
                             camara
                             mqtt_cmp)