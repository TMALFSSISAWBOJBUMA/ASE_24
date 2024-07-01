idf_component_register(SRCS "main.c"
REQUIRES ultrasonic sensors driver esp_timer bt_comms TMAG5273 buzzer servo
                    INCLUDE_DIRS "include")