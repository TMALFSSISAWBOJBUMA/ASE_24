#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "pinout.h"
#include <esp_err.h>
#include <esp_check.h>
#include "bdc_motor.h"
#include "driver/pulse_cnt.h"
#include "sensors.h"
#include "esp_log.h"

typedef struct
{
    bdc_motor_handle_t motor;
    pcnt_unit_handle_t encoder;
} motor_t;

motor_t left, right;
const uint32_t PMW_FREQUENCY = 1000, RESOLUTION_HZ = 1000000;

void motors_direction(bool forward)
{
    if (forward)
    {
        bdc_motor_forward(left.motor);
        bdc_motor_forward(right.motor);
    }
    else
    {
        bdc_motor_reverse(left.motor);
        bdc_motor_reverse(right.motor);
    }
}

void motors_speed(float speed)
{
    uint32_t speed_32 = speed / 100.0 * (RESOLUTION_HZ / PMW_FREQUENCY-1);
    bdc_motor_set_speed(left.motor, speed_32);
    bdc_motor_set_speed(right.motor, speed_32);
}

esp_err_t configure_pcnt(gpio_num_t count_pin, pcnt_unit_handle_t *ret_unit){
    pcnt_unit_config_t pcnt_config = {
        .low_limit = -1,
        .high_limit = 32767, // PCNT_LL_MAX_LIM
        .intr_priority = 1,
    };
    ESP_RETURN_ON_ERROR(pcnt_new_unit(&pcnt_config, ret_unit), "pcnt", "pcnt_new_unit failed");
    pcnt_chan_config_t pcnt_channel_config = {
        .edge_gpio_num = count_pin,
        .level_gpio_num = -1,
        .flags={},
    };
    pcnt_channel_handle_t pcnt_chan = NULL;
    ESP_RETURN_ON_ERROR(pcnt_new_channel(*ret_unit, &pcnt_channel_config, &pcnt_chan), "pcnt", "pcnt_new_channel failed");
    ESP_RETURN_ON_ERROR(pcnt_channel_set_edge_action(pcnt_chan, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE), "pcnt", "pcnt_channel_set_edge_action failed");
    ESP_ERROR_CHECK(pcnt_unit_enable(*ret_unit));
    return pcnt_unit_start(*ret_unit);
}

void initialize_motors()
{
    ESP_LOGI("initialize motors", "start\n");
    bdc_motor_config_t motor_config = {
        .pwm_freq_hz = 1000,
        .pwma_gpio_num = LEFT_FWD_PIN,
        .pwmb_gpio_num = LEFT_REV_PIN,
    };
    bdc_motor_mcpwm_config_t mcpwm_config = {
        .group_id = 0,
        .resolution_hz = RESOLUTION_HZ,
    };

    ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &left.motor));
    ESP_ERROR_CHECK(configure_pcnt(SPEED_L, &left.encoder));

    motor_config.pwma_gpio_num = RIGHT_FWD_PIN;
    motor_config.pwmb_gpio_num = RIGHT_REV_PIN;
    ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &right.motor));
    ESP_ERROR_CHECK(configure_pcnt(SPEED_R, &right.encoder));

    ESP_LOGI("initialize motors", "enabling motor drivers\n");
    bdc_motor_enable(left.motor);
    bdc_motor_enable(right.motor);
}

typedef enum
{
    FORWARD,
    REVERSING,
    TWISTING,
    WAITING
} algorithm_state_e;

#define MAX_SPEED 50.0
void app_main()
{
    // ultrasonic_config(TRIGGER_PIN, ECHO_PIN);
    // xTaskCreate(measure_distance, "dist_meas", configMINIMAL_STACK_SIZE * 2, NULL, 5, NULL);
    algorithm_state_e state = WAITING;
    initialize_motors();
    initalize_sensors();
    bool obstacle_in_front = sensors_obstacle_front();
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(20));
        obstacle_in_front = sensors_obstacle_front() || sensors_tape_detected();
        ESP_LOGI("main", "%d -> %d\n", state, obstacle_in_front);
        switch (state)
        {
        case FORWARD:
            if (obstacle_in_front)
            {
                motors_speed(0);
                vTaskDelay(pdMS_TO_TICKS(100));
                state = REVERSING;
            }
            break;
        case REVERSING:
            motors_direction(false);
            motors_speed(MAX_SPEED);
            vTaskDelay(pdMS_TO_TICKS(1200));
            motors_speed(0);
            state = TWISTING;
            break;
        case TWISTING:
            bdc_motor_forward(left.motor);
            bdc_motor_reverse(right.motor);
            motors_speed(MAX_SPEED);
            vTaskDelay(pdMS_TO_TICKS(500));
            motors_speed(0);
            state = WAITING;
            break;
        case WAITING:
            state = FORWARD;
            motors_direction(true);
            motors_speed(MAX_SPEED);
            break;
        default:
            state = WAITING;
        }
    }
}
