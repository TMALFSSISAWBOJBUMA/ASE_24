#include "sensors.h"

void initalize_sensors()
{
    gpio_set_direction(IR_FRONT_L, GPIO_MODE_INPUT);
    gpio_set_direction(IR_FRONT_R, GPIO_MODE_INPUT);
    gpio_set_direction(IR_SIDE_L, GPIO_MODE_INPUT);
    gpio_set_direction(IR_SIDE_R, GPIO_MODE_INPUT);
    gpio_set_direction(IR_TAPE_L, GPIO_MODE_INPUT);
    gpio_set_direction(IR_TAPE_R, GPIO_MODE_INPUT);
}

bool sensors_obstacle_in_front()
{
    return !(gpio_get_level(IR_FRONT_R) && gpio_get_level(IR_FRONT_L));
}

bool sensors_obstacle_on_side()
{
    return !(gpio_get_level(IR_SIDE_R) && gpio_get_level(IR_SIDE_L));
}

bool sensors_tape_detected()
{
    return !(gpio_get_level(IR_TAPE_R) && gpio_get_level(IR_TAPE_L));
}