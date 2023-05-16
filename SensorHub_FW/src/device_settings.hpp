#ifndef DEVICE_SETTINGS_HPP_
#define DEVICE_SETTINGS_HPP_
#ifdef __arm__
#include <Adafruit_SPIFlash.h>
#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <DeviceManager.hpp>
#include <Status.hpp>
#include <gpio.hpp>
#include <i2c_helper.hpp>
#include <measurement_grabber.hpp>
#include <sensor_helper.hpp>
#include "Wire.h"
#include "wiring_private.h"

// i2c system bus
inline constexpr uint8_t kW0_SDA = 26;  // PA22
inline constexpr uint8_t kW0_SCL = 27;  // PA23

inline constexpr uint8_t kW1_SDA = 28;  // PA12
inline constexpr uint8_t kW1_SCL = 39;  // PA13

inline constexpr uint8_t kW2_SDA = 11;  // PA16
inline constexpr uint8_t kW2_SCL = 13;  // PA17

inline constexpr uint8_t kSpiFramMisoPin = 2;
inline constexpr uint8_t kSpiFramMosiPin = 4;
inline constexpr uint8_t kSpiFramClkPin = 3;
inline constexpr uint8_t kSpiFramSSPin = 9;

TwoWire wireBackbone(&sercom3, kW0_SDA, kW0_SCL);  // Main
TwoWire wireSensorA(&sercom1, kW1_SDA, kW1_SCL);   // Sensor A
TwoWire wireSensorB(&sercom4, kW2_SDA, kW2_SCL);   // Sensor B

I2CDriver i2c_handle_port_a = I2CDriver(&wireSensorA, kI2cSpeed_400KHz);
I2CDriver i2c_handle_port_b = I2CDriver(&wireSensorB, kI2cSpeed_100KHz);

void InitI2CPins() {
  pinPeripheral(kW0_SDA, PIO_SERCOM);
  pinPeripheral(kW0_SCL, PIO_SERCOM);

  pinPeripheral(kW1_SDA, PIO_SERCOM_ALT);
  pinPeripheral(kW1_SCL, PIO_SERCOM_ALT);

  pinPeripheral(kW2_SDA, PIO_SERCOM);
  pinPeripheral(kW2_SCL, PIO_SERCOM);
}

static DeviceManager PortAMgr;
static DeviceManager PortBMgr;
static Status DevStatus;

Logger* StatusLoggerInst;
static LoggerSettings StatusLoggerSettings;

Logger* ExceptionLoggerInst;
static LoggerSettings ExceptionLoggerSettings;

void InitSerialExceptionLogger() {
  ExceptionLoggerSettings.CommHandle.SerialHandle = &Serial;
  ExceptionLoggerSettings.CommMethod = communicationMethod::Serial;
  ExceptionLoggerInst = new SerialLogger(&ExceptionLoggerSettings);
  ExceptionLoggerInst->init();
}

Adafruit_FlashTransport_SPI flashTransport(kSpiFramSSPin, &SPI);
Adafruit_SPIFlash flash(&flashTransport);
FatVolume fatfs;

static const SPIFlash_Device_t my_flash_devices[] = {MB85RS2MTA};

static char FlashLoggerFilepath[kMaxFilePathSize];
const char* kLoggerFilePathPrefix = "/LOG/";

void InitFlashMem() {
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
  strcat(FlashLoggerFilepath, kLoggerFilePathPrefix);
  strcat(FlashLoggerFilepath, filename);
  ExceptionLoggerSettings.CommHandle.FlashHandle.FatHandle = &fatfs;
  ExceptionLoggerSettings.CommHandle.FlashHandle.FilePath = FlashLoggerFilepath;
  ExceptionLoggerSettings.CommMethod = communicationMethod::Flash;
  ExceptionLoggerInst= new FlashLogger(&StatusLoggerSettings);
  ExceptionLoggerInst->init();
}

void InitStatusLogger(const char* filename) {
  strcat(FlashLoggerFilepath, kLoggerFilePathPrefix);
  strcat(FlashLoggerFilepath, filename);
  StatusLoggerSettings.CommHandle.FlashHandle.FatHandle = &fatfs;
  StatusLoggerSettings.CommHandle.FlashHandle.FilePath = FlashLoggerFilepath;
  StatusLoggerSettings.CommMethod = communicationMethod::Flash;
  StatusLoggerInst= new FlashLogger(&StatusLoggerSettings);
  StatusLoggerInst->init();
}

/* The queue is to be created to hold a maximum of 10 uint64_t
variables. */
#define QUEUE_LENGTH 5
#define ITEM_SIZE sizeof(SensorData)
uint8_t ucQueueStorageArea[QUEUE_LENGTH * ITEM_SIZE];

/* The variable used to hold the queue's data structure. */
static StaticQueue_t StaticServiceProtocolQueueStruct;
QueueHandle_t ServiceProtocolQueue;

static CompressionSensor CompressionSens1;
static DifferentialPressureSensor DiffSensor1;
static FingerPositionSensor FingerposSensor1;

UniversalSensor* Sensors_objPool1[4] = {NULL, &CompressionSens1, &DiffSensor1,
                                        &FingerposSensor1};

static CompressionSensor CompressionSens2;
static DifferentialPressureSensor DiffSensor2;
static FingerPositionSensor FingerposSensor2;

UniversalSensor* Sensors_objPool2[4] = {NULL, &CompressionSens2, &DiffSensor2,
                                        &FingerposSensor2};

#endif
#endif  // DEVICE_SETTINGS_HPP_
