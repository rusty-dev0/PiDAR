#include <stdio.h>
#include "pico/stdlib.h"
#include "TB6612FNG.h"

#define LEFT_IN1   2
#define LEFT_IN2   3
#define LEFT_PWM   4
#define RIGHT_IN1  5
#define RIGHT_IN2  6
#define RIGHT_PWM  7

DualMotorDriver* setup()
{
    stdio_init_all();

    printf("[in setup()] setting up motor controller\n");

    DualMotorDriver controller;
    tb6612fnd_init(&controller, LEFT_IN1, LEFT_IN2, LEFT_PWM, RIGHT_IN1, RIGHT_IN2, RIGHT_PWM);

    printf("[in setup()] motor controller sucessfully initialized\n");
    return &controller;
}

int main()
{
    DualMotorDriver* controller = setup();

    while (1)
    {
        printf("[in main()] moving forward\n");
        tb6612fng_forward(controller, 32768);
        sleep_ms(2000);

        printf("[in main()] moving backward\n");
        tb6612fng_backward(controller, 32768);
        sleep_ms(2000);

        printf("[in main()] turning left\n");
        tb6612fng_turn_left(controller, 32768);
        sleep_ms(2000);

        printf("[in main()] turning right\n");
        tb6612fng_turn_right(controller, 32768);
        sleep_ms(2000);

        printf("[in main()] stopping\n");
        tb6612fng_stop(controller);
        sleep_ms(2000);
    }

}
