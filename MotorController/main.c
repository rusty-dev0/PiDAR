#include <stdio.h>
#include "pico/stdlib.h"
#include "TB6612FNG.h"

#define LEFT_IN1   2
#define LEFT_IN2   3
#define LEFT_PWM   4
#define RIGHT_IN1  5
#define RIGHT_IN2  6
#define RIGHT_PWM  7

int main()
{
    stdio_init_all();
    printf("Motor Controller Started\n");

    DualMotorDriver motor_driver;
    tb6612fng_init(&motor_driver, LEFT_IN1, LEFT_IN2, LEFT_PWM, RIGHT_IN1, RIGHT_IN2, RIGHT_PWM);

    printf("Motor driver initialized\n");

    while (true) {
        printf("Moving forward\n");
        tb6612fng_forward(&motor_driver, 32768);  // half speed
        sleep_ms(2000);

        printf("Turning left\n");
        tb6612fng_turn_left(&motor_driver, 32768);
        sleep_ms(1000);

        printf("Rotating right\n");
        tb6612fng_rotate_right(&motor_driver, 32768);
        sleep_ms(1000);

        printf("Stopping!\n");
        tb6612fng_stop(&motor_driver);
        sleep_ms(1000);
    }
}
