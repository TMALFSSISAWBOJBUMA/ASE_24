#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <freertos/FreeRTOS.h>
#include <esp_err.h>
#include <esp_check.h>
#include <esp_log.h>
#include "driver/ledc.h"

esp_err_t initalize_buzzer(gpio_num_t pin);
void buzzer_set_freq(uint32_t freq);
void buzzer_start();
void buzzer_stop();

void test_buzzer(void *pvParameters);

#endif
