#include "drivetrain.h"

// https://esp32tutorials.com/esp32-dc-motor-l289n-esp-idf/

motor_struct_t left = {.state = STOP, .duty_fix = {1.0, 1.0}}, right = {.state = STOP, .duty_fix = {1.0, 1.0}};

void initialize_motors()
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000; // frequency = 500Hz,
    pwm_config.cmpr_a = 0;       // duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;       // duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, LEFT_FWD_PIN);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, LEFT_REV_PIN);
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    left.timer = MCPWM_TIMER_0;

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, RIGHT_FWD_PIN);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, RIGHT_REV_PIN);
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);
    right.timer = MCPWM_TIMER_1;
}

void set_motors(float duty, bool forward)
{
    if (duty < 0.05)
    {
        motor_stop(&left);
        motor_stop(&right);
        return;
    }
    if (forward)
    {
        motor_forward(&left, duty);
        motor_forward(&right, duty);
    }
    else
    {
        motor_backward(&left, duty);
        motor_backward(&right, duty);
    }
}

/**
 * @brief motor moves in forward direction, with duty cycle = duty %
 */
void motor_forward(motor_struct_t *motor, float duty_cycle)
{
    mcpwm_set_signal_low(MCPWM_UNIT_0, motor->timer, MCPWM_OPR_B);
    mcpwm_set_duty(MCPWM_UNIT_0, motor->timer, MCPWM_OPR_A, motor->duty_fix[FWD] * duty_cycle);
    if (FWD != motor->state)
        mcpwm_set_duty_type(MCPWM_UNIT_0, motor->timer, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
    motor->state = FWD;
}

/**
 * @brief motor moves in backward direction, with duty cycle = duty %
 */
void motor_backward(motor_struct_t *motor, float duty_cycle)
{
    mcpwm_set_signal_low(MCPWM_UNIT_0, motor->timer, MCPWM_OPR_A);
    mcpwm_set_duty(MCPWM_UNIT_0, motor->timer, MCPWM_OPR_B, motor->duty_fix[REV] * duty_cycle);
    if (REV != motor->state)
        mcpwm_set_duty_type(MCPWM_UNIT_0, motor->timer, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
    motor->state = REV;
}

/**
 * @brief motor stop
 */
void motor_stop(motor_struct_t *motor)
{
    mcpwm_set_signal_low(MCPWM_UNIT_0, motor->timer, MCPWM_OPR_A);
    mcpwm_set_signal_low(MCPWM_UNIT_0, motor->timer, MCPWM_OPR_B);
    motor->state = STOP;
}
