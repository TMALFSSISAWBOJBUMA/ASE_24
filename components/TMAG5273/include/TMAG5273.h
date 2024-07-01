/******************************************************************************
Based on SparkFun_TMAG5273_Arduino_Library.h
https://github.com/sparkfun/SparkFun_TMAG5273_Arduino_Library

This header file implements all functions of the TMAG5273. Functions here range
from reading the X, Y,and Z Magnetic Fields, to reading and writing various
settings in the sensor.
******************************************************************************/

#ifndef __TMAG5273_H__
#define __TMAG5273_H__

#include <stdint.h>
#include <esp_err.h>
#include <esp_check.h>
#include "driver/i2c_master.h"
#include "TMAG5273_Library_Defs.h"

typedef struct
{
    i2c_master_dev_handle_t self;
    i2c_master_bus_handle_t bus;
    uint8_t deviceAddress;
} TMAG5273_device_t;

typedef TMAG5273_device_t *TMAG5273_device_handle_t;

esp_err_t TMAG5273_init(uint8_t sensorAddress, i2c_master_bus_handle_t bus, TMAG5273_device_handle_t sensor);
int8_t TMAG5273_isConnected(TMAG5273_device_handle_t sensor);
int8_t TMAG5273_setupWakeUpAndSleep(TMAG5273_device_handle_t sensor);
int8_t TMAG5273_readWakeUpAndSleepData(TMAG5273_device_handle_t sensor, float *xVal, float *yVal, float *zVal, float *temperature);

int8_t TMAG5273_setCRCMode(TMAG5273_device_handle_t sensor, uint8_t crcMode);
int8_t TMAG5273_setMagTemp(TMAG5273_device_handle_t sensor, uint8_t magTempMode);
int8_t TMAG5273_setConvAvg(TMAG5273_device_handle_t sensor, uint8_t avgMode);
int8_t TMAG5273_setReadMode(TMAG5273_device_handle_t sensor, uint8_t readMode);
int8_t TMAG5273_setIntThreshold(TMAG5273_device_handle_t sensor, uint8_t threshold);
int8_t TMAG5273_setLowPower(TMAG5273_device_handle_t sensor, uint8_t lpLnMode);
int8_t TMAG5273_setGlitchFilter(TMAG5273_device_handle_t sensor, uint8_t glitchMode);
int8_t TMAG5273_setTriggerMode(TMAG5273_device_handle_t sensor, uint8_t trigMode);
int8_t TMAG5273_setOperatingMode(TMAG5273_device_handle_t sensor, uint8_t opMode);
int8_t TMAG5273_setMagneticChannel(TMAG5273_device_handle_t sensor, uint8_t channelMode);
int8_t TMAG5273_setSleeptime(TMAG5273_device_handle_t sensor, uint8_t sleepTime);
int8_t TMAG5273_setMagDir(TMAG5273_device_handle_t sensor, uint8_t threshDir);
int8_t TMAG5273_setMagnitudeGain(TMAG5273_device_handle_t sensor, uint8_t gainAdjust);
int8_t TMAG5273_setMagneticGain(TMAG5273_device_handle_t sensor, float magneticGain);
int8_t TMAG5273_setMagneticOffset1(TMAG5273_device_handle_t sensor, float offset1);
int8_t TMAG5273_setMagneticOffset2(TMAG5273_device_handle_t sensor, float offset2);
int8_t TMAG5273_setAngleEn(TMAG5273_device_handle_t sensor, uint8_t angleEnable);
int8_t TMAG5273_setXYAxisRange(TMAG5273_device_handle_t sensor, uint8_t xyAxisRange);
int8_t TMAG5273_setZAxisRange(TMAG5273_device_handle_t sensor, uint8_t zAxisRange);
int8_t TMAG5273_setXThreshold(TMAG5273_device_handle_t sensor, float xThreshold);
int8_t TMAG5273_setYThreshold(TMAG5273_device_handle_t sensor, float yThreshold);
int8_t TMAG5273_setZThreshold(TMAG5273_device_handle_t sensor, float zThresh);
int8_t TMAG5273_setTemperatureThreshold(TMAG5273_device_handle_t sensor, int8_t tempThresh);
int8_t TMAG5273_setTemperatureEn(TMAG5273_device_handle_t sensor, bool temperatureEnable);
int8_t TMAG5273_setInterruptResult(TMAG5273_device_handle_t sensor, bool interruptEnable);
int8_t TMAG5273_setThresholdEn(TMAG5273_device_handle_t sensor, bool enableInterruptResponse);
int8_t TMAG5273_setIntPinState(TMAG5273_device_handle_t sensor, bool interruptState);
int8_t TMAG5273_setInterruptMode(TMAG5273_device_handle_t sensor, uint8_t configurationMode);
int8_t TMAG5273_setMaskInterrupt(TMAG5273_device_handle_t sensor, bool interruptPinEnable);
int8_t TMAG5273_setI2CAddress(TMAG5273_device_handle_t sensor, uint8_t address);
int8_t TMAG5273_setI2CAddressEN(TMAG5273_device_handle_t sensor, bool addressEnable);
int8_t TMAG5273_setOscillatorError(TMAG5273_device_handle_t sensor, bool oscError);

uint8_t TMAG5273_getCRCMode(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getMagTemp(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getConvAvg(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getReadMode(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getIntThreshold(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getLowPower(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getGlitchFiler(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getTriggerMode(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getOperatingMode(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getMagneticChannel(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getSleeptime(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getMagDir(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getMagnitudeChannelSelect(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getMagneticGain(TMAG5273_device_handle_t sensor);
int8_t TMAG5273_getMagneticOffset1(TMAG5273_device_handle_t sensor);
int8_t TMAG5273_getMagneticOffset2(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getAngleEn(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getXYAxisRange(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getZAxisRange(TMAG5273_device_handle_t sensor);
float TMAG5273_getXThreshold(TMAG5273_device_handle_t sensor);
float TMAG5273_getYThreshold(TMAG5273_device_handle_t sensor);
float TMAG5273_getZThreshold(TMAG5273_device_handle_t sensor);
float TMAG5273_getTemperatureThreshold(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getTemperatureEN(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getInterruptResult(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getThresholdEn(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getIntPinState(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getInterruptMode(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getMaskInt(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getSetCount(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getPOR(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getDiagStatus(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getResultStatus(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getI2CAddress(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getDeviceID(TMAG5273_device_handle_t sensor);
uint16_t TMAG5273_getManufacturerID(TMAG5273_device_handle_t sensor);

float TMAG5273_getTemp(TMAG5273_device_handle_t sensor);
float TMAG5273_getXData(TMAG5273_device_handle_t sensor);
float TMAG5273_getYData(TMAG5273_device_handle_t sensor);
float TMAG5273_getZData(TMAG5273_device_handle_t sensor);
float TMAG5273_getAngleResult(TMAG5273_device_handle_t sensor);
float TMAG5273_getMagnitudeResult(TMAG5273_device_handle_t sensor);

uint8_t TMAG5273_getInterruptPinStatus(TMAG5273_device_handle_t sensor);
uint8_t TMAG5273_getDeviceStatus(TMAG5273_device_handle_t sensor);
int8_t TMAG5273_getError(TMAG5273_device_handle_t sensor);

#endif