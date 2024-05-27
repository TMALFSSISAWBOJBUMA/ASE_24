#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <stdbool.h>
#include <pinout.h>
#include <driver/gpio.h>

void initalize_sensors();
bool sensors_obstacle_in_front();
bool sensors_obstacle_on_side();
bool sensors_tape_detected();

#endif