#ifndef __MAIN_H__
#define __MAIN_H__

#include <freertos/queue.h>
#include <freertos/task.h>
#include "drivetrain.h"

typedef struct
{
    motor_driver_t m_left, m_right;
    float x, y, speed, azimuth;
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

typedef enum
{
    FORWARD,
    REVERSING,
    TWISTING,
    STOPPING,
    WAITING,
    FINISHED
} algorithm_state_e;
typedef struct
{
    motor_control_context_t motor_ctx;
    QueueHandle_t BT_q, TMAG_q;
    sensors_readout_t sensors;
    algorithm_state_e state;
    bool run;
    TaskHandle_t music_task;
} app_context_t;

#endif