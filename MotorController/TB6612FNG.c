#include "TB6612FNG.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

static void _configure_motor(Motor *motor) {
    gpio_init(motor->pins.in1);
    gpio_set_dir(motor->pins.in1, GPIO_OUT);
    gpio_init(motor->pins.in2);
    gpio_set_dir(motor->pins.in2, GPIO_OUT);

    gpio_set_function(motor->pins.pwm, GPIO_FUNC_PWM);
    motor->pwm_slice = pwm_gpio_to_slice_num(motor->pins.pwm);
    motor->pwm_channel = pwm_gpio_to_channel(motor->pins.pwm);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);
    pwm_config_set_wrap(&config, 65535);
    pwm_init(motor->pwm_slice, &config, true);
    motor->max_pwm = 65535;
}

void tb6612fng_init(DualMotorDriver *driver, uint8_t left_in1, uint8_t left_in2, uint8_t left_pwm, uint8_t right_in1, uint8_t right_in2, uint8_t right_pwm) {
    driver->left_motor.pins.in1 = left_in1;
    driver->left_motor.pins.in2 = left_in2;
    driver->left_motor.pins.pwm = left_pwm;
    _configure_motor(&driver->left_motor);

    driver->right_motor.pins.in1 = right_in1;
    driver->right_motor.pins.in2 = right_in2;
    driver->right_motor.pins.pwm = right_pwm;
    _configure_motor(&driver->right_motor);

    tb6612fng_stop(driver);
}

void tb6612fng_set_motor(Motor *motor, MotorDirection direction, MotorSpeed speed) {
    switch (direction) {
        case MOTOR_FORWARD:
            gpio_put(motor->pins.in1, 1);
            gpio_put(motor->pins.in2, 0);
            break;
        case MOTOR_BACKWARD:
            gpio_put(motor->pins.in1, 0);
            gpio_put(motor->pins.in2, 1);
            break;
        case MOTOR_STOP:
            gpio_put(motor->pins.in1, 0);
            gpio_put(motor->pins.in2, 0);
            break;
    }

    pwm_set_chan_level(motor->pwm_slice, motor->pwm_channel, speed);
}

void tb6612fng_forward(DualMotorDriver *driver, MotorSpeed speed) {
    tb6612fng_set_motor(&driver->left_motor, MOTOR_FORWARD, speed);
    tb6612fng_set_motor(&driver->right_motor, MOTOR_FORWARD, speed);
}

void tb6612fng_backward(DualMotorDriver *driver, MotorSpeed speed) {
    tb6612fng_set_motor(&driver->left_motor, MOTOR_BACKWARD, speed);
    tb6612fng_set_motor(&driver->right_motor, MOTOR_BACKWARD, speed);
}

void tb6612fng_turn_left(DualMotorDriver *driver, MotorSpeed speed) {
    tb6612fng_set_motor(&driver->left_motor, MOTOR_FORWARD, speed / 2);
    tb6612fng_set_motor(&driver->right_motor, MOTOR_FORWARD, speed);
}

void tb6612fng_turn_right(DualMotorDriver *driver, MotorSpeed speed) {
    tb6612fng_set_motor(&driver->left_motor, MOTOR_FORWARD, speed);
    tb6612fng_set_motor(&driver->right_motor, MOTOR_FORWARD, speed / 2);
}

void tb6612fng_rotate_left(DualMotorDriver *driver, MotorSpeed speed) {
    tb6612fng_set_motor(&driver->left_motor, MOTOR_BACKWARD, speed);
    tb6612fng_set_motor(&driver->right_motor, MOTOR_FORWARD, speed);
}

void tb6612fng_rotate_right(DualMotorDriver *driver, MotorSpeed speed) {
    tb6612fng_set_motor(&driver->left_motor, MOTOR_FORWARD, speed);
    tb6612fng_set_motor(&driver->right_motor, MOTOR_BACKWARD, speed);
}

void tb6612fng_stop(DualMotorDriver *driver) {
    tb6612fng_set_motor(&driver->left_motor, MOTOR_STOP, 0);
    tb6612fng_set_motor(&driver->right_motor, MOTOR_STOP, 0);
}

void tb6612fng_set_speed(DualMotorDriver *driver, MotorSpeed left_speed, MotorSpeed right_speed) {
    pwm_set_chan_level(driver->left_motor.pwm_slice, driver->left_motor.pwm_channel, left_speed);
    pwm_set_chan_level(driver->right_motor.pwm_slice, driver->right_motor.pwm_channel, right_speed);
}

int linearly_map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) / (in_max - in_min) * (out_max - out_min)  + out_min;
}