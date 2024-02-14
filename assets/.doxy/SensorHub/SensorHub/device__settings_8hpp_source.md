

# File device\_settings.hpp

[**File List**](files.md) **>** [**SensorHub\_FW**](dir_8a3376b0b822df0b6266211cee305325.md) **>** [**src**](dir_d67209f9e2b4e51eca02397244f6bfa8.md) **>** [**device\_settings.hpp**](device__settings_8hpp.md)

[Go to the documentation of this file](device__settings_8hpp.md)

```C++

/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/
#ifndef DEVICE_SETTINGS_HPP_
#define DEVICE_SETTINGS_HPP_
#ifdef __arm__
#include <Adafruit_SPIFlash.h>
#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <device_properties.hpp>
#include <device_status.hpp>
#include <gpio.hpp>
#include <hal_log.hpp>
#include <i2c_helper.hpp>
#include <measurement_grabber.hpp>
#include <device_sensor_lib_settings.hpp>
#include <sensor_helper.hpp>
#include "Wire.h"
#include "wiring_private.h"

inline constexpr uint8_t kNumOfI2CPorts = 3;
inline constexpr uint8_t kBackBonePortSDA = 26;  // PA22
inline constexpr uint8_t kBackBonePortSCL = 27;  // PA23

inline constexpr uint8_t kSensorPortASDA = 28;  // PA12
inline constexpr uint8_t kSensorPortASCL = 39;  // PA13

inline constexpr uint8_t kSensorPortBSDA = 11;  // PA16
inline constexpr uint8_t kSensorPortBSCL = 13;  // PA17

TwoWire wireBackbone(&sercom3, kBackBonePortSDA, kBackBonePortSCL);  // Main
TwoWire wireSensorA(&sercom1, kSensorPortASDA, kSensorPortASCL);   // Sensor A
TwoWire wireSensorB(&sercom4, kSensorPortBSDA, kSensorPortBSCL);   // Sensor B

I2CDriver i2c_handle_port_a = I2CDriver(&wireSensorA, kI2cSpeed_400KHz);
I2CDriver i2c_handle_port_b = I2CDriver(&wireSensorB, kI2cSpeed_100KHz);

void InitI2CPins() {
  pinPeripheral(kBackBonePortSDA, PIO_SERCOM);
  pinPeripheral(kBackBonePortSCL, PIO_SERCOM);

  pinPeripheral(kSensorPortASDA, PIO_SERCOM_ALT);
  pinPeripheral(kSensorPortASCL, PIO_SERCOM_ALT);

  pinPeripheral(kSensorPortBSDA, PIO_SERCOM);
  pinPeripheral(kSensorPortBSCL, PIO_SERCOM);
}

static xTaskHandle USBServiceProtocolReadTask;
static DeviceProperties portAProperties;
static DeviceProperties portBProperties;
static module::status::DeviceStatus systemStatus;

hal::log::Logger* statusLoggerInst;
static hal::log::LoggerSettings statusLoggerSettings;

hal::log::Logger* exceptionLoggerInst;
static hal::log::LoggerSettings exceptionLoggerSettings;

void InitSerialExceptionLogger() {
  exceptionLoggerSettings.CommHandle.SerialHandle = &Serial;
  exceptionLoggerSettings.CommMethod = hal::log::communicationMethod::Serial;
  exceptionLoggerInst = new hal::log::SerialLogger(&exceptionLoggerSettings);
  exceptionLoggerInst->init();
}

inline constexpr uint8_t kSpiFramSSPin = 9;
Adafruit_FlashTransport_SPI flashTransport(kSpiFramSSPin, &SPI);
Adafruit_SPIFlash flash(&flashTransport);
FatVolume fatfs;
static const SPIFlash_Device_t my_flash_devices[] = {MB85RS2MTA};
static char flashLoggerFilepath[hal::log::kMaxFilePathSize];


constexpr const char* kLoggerFilePathPrefix = "/LOG/";

void InitExternalFlashMemory() {
  if (!flash.begin(my_flash_devices, 1)) {
    Serial.println(F("Error, failed to initialize flash chip!"));
    while (1)
      yield();
  }

  if (!fatfs.begin(&flash)) {
    Serial.println(F("Error, failed to mount newly formatted filesystem!"));
    Serial.println(
        F("Was the flash chip formatted with the SdFat_format example?"));
    while (1)
      yield();
  };
  
  if (!fatfs.exists("/LOG")) {
    Serial.println(F("LOG directory not found, creating..."));
    fatfs.mkdir("/LOG");
  }
}

void InitFlashExceptionLogger(const char* filename){
  strcat(flashLoggerFilepath, kLoggerFilePathPrefix);
  strcat(flashLoggerFilepath, filename);
  exceptionLoggerSettings.CommHandle.FlashHandle.FatHandle = &fatfs;
  exceptionLoggerSettings.CommHandle.FlashHandle.FilePath = flashLoggerFilepath;
  exceptionLoggerSettings.CommMethod = hal::log::communicationMethod::Flash;
  exceptionLoggerInst = new hal::log::FlashLogger(&exceptionLoggerSettings);
  exceptionLoggerInst->init();
}

void InitStatusLogger(const char* filename) {
  strcat(flashLoggerFilepath, kLoggerFilePathPrefix);
  strcat(flashLoggerFilepath, filename);
  statusLoggerSettings.CommHandle.FlashHandle.FatHandle = &fatfs;
  statusLoggerSettings.CommHandle.FlashHandle.FilePath = flashLoggerFilepath;
  statusLoggerSettings.CommMethod = hal::log::communicationMethod::Flash;
  statusLoggerInst = new hal::log::FlashLogger(&statusLoggerSettings);
  statusLoggerInst->init();
}

inline constexpr uint8_t kServiceProtocolQueueLength =  5;
inline constexpr uint8_t kServiceProtocolQueueItemSize = sizeof(SensorData);

uint8_t serviceProtocolQueueStorageArea[kServiceProtocolQueueLength * kServiceProtocolQueueItemSize];
static StaticQueue_t staticServiceProtocolQueueStruct;
QueueHandle_t serviceProtocolQueue;

static CompressionSensor compressionSens1;
static DifferentialPressureSensor ventilationSensor1;
static FingerPositionSensor fingerPositionSensor1;

UniversalSensor* sensorsObjPool1[kNumOfSupportedSensors] = {NULL, &compressionSens1, &ventilationSensor1,
                                                      &fingerPositionSensor1};

static CompressionSensor compressionSensor2;
static DifferentialPressureSensor ventilationSensor2;
static FingerPositionSensor fingerPositionSensor2;

UniversalSensor* sensorsObjPool2[kNumOfSupportedSensors] = {NULL, &compressionSensor2, &ventilationSensor2,
                                                      &fingerPositionSensor2};

#endif
#endif  // DEVICE_SETTINGS_HPP_

```

