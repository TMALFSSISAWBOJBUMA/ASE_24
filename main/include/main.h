#ifndef __MAIN_H__
#define __MAIN_H__

#include <freertos/queue.h>
#include "drivetrain.h"

typedef struct
{
    motor_driver_t m_left, m_right;
    int32_t x, y;
    float speed, azimuth;
} motor_control_context_t;

typedef struct
{
    uint8_t front : 1;
    uint8_t back : 1;
    uint8_t right : 1;
    uint8_t left : 1;
    uint8_t tape : 1;
    float mag_z;
} sensors_readout_t;

typedef struct
{
    motor_control_context_t motor_ctx;
    QueueHandle_t BT_q, TMAG_q;
    sensors_readout_t sensors;
    bool run;
} app_context_t;

#endif