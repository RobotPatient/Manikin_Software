#ifndef SENSORHUB_FW_SRC_SENSORHUB_SETTINGS_HPP_
#define SENSORHUB_FW_SRC_SENSORHUB_SETTINGS_HPP_

#ifdef __arm__
#include <Arduino.h>
#include "wiring_private.h"
#include "Wire.h"
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>
#include <i2c_helper.hpp>
#include <hal_log.hpp>

using namespace hal::log;

// i2c system bus
const uint8_t kW0_SCL = 27;  // PA22
const uint8_t kW0_SDA = 26;  // PA23

const uint8_t kW1_SCL = 39;  // PA13
const uint8_t kW1_SDA = 28;  // PA12

const uint8_t kW2_SCL = 13;  // PA17
const uint8_t kW2_SDA = 11;  // PA16

TwoWire wireBackbone(&sercom3, kW0_SDA, kW0_SCL);  // Main
TwoWire wireSensorA(&sercom1, kW1_SDA, kW1_SCL);  // Sensor A
TwoWire wireSensorB(&sercom4, kW2_SDA, kW2_SCL);  // Sensor B

I2CDriver i2c_handle_port_a = I2CDriver(&wireSensorA, kI2cSpeed_100KHz);
I2CDriver i2c_handle_port_b = I2CDriver(&wireSensorB, kI2cSpeed_100KHz);

inline constexpr uint8_t kSpiFramMisoPin = 2;
inline constexpr uint8_t kSpiFramMosiPin = 4;
inline constexpr uint8_t kSpiFramClkPin = 3;
inline constexpr uint8_t kSpiFramSSPin = 9;

Adafruit_FlashTransport_SPI flashTransport(kSpiFramSSPin, &SPI1);
Adafruit_SPIFlash flash(&flashTransport);


void InitI2CPins() {
  pinPeripheral(kW0_SDA, PIO_SERCOM);
  pinPeripheral(kW0_SCL, PIO_SERCOM);

  pinPeripheral(kW2_SDA, PIO_SERCOM);
  pinPeripheral(kW2_SCL, PIO_SERCOM);

  pinPeripheral(kW1_SDA, PIO_SERCOM_ALT);
  pinPeripheral(kW1_SCL, PIO_SERCOM_ALT);
}

void InitSerialLogger(Logger* log_inst, LoggerSettings *log_settings, Serial_* serial_obj) {
  log_settings->CommHandle.SerialHandle = serial_obj;
  log_settings->CommMethod = communicationMethod::Serial;
  log_inst = new SerialLogger(log_settings);
}

FatVolume fatfs;

static const SPIFlash_Device_t my_flash_devices[] = {
    MB85RS2MTA
};

static char FlashLoggerFilepath[kMaxFilePathSize];
const char* kLoggerFilePathPrefix = "/LOG/";

void InitFlashLogger(Logger* log_inst, LoggerSettings *log_settings, const char* filename) {
   if (!flash.begin(my_flash_devices, 1)) {
    Serial.println(F("Error, failed to initialize flash chip!"));
    while(1) yield();
  }
  
  if (!fatfs.begin(&flash)) {
    Serial.println(F("Error, failed to mount newly formatted filesystem!"));
    Serial.println(F("Was the flash chip formatted with the SdFat_format example?"));
    while(1) yield();
  };

  if (!fatfs.exists("/LOG")) {
    Serial.println(F("LOG directory not found, creating..."));
    fatfs.mkdir("/LOG");
  }

  strcat(FlashLoggerFilepath, kLoggerFilePathPrefix);
  strcat(FlashLoggerFilepath, filename);
  log_settings->CommHandle.FlashHandle.FatHandle = &fatfs;
  log_settings->CommHandle.FlashHandle.FilePath = FlashLoggerFilepath;
  log_settings->CommMethod = communicationMethod::Flash;
  log_inst = new FlashLogger(log_settings);
  log_inst->init();
  // logflash->init();
  // logflash->setcursorpos(0);
  // File32* flash_handle = logflash->getnativehandle().FlashHandle;

  // uint32_t filesize = flash_handle->size();
  // Serial.printf("Size: %d \n",filesize);
}


#endif
#endif  // SENSORHUB_FW_SRC_SENSORHUB_SETTINGS_HPP_
