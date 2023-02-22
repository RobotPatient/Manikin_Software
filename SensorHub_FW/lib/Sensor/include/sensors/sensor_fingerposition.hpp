#ifndef SENSOR_FINGERPOSITION_H
#define SENSOR_FINGERPOSITION_H

#include <sensors/sensor_base.hpp>
#include "../../../ADS7138/include/ads7138.hpp"
class fingerPositionSensor : public universal_sensor {
 public:
  explicit fingerPositionSensor(i2c_peripheral_t i2c_peripheral)
                                : universal_sensor(i2c_peripheral) {
    i2c_handle = new i2c_driver(i2c_peripheral, i2c_speed_400KHz, i2c_addr);
    ADS = new ads7138(i2c_handle);
  }
  void initialize() override;
  SensorData getSensorData() override;
  void deinitialize() override;
  ~fingerPositionSensor() {
    deinitialize();
  }
 private:
  i2c_driver* i2c_handle;
  SensorData sensData{};
  const uint8_t i2c_addr = 0x29;
  ads7138* ADS;
};
#endif  // SENSOR_FINGERPOSITION_H
