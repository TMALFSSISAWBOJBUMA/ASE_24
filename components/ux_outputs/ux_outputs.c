#include "ux_outputs.h"

#define LEDC_SPEED LEDC_LOW_SPEED_MODE
#define BUZZ_TIMER LEDC_TIMER_0
#define BUZZ_CHAN LEDC_CHANNEL_0

esp_err_t initalize_buzzer(gpio_num_t pin)
{
    esp_err_t ret = ESP_OK;
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_SPEED,
        .duty_resolution = LEDC_TIMER_2_BIT,
        .timer_num = BUZZ_TIMER,
        .freq_hz = 1000, // Set output frequency at 4 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_SPEED,
        .channel = BUZZ_CHAN,
        .timer_sel = BUZZ_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = pin,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    return ESP_OK;
}

void buzzer_set_freq(uint32_t freq)
{
    ledc_set_freq(LEDC_SPEED, BUZZ_TIMER, freq);
}

void buzzer_start()
{
    ledc_set_duty(LEDC_SPEED, BUZZ_CHAN, 2);
}

void buzzer_stop()
{
    ledc_set_duty(LEDC_SPEED, BUZZ_CHAN, 0);
}

#define SERVO_TIMER LEDC_TIMER_1
#define SERVO_CHAN LEDC_CHANNEL_0
#define SERVO_TIM_BITS LEDC_TIMER_10_BIT
esp_err_t initalize_servo(gpio_num_t pin)
{
    esp_err_t ret = ESP_OK;
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = SERVO_TIM_BITS,
        .timer_num = SERVO_TIMER,
        .freq_hz = 50, // Set output frequency at 4 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = SERVO_CHAN,
        .timer_sel = SERVO_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = pin,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    return ESP_OK;
}

void servo_set_angle(float angle)
{
    uint32_t duty = (angle - SERVO_MIN_ANGLE) * (SERVO_MAX_US - SERVO_MIN_US) / (SERVO_MAX_ANGLE - SERVO_MIN_ANGLE) + SERVO_MIN_US; //us
    duty = duty * (1<<SERVO_TIM_BITS - 1) * SERVO_FREQ / 1e6; 
    ledc_set_duty(LEDC_SPEED, SERVO_CHAN, duty);
}