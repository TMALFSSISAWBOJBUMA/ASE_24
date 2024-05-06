#ifndef __DRIVETRAIN_H__
#define __DRIVETRAIN_H__

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include <pinout.h>

#define MAX_PWM 100

enum
{
    FWD=0,
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

#endif