#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <stdbool.h>
#include <pinout.h>
#include <driver/gpio.h>

void initalize_sensors();
bool sensors_obstacle_front();
bool sensors_obstacle_back();
bool sensors_obstacle_right();
bool sensors_obstacle_left();
bool sensors_tape_detected();

#endif