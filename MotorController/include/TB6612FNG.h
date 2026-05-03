#ifndef TB6612FNG_H
#define TB6612FNG_H

#include <stdint.h>

typedef struct {
    uint8_t in1;      // direction
    uint8_t in2;      // direction as well
    uint8_t pwm;      // speed
} MotorPins;

typedef struct {
    MotorPins pins;
    uint16_t pwm_slice;
    uint8_t pwm_channel;
    uint16_t max_pwm;
} Motor;

typedef struct {
    Motor left_motor;
    Motor right_motor;
} DualMotorDriver;

typedef enum {
    MOTOR_FORWARD = 0,
    MOTOR_BACKWARD = 1,
    MOTOR_STOP = 2
} MotorDirection;

typedef uint16_t MotorSpeed;

void tb6612fng_init(DualMotorDriver *driver, uint8_t left_in1, uint8_t left_in2, uint8_t left_pwm, uint8_t right_in1, uint8_t right_in2, uint8_t right_pwm);

void tb6612fng_set_motor(Motor *motor, MotorDirection direction, MotorSpeed speed);

void tb6612fng_forward(DualMotorDriver *driver, MotorSpeed speed);
void tb6612fng_backward(DualMotorDriver *driver, MotorSpeed speed);

void tb6612fng_turn_left(DualMotorDriver *driver, MotorSpeed speed);
void tb6612fng_turn_right(DualMotorDriver *driver, MotorSpeed speed);

void tb6612fng_rotate_left(DualMotorDriver *driver, MotorSpeed speed);
void tb6612fng_rotate_right(DualMotorDriver *driver, MotorSpeed speed);

void tb6612fng_stop(DualMotorDriver *driver);

void tb6612fng_set_speed(DualMotorDriver *driver, MotorSpeed left_speed, MotorSpeed right_speed);

int linearly_map(int x, int in_min, int in_max, int out_min, int out_max);

#endif