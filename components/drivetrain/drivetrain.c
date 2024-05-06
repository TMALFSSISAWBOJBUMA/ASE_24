#include "drivetrain.h"

// https://esp32tutorials.com/esp32-dc-motor-l289n-esp-idf/

void initialize_motors(motor_struct_t *left, motor_struct_t *right)
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, LEFT_FWD_PIN);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, LEFT_REV_PIN);
    left->timer = MCPWM_TIMER_0;
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, RIGHT_FWD_PIN);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, RIGHT_REV_PIN);
    right->timer = MCPWM_TIMER_1;
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
