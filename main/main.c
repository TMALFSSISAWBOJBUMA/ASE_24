#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "pinout.h"
#include "ultrasonic.h"
#include "dht11.h"
#include <esp_err.h>
#include "bdc_motor.h"
#include "sensors.h"
#include "esp_log.h"

float front_distance = 1.0;
float humidity, temperature = 23.0;

void measure_environment(void *pvParameters)
{
    while (true)
    {
        esp_err_t res = dht_read_float_data(DHT_TYPE_DHT11, DHT_PIN, &humidity, &temperature);
        if (res != ESP_OK)
        {
            printf("DHT error %d: ", res);
            switch (res)
            {
            case ESP_ERR_TIMEOUT:
                printf("Timeout occurred\n");
                break;
            case ESP_ERR_INVALID_CRC:
                printf("Checksum failed, invalid data received from sensor\n");
                break;
            default:
                printf("%s\n", esp_err_to_name(res));
            }
        }
        // else
        //     printf("DHT Humidity: %.2f%%, Temperature: %.2f°C\n", humidity, temperature);

        vTaskDelay(pdMS_TO_TICKS(2000)); // Czekaj 2 sekundy przed kolejnym pomiarem
    }
}

bdc_motor_handle_t left, right;
const uint32_t PMW_FREQUENCY = 1000, RESOLUTION_HZ = 1000000;

void motors_direction(bool forward)
{
    if (forward)
    {
        bdc_motor_forward(left);
        bdc_motor_forward(right);
    }
    else
    {
        bdc_motor_reverse(left);
        bdc_motor_reverse(right);
    }
}

void motors_speed(float speed)
{
    uint32_t speed_32 = speed / 100.0 * (RESOLUTION_HZ / PMW_FREQUENCY-1);
    bdc_motor_set_speed(left, speed_32);
    bdc_motor_set_speed(right, speed_32);
}

void initialize_motors()
{
    printf("initializing mcpwm gpio...\n");
    bdc_motor_config_t motor_config = {
        .pwm_freq_hz = 1000,
        .pwma_gpio_num = LEFT_FWD_PIN,
        .pwmb_gpio_num = LEFT_REV_PIN,
    };
    bdc_motor_mcpwm_config_t mcpwm_config = {
        .group_id = 0,
        .resolution_hz = RESOLUTION_HZ,
    };

    ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &left));

    motor_config.pwma_gpio_num = RIGHT_FWD_PIN;
    motor_config.pwmb_gpio_num = RIGHT_REV_PIN;
    ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &right));

    bdc_motor_enable(left);
    bdc_motor_enable(right);
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
    ultrasonic_config(TRIGGER_PIN, ECHO_PIN);
    xTaskCreate(measure_distance, "dist_meas", configMINIMAL_STACK_SIZE * 2, NULL, 5, NULL);
    algorithm_state_e state = WAITING;
    initialize_motors();
    initalize_sensors();
    // xTaskCreate(measure_environment, "env_meas", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    bool obstacle_in_front = sensors_obstacle_in_front();
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(20));
        obstacle_in_front = sensors_obstacle_in_front() || sensors_tape_detected();
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
            bdc_motor_forward(left);
            bdc_motor_reverse(right);
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
