#ifndef __DRIVETRAIN_H__
#define __DRIVETRAIN_H__

#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
#include "bdc_motor.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"

#define PMW_FREQUENCY 1000 // Hz
#define RESOLUTION_HZ 1000000
#define HANDLER_PERIOD 100    // ms
#define TRACKS_DISTANCE 95.0f // mm
#define PULSE_RESOLUTION 4    // mm/pulse

typedef struct
{
    bdc_motor_handle_t motor;
    pcnt_unit_handle_t encoder;
    pcnt_channel_handle_t encoder_channel;
    float speed;
    int32_t last_pulses;
} motor_driver_t;

typedef struct
{
    gpio_num_t encoder;
    gpio_num_t fwd_pin;
    gpio_num_t rev_pin;
    float max_pwm_comp; // for motor speed calibration
} motor_driver_config_t;

void initialize_motor(motor_driver_config_t *config, motor_driver_t *driver);
int get_distance_update_speed(motor_driver_t *motor);

#endif