#ifndef __SERVO_H__
#define __SERVO_H__

#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <esp_err.h>
#include <esp_check.h>
#include <esp_log.h>
#include "driver/ledc.h"

#define SERVO_MIN_US 500
#define SERVO_MAX_US 2500
#define SERVO_MAX_ANGLE 180
#define SERVO_MIN_ANGLE 0
#define SERVO_FREQ 50

esp_err_t initalize_servo(gpio_num_t pin);
esp_err_t servo_set_angle(float angle);

void test_servo(void *pvParameters);

#endif