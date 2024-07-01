#ifndef __UX_OUTPUTS_H__
#define __UX_OUTPUTS_H__

#include <stdbool.h>
#include "pinout.h"
#include "driver/ledc.h"
#include <esp_err.h>
#include <esp_log.h>


#define SERVO_MIN_US 500
#define SERVO_MAX_US 2500
#define SERVO_MAX_ANGLE 180
#define SERVO_MIN_ANGLE 0
#define SERVO_FREQ 50

esp_err_t initalize_buzzer(gpio_num_t pin);
void buzzer_set_freq(uint32_t freq);
void buzzer_start();
void buzzer_stop();

#endif
