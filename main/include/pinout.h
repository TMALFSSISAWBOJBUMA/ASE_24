#ifndef __PINOUT_H__
#define __PINOUT_H__

// https://www.upesy.com/blogs/tutorials/esp32-pinout-reference-gpio-pins-ultimate-guide#

// front ultrasonic
#define TRIGGER_PIN 22
#define ECHO_PIN 23

// dht11 sensor
#define DHT_PIN 21

// motors
#define LEFT_FWD_PIN 32
#define LEFT_REV_PIN 33
#define RIGHT_FWD_PIN 25
#define RIGHT_REV_PIN 26

// IR obstacle sensors
#define IR_FRONT_L 36
#define IR_FRONT_R 39
#define IR_SIDE_L 34
#define IR_SIDE_R 35

#endif