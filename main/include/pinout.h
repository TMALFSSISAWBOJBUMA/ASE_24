#ifndef __PINOUT_H__
#define __PINOUT_H__

// https://www.upesy.com/blogs/tutorials/esp32-pinout-reference-gpio-pins-ultimate-guide#

// front ultrasonic
#define TRIGGER_PIN 13
#define ECHO_PIN 12

// dht11 sensor
#define DHT_PIN 21

// motors
#define LEFT_FWD_PIN 32
#define LEFT_REV_PIN 33
#define RIGHT_FWD_PIN 25
#define RIGHT_REV_PIN 26

// IR obstacle sensors
#define IR_FRONT_L 19
#define IR_FRONT_R 18
#define IR_SIDE_L 21
#define IR_SIDE_R 22

#endif
