#include "buzzer.h"

#define LEDC_SPEED LEDC_LOW_SPEED_MODE
#define BUZZ_TIMER LEDC_TIMER_0
#define BUZZ_CHAN LEDC_CHANNEL_0

esp_err_t initalize_buzzer(gpio_num_t pin)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_SPEED,
        .duty_resolution = LEDC_TIMER_2_BIT,
        .timer_num = BUZZ_TIMER,
        .freq_hz = 1000, // Set output frequency at 4 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_RETURN_ON_ERROR(ledc_timer_config(&ledc_timer), "buzzer", "ledc_timer_config failed");

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
    ledc_update_duty(LEDC_SPEED, BUZZ_CHAN);
}

void buzzer_stop()
{
    ledc_set_duty(LEDC_SPEED, BUZZ_CHAN, 0);
    ledc_update_duty(LEDC_SPEED, BUZZ_CHAN);
}

void test_buzzer(void *pvParameters)
{
    uint16_t freq = 500;
    int8_t diff = 100;
    for (;;)
    {
        buzzer_set_freq(freq);
        freq += diff;
        if (freq > 5000 || freq < 900)
            diff *= -1;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void play_melody(void *pvParameters)
{
    melody_t *melody = (melody_t *)pvParameters;

    // this calculates the duration of a whole note in ms
    uint16_t wholenote = (60000 * 4) / melody->tempo;

    int16_t divider = 0, noteDuration = 0;
    while (1)
    {
        for (int thisNote = 0; thisNote < melody->length; thisNote++)
        {

            // calculates the duration of each note
            divider = melody->notes[2 * thisNote + 1];
            if (divider > 0)
            {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
            }
            else if (divider < 0)
            {
                // dotted notes are represented with negative durations!!
                noteDuration = (wholenote) / abs(divider);
                noteDuration *= 1.5; // increases the duration in half for dotted notes
            }

            if (melody->notes[2 * thisNote])
            {
                buzzer_start();
                buzzer_set_freq(melody->notes[2 * thisNote]);
            }

            vTaskDelay(pdMS_TO_TICKS(noteDuration * 0.9));
            buzzer_stop();
            vTaskDelay(pdMS_TO_TICKS(noteDuration * 0.1));
        }
    }
}