#ifndef SENSORHUB_FW_SRC_SENSORHUB_SETTINGS_HPP_
#define SENSORHUB_FW_SRC_SENSORHUB_SETTINGS_HPP_

#ifdef __arm__
#include "wiring_private.h"
#include "Wire.h"

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


void InitI2CPins() {
  pinPeripheral(kW0_SDA, PIO_SERCOM);
  pinPeripheral(kW0_SCL, PIO_SERCOM);

  pinPeripheral(kW2_SDA, PIO_SERCOM);
  pinPeripheral(kW2_SCL, PIO_SERCOM);

  pinPeripheral(kW1_SDA, PIO_SERCOM_ALT);
  pinPeripheral(kW1_SCL, PIO_SERCOM_ALT);
}
#endif
#endif  // SENSORHUB_FW_SRC_SENSORHUB_SETTINGS_HPP_
