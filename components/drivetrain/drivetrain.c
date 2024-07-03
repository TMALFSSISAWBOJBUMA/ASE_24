#include <stdio.h>
#include "drivetrain.h"

static esp_err_t set_edge(pcnt_channel_handle_t channel, pcnt_channel_edge_action_t edge_action){
    return pcnt_channel_set_edge_action(channel, edge_action, edge_action);
}

esp_err_t configure_pcnt(gpio_num_t count_pin, pcnt_unit_handle_t *ret_unit, pcnt_channel_handle_t *ret_channel)
{
    pcnt_unit_config_t pcnt_config = {
        .low_limit = -1,
        .high_limit = 32767, // PCNT_LL_MAX_LIM
        .intr_priority = 1,
    };
    ESP_RETURN_ON_ERROR(pcnt_new_unit(&pcnt_config, ret_unit), "pcnt", "pcnt_new_unit failed");
    pcnt_chan_config_t pcnt_channel_config = {
        .edge_gpio_num = count_pin,
        .level_gpio_num = -1,
        .flags = {},
    };
    ESP_RETURN_ON_ERROR(pcnt_new_channel(*ret_unit, &pcnt_channel_config, ret_channel), "pcnt", "pcnt_new_channel failed");
    ESP_RETURN_ON_ERROR(set_edge(*ret_channel, PCNT_CHANNEL_EDGE_ACTION_INCREASE), "pcnt", "pcnt_channel_set_edge_action failed");
    ESP_ERROR_CHECK(pcnt_unit_enable(*ret_unit));
    return pcnt_unit_start(*ret_unit);
}

int get_pulses_update_speed(motor_driver_t *motor)
{
    int pcnt_val;
    pcnt_unit_get_count(motor->encoder, &pcnt_val);
    int dp = (pcnt_val - motor->last_pulses) * PULSE_RESOLUTION;
    motor->last_pulses = pcnt_val;
    motor->speed = dp / (HANDLER_PERIOD / 1e3);
    return dp;
}


void initialize_motor(motor_driver_config_t* config, motor_driver_t* driver)
{
    bdc_motor_config_t motor_config = {
        .pwm_freq_hz = PMW_FREQUENCY,
        .pwma_gpio_num = config->fwd_pin,
        .pwmb_gpio_num = config->rev_pin,
    };
    bdc_motor_mcpwm_config_t mcpwm_config = {
        .group_id = 0,
        .resolution_hz = RESOLUTION_HZ,
    };

    ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &driver->motor));
    ESP_ERROR_CHECK(configure_pcnt(config->encoder, &driver->encoder, &driver->encoder_channel));
}
