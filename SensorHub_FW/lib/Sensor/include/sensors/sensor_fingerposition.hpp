#ifndef SENSOR_FINGERPOSITION_H
#define SENSOR_FINGERPOSITION_H

#include <sensors/sensor_base.hpp>
#include "ADS7138.hpp"
class FingerPositionSensor : public UniversalSensor {
 public:
  explicit FingerPositionSensor(i2c_peripheral_t i2c_peripheral)
      : UniversalSensor(i2c_peripheral) {
    i2c_handle_ = new I2CDriver(i2c_peripheral, i2c_speed_400KHz, kSensorI2CAddress_);
    ads7138_handle_ = new ADS7138(i2c_handle_);
  }
  void Initialize() override;
  SensorData GetSensorData() override;
  void Uninitialize() override;
  ~FingerPositionSensor() {
    Uninitialize();
  }
 private:
  const uint8_t kSensorI2CAddress_ = 0x10;
  I2CDriver *i2c_handle_;
  SensorData sensor_data_{};
  ADS7138 *ads7138_handle_;
};
#endif  // SENSOR_FINGERPOSITION_H
