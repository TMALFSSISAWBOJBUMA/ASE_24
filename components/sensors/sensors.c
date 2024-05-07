#include "sensors.h"

bool sensors_obstacle_in_front()
{
    int prawy_przod = gpio_get_level(IR_FRONT_R);
    int lewy_przod = gpio_get_level(IR_FRONT_L);
    
    return (prawy_przod || lewy_przod);
}

bool sensors_obstacle_on_side()
{
    int prawy_bok = gpio_get_level(IR_SIDE_R);
    int lewy_bok = gpio_get_level(IR_SIDE_L);
    
    return (prawy_bok || lewy_bok);
}
