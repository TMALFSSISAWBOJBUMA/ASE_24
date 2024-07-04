#include "sensors.h"

void initalize_sensors()
{
    gpio_set_direction(IR_FRONT_R, GPIO_MODE_INPUT);
    gpio_set_direction(IR_FRONT_L, GPIO_MODE_INPUT);
    gpio_set_direction(IR_SIDE_LF, GPIO_MODE_INPUT);
    gpio_set_direction(IR_SIDE_LB, GPIO_MODE_INPUT);
    gpio_set_direction(IR_SIDE_RB, GPIO_MODE_INPUT);
    gpio_set_direction(IR_SIDE_RF, GPIO_MODE_INPUT);
    gpio_set_direction(IR_TAPE_L, GPIO_MODE_INPUT);
    gpio_set_direction(IR_TAPE_R, GPIO_MODE_INPUT);
    gpio_set_direction(IR_BACK, GPIO_MODE_INPUT);
}

bool sensors_obstacle_front()
{
    return !(gpio_get_level(IR_FRONT_R) && gpio_get_level(IR_FRONT_L));
}

bool sensors_obstacle_back()
{
    return !gpio_get_level(IR_BACK);
}

bool sensors_obstacle_left()
{
    return !(gpio_get_level(IR_SIDE_LF) && gpio_get_level(IR_SIDE_LB));
}

bool sensors_obstacle_right()
{
    return !(gpio_get_level(IR_SIDE_RF) && gpio_get_level(IR_SIDE_RB));
}

inline bool sensors_tape_detected()
{
    return !(gpio_get_level(IR_TAPE_L) && gpio_get_level(IR_TAPE_R));
}