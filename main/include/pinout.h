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
#define IR_FRONT 19
#define IR_SIDE_L 36
#define IR_SIDE_R 39
#define IR_BACK 34

// tape sensors
#define IR_TAPE 18

// speed feedback photocouplers
#define SPEED_L 13
#define SPEED_R 14

#endif
