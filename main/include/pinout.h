#ifndef __PINOUT_H__
#define __PINOUT_H__

// https://www.upesy.com/blogs/tutorials/esp32-pinout-reference-gpio-pins-ultimate-guide#

// front ultrasonic
// #define TRIGGER_PIN 13
// #define ECHO_PIN 14

#define BUZZER_PIN 23

#define SERVO_PIN 27

// I2C for hall and IMU
#define SDA_PIN 21
#define SCL_PIN 22

// motors
#define LEFT_FWD_PIN 32
#define LEFT_REV_PIN 33
#define RIGHT_FWD_PIN 26
#define RIGHT_REV_PIN 25

// IR obstacle sensors
#define IR_FRONT_R 17
#define IR_FRONT_L 5
#define IR_SIDE_LF 16
#define IR_SIDE_LB 4
#define IR_SIDE_RF 35
#define IR_SIDE_RB 39
#define IR_BACK 34

// tape sensors
#define IR_TAPE_L 18
#define IR_TAPE_R 19

// speed feedback photocouplers
#define SPEED_L 13
#define SPEED_R 14

#endif
