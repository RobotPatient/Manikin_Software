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
#include <sensor_helper.hpp>
#include "Wire.h"
#include "wiring_private.h"

// i2c system bus
inline constexpr uint8_t kNumOfI2CPorts = 3;
inline constexpr uint8_t kW0_SDA = 26;  // PA22
inline constexpr uint8_t kW0_SCL = 27;  // PA23

inline constexpr uint8_t kW1_SDA = 28;  // PA12
inline constexpr uint8_t kW1_SCL = 39;  // PA13

inline constexpr uint8_t kW2_SDA = 11;  // PA16
inline constexpr uint8_t kW2_SCL = 13;  // PA17

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

/**
 * @brief Classes used by communication modules (service protocol and i2c slave)
 *        These are interfaces to get sensors, set sensortype and certain
 *        measuring characteristics (like sample rate).
 */
static DeviceProperties portAProperties;
static DeviceProperties portBProperties;
static module::status::DeviceStatus systemStatus;

/**
 * @brief These classes are used for logging, the medium can either be Serial or
 *        flash. One is used by the exception module the other one is used by the 
 *        Status module.
 */
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

/**
 * @brief Constants for creating the message queue from the sensor (grabbers)
 *        to the usb service protocol
 */
inline constexpr uint8_t kServiceProtocolQueueLength =  5;
inline constexpr uint8_t kServiceProtocolQueueItemSize = sizeof(SensorData);

/**
 * @brief To prevent FreeRTOS from dynamically creating the queue's, a static
 *         QueueHandle and space for the Queue storage area is defined here
 *
 * @note If FreeRTOS would dynamically allocate the queue area it would put reserve
 *       significant RAM space for the HEAP pool! Making it harder to do predictions
 *       on the memory footprint of the code.
 */
uint8_t serviceProtocolQueueStorageArea[kServiceProtocolQueueLength * kServiceProtocolQueueItemSize];
static StaticQueue_t staticServiceProtocolQueueStruct;
QueueHandle_t serviceProtocolQueue;

/**
 * @brief The DeviceProperties classes have a feature to switch between SensorTypes
 *        at runtime. Doing this dynamically can cause undefined behaviour and
 *        makes it difficult to predict memory footprint. Therefore static objectpool were
 *        created
 *
*/
inline constexpr uint8_t kNumOfSensorTypes = 4;
static CompressionSensor compressionSens1;
static DifferentialPressureSensor ventilationSensor1;
static FingerPositionSensor fingerPositionSensor1;

UniversalSensor* sensorsObjPool1[kNumOfSensorTypes] = {NULL, &compressionSens1, &ventilationSensor1,
                                                      &fingerPositionSensor1};

static CompressionSensor compressionSensor2;
static DifferentialPressureSensor ventilationSensor2;
static FingerPositionSensor fingerPositionSensor2;

UniversalSensor* sensorsObjPool2[kNumOfSensorTypes] = {NULL, &compressionSensor2, &ventilationSensor2,
                                                      &fingerPositionSensor2};

#endif
#endif  // DEVICE_SETTINGS_HPP_
