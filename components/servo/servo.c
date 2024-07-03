#include "servo.h"

#define SERVO_TIMER LEDC_TIMER_1
#define SERVO_CHAN LEDC_CHANNEL_0
#define SERVO_TIM_BITS LEDC_TIMER_10_BIT

esp_err_t initalize_servo(gpio_num_t pin)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = SERVO_TIM_BITS,
        .timer_num = SERVO_TIMER,
        .freq_hz = 50, // Set output frequency at 4 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_RETURN_ON_ERROR(ledc_timer_config(&ledc_timer), "servo", "ledc_timer_config failed");

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = SERVO_CHAN,
        .timer_sel = SERVO_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = pin,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    return ledc_channel_config(&ledc_channel);
}

esp_err_t servo_set_angle(float angle)
{
    uint32_t duty = (angle - SERVO_MIN_ANGLE) * (SERVO_MAX_US - SERVO_MIN_US) / (SERVO_MAX_ANGLE - SERVO_MIN_ANGLE) + SERVO_MIN_US; // us
    duty = duty * ((1 << SERVO_TIM_BITS) - 1) * SERVO_FREQ / 1e6;
    ESP_RETURN_ON_ERROR(ledc_set_duty(LEDC_LOW_SPEED_MODE, SERVO_CHAN, duty), "servo", "ledc_set_duty failed");
    return ledc_update_duty(LEDC_LOW_SPEED_MODE, SERVO_CHAN);
}

void test_servo(void *pvParameters)
{
    float angle = 0.0;
    for (;;)
    {
        // ESP_LOGI("servo", "Setting: %f", angle);
        ESP_ERROR_CHECK_WITHOUT_ABORT(servo_set_angle(angle));
        angle = (float)(rand() % 180);
        vTaskDelay(pdMS_TO_TICKS((rand() % 10) * 1000 + 500));
    }
}