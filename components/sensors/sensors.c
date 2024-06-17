#include "sensors.h"

void initalize_sensors()
{
    gpio_set_direction(IR_FRONT, GPIO_MODE_INPUT);
    gpio_set_direction(IR_SIDE_L, GPIO_MODE_INPUT);
    gpio_set_direction(IR_SIDE_R, GPIO_MODE_INPUT);
    gpio_set_direction(IR_TAPE, GPIO_MODE_INPUT);
    gpio_set_direction(IR_BACK, GPIO_MODE_INPUT);
}

bool sensors_obstacle_front()
{
    return !gpio_get_level(IR_FRONT);
}

bool sensors_obstacle_back()
{
    return !gpio_get_level(IR_BACK);
}

bool sensors_obstacle_left()
{
    return !gpio_get_level(IR_SIDE_L);
}

bool sensors_obstacle_right()
{
    return !gpio_get_level(IR_SIDE_R);
}

inline bool sensors_tape_detected()
{
    return !(gpio_get_level(IR_TAPE));
}