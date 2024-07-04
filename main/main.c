#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "pinout.h"
#include "config.h"
#include <esp_err.h>
#include <esp_check.h>
#include "sensors.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <math.h>
#include "bt_comms.h"
#include "driver/i2c_master.h"
#include "TMAG5273.h"
#include "servo.h"
#include "buzzer.h"
#include <string.h>
#include "main.h"

app_context_t app_context = {.run = false};

void motors_direction(bool forward)
{
    if (forward)
    {
        bdc_motor_forward(app_context.motor_ctx.m_left.motor);
        bdc_motor_forward(app_context.motor_ctx.m_right.motor);
    }
    else
    {
        bdc_motor_reverse(app_context.motor_ctx.m_left.motor);
        bdc_motor_reverse(app_context.motor_ctx.m_right.motor);
    }
}

void motors_speed(float speed)
{
    speed = speed / 100.0 * (RESOLUTION_HZ / PMW_FREQUENCY - 1);
    bdc_motor_set_speed(app_context.motor_ctx.m_left.motor, (uint32_t)(speed * app_context.motor_ctx.m_left.comp));
    bdc_motor_set_speed(app_context.motor_ctx.m_right.motor, (uint32_t)(speed * app_context.motor_ctx.m_right.comp));
}

void motors_handler(void *args)
{
    motor_control_context_t *ctx = (motor_control_context_t *)args;

    int dl = get_distance_update_speed(&ctx->m_left);
    int dr = get_distance_update_speed(&ctx->m_right);

    if (dl == dr)
    {
        ctx->x += (float)dl * cos(ctx->azimuth);
        ctx->y += (float)dl * sin(ctx->azimuth);
    }
    else
    {
        float R = dl, omega = 0.0;
        R = TRACKS_DISTANCE / 2.0f * (float)(dl + dr) / (float)(dr - dl);
        omega = (float)(dr - dl) / TRACKS_DISTANCE;
        ctx->x += R * (sin(ctx->azimuth) * (cos(omega) - 1.0f) + cos(ctx->azimuth) * sin(omega));
        ctx->y += R * (cos(ctx->azimuth) * (1.0f - cos(omega)) + sin(ctx->azimuth) * sin(omega));
        ctx->azimuth += omega;
    }

    ctx->speed = (ctx->m_right.speed + ctx->m_left.speed) / 2.0f;

    // char buff[60] = {0};
    // sprintf(buff, "%3d,%3d,%3.4f,%3.4f;%2.5f\n", dl, dr, ctx->x, ctx->y, ctx->azimuth);
    // bt_comms_send(buff);
}

void buzzer_finish(void *pvParameters)
{
    uint16_t freq = 1200;
    int8_t diff = 100;
    buzzer_start();
    for (;;)
    {
        buzzer_set_freq(freq);
        freq += diff;
        if (freq > 5000 || freq < 1200)
            diff *= -1;
        vTaskDelay(pdMS_TO_TICKS(80));
    }
}

void initialize_motors()
{
    motor_driver_config_t motor_cfg = {
        .encoder = SPEED_L,
        .fwd_pin = LEFT_FWD_PIN,
        .rev_pin = LEFT_REV_PIN,
        .max_pwm_comp = 0.9f};
    initialize_motor(&motor_cfg, &app_context.motor_ctx.m_left);
    motor_cfg.encoder = SPEED_R;
    motor_cfg.fwd_pin = RIGHT_FWD_PIN;
    motor_cfg.rev_pin = RIGHT_REV_PIN;
    motor_cfg.max_pwm_comp = 1.0f;
    initialize_motor(&motor_cfg, &app_context.motor_ctx.m_right);

    esp_timer_create_args_t periodic_timer_args = {
        .callback = motors_handler,
        .arg = &app_context.motor_ctx,
        .name = "motors_handler"};
    esp_timer_handle_t motors_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &motors_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(motors_timer, HANDLER_PERIOD * 1000));

    ESP_LOGI("initialize motors", "enabling motor drivers\n");
    bdc_motor_enable(app_context.motor_ctx.m_left.motor);
    bdc_motor_enable(app_context.motor_ctx.m_right.motor);
}

#define MAX_SPEED 50.0
void algorithm(void *pvParameters)
{
    app_context_t *ctx = (app_context_t *)pvParameters;
    ctx->state = WAITING;
    char buff[MAX_MSG_LEN] = {0};
    uint8_t keep_state = 0;
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(30));
        ctx->sensors.front = sensors_obstacle_front();
        ctx->sensors.back = sensors_obstacle_back();
        ctx->sensors.right = sensors_obstacle_right();
        ctx->sensors.left = sensors_obstacle_left();
        ctx->sensors.tape = sensors_tape_detected();
        keep_state = keep_state ? keep_state - 1 : 0;
        // ESP_LOGI("main", "%d -> %d\n", state, obstacle_in_front);
        switch (ctx->state)
        {
        case FORWARD:
            if (ctx->sensors.tape)
                vTaskDelay(pdMS_TO_TICKS(200));
            if (ctx->sensors.front || ctx->sensors.tape)
            {
                motors_speed(0);
                vTaskDelay(pdMS_TO_TICKS(100));
                ctx->state = REVERSING;
                keep_state = 11;
            }
            break;
        case REVERSING:
            if (keep_state == 10)
            {
                motors_direction(false);
                motors_speed(MAX_SPEED);
            }
            else if (!keep_state || ctx->sensors.back)
            {
                motors_speed(0);
                keep_state = 11;
                ctx->state = TWISTING;
            }
            break;
        case TWISTING:
            if (keep_state == 10)
            {
                bdc_motor_forward(app_context.motor_ctx.m_left.motor);
                bdc_motor_reverse(app_context.motor_ctx.m_right.motor);
                motors_speed(MAX_SPEED);
            }
            else if (!keep_state)
            {
                motors_speed(0);
                ctx->state = WAITING;
            }
            break;
        case WAITING:
            if (!ctx->run)
                break;
            ctx->state = FORWARD;
            motors_direction(true);
            motors_speed(MAX_SPEED);
            break;
        case FINISHED:
            motors_speed(0);
            break;
        case STOPPING:
            motors_speed(0);
        default:
            ctx->state = WAITING;
        }

        if (xQueueReceive(ctx->BT_q, buff, (TickType_t)5) == pdTRUE)
        {
            if (strcmp(buff, "Start\n") == 0)
            {
                ESP_LOGI("algorithm", "starting");
                ctx->run = true;
            }
            else if (strcmp(buff, "Stop\n") == 0)
            {
                ESP_LOGI("algorithm", "stopping");
                ctx->run = false;
                ctx->state = STOPPING;
            }
            else if (strcmp(buff, "Servo\n") == 0)
            {
                initalize_servo(SERVO_PIN);
                xTaskCreate(&test_servo, "servo", configMINIMAL_STACK_SIZE * 2, NULL, 1, NULL);
            }
        }

        if (xQueueReceive(ctx->TMAG_q, &ctx->sensors.mag_z, (TickType_t)5) == pdTRUE)
        {
            if (ctx->sensors.tape && fabs(ctx->sensors.mag_z) > 2.0)
            {
                ctx->state = FINISHED;
                ESP_LOGI("algorithm", "finished");
                xTaskCreate(&buzzer_finish, "buzzer", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
            }
        }
    }
}

void read_TMAG(void *pvParameters)
{
    app_context_t *ctx = (app_context_t *)pvParameters;
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = SCL_PIN,
        .sda_io_num = SDA_PIN,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));
    TMAG5273_device_t hall_sensor;
    ESP_ERROR_CHECK(TMAG5273_init(TMAG5273_I2C_ADDRESS_INITIAL, bus_handle, &hall_sensor));
    float readout;
    vTaskDelay(1);
    while (1)
    {
        readout = TMAG5273_getZData(&hall_sensor);
        xQueueSend(ctx->TMAG_q, &readout, (TickType_t)0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main()
{
#ifdef USE_ULTRASONIC
    ultrasonic_config(TRIGGER_PIN, ECHO_PIN);
    xTaskCreate(measure_distance, "dist_meas", configMINIMAL_STACK_SIZE * 2, NULL, 5, NULL);
#endif
    app_context.TMAG_q = xQueueCreate(2, sizeof(float));
#ifdef USE_TMAG5273
    xTaskCreatePinnedToCore(read_TMAG, "hall", configMINIMAL_STACK_SIZE * 3, &app_context, 2, NULL, 1);
#endif
    initalize_sensors();
    initalize_buzzer(BUZZER_PIN);
    app_context.BT_q = xQueueCreate(5, MAX_MSG_LEN);
    xTaskCreate(&algorithm, "algorithm", configMINIMAL_STACK_SIZE * 2, &app_context, 1, NULL);
    bt_comms_init(app_context.BT_q);
    initialize_motors();

    while (1)
    {
        char buff[40] = {0};
        sprintf(buff, "%2.4f;%d;%d;%d;%d;%d;%3.2f;%3.2f;%d\n",
                app_context.sensors.mag_z,
                app_context.sensors.front,
                app_context.sensors.back,
                app_context.sensors.right,
                app_context.sensors.left,
                app_context.sensors.tape,
                app_context.motor_ctx.m_left.speed,
                app_context.motor_ctx.m_right.speed,
                app_context.state);
        bt_comms_send(buff);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
