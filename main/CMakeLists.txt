idf_component_register(SRCS "main.c"
REQUIRES ultrasonic sensors driver esp_timer bt_comms TMAG5273 ux_outputs
                    INCLUDE_DIRS "include")