idf_component_register(
    SRCS ultrasonic.c
    INCLUDE_DIRS "include"
    REQUIRES driver freertos esp_timer
)
