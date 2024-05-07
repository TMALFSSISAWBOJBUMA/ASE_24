#ifndef __DRIVETRAIN_H__
#define __DRIVETRAIN_H__

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include <pinout.h>

#define MAX_PWM 100

typedef enum
{
    FWD = 0,
    REV,
    STOP
} motor_state_e;

typedef struct
{
    mcpwm_timer_t timer;
    motor_state_e state;
    float duty;
    float duty_fix[2];
} motor_struct_t;

void initialize_motors();
void set_motors(float duty, bool forward);
void motor_forward(motor_struct_t *motor, float duty_cycle);
void motor_backward(motor_struct_t *motor, float duty_cycle);
void motor_stop(motor_struct_t *motor);

#endif