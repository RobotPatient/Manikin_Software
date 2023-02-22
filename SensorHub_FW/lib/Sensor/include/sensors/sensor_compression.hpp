#ifndef SENSOR_COMPRESSION_HPP
#define SENSOR_COMPRESSION_HPP

#include <sensors/sensor_base.hpp>
#include <RobotPatient_VL6180x.h>

class compressionSensor : public universal_sensor {
 public:
  explicit compressionSensor(i2c_peripheral_t i2c_peripheral)
                              : universal_sensor(i2c_peripheral) {
    i2c_handle = new i2c_driver(i2c_peripheral, i2c_speed_400KHz, i2c_addr);
    Tof = new RobotPatient_VL6180x(i2c_handle);
  }
  void initialize() override;
  SensorData getSensorData() override;
  void deinitialize() override;
  ~compressionSensor() {
    deinitialize();
  }
 private:
  SensorData sensData {};
  const uint8_t i2c_addr = 0x29;
  i2c_driver* i2c_handle;
  RobotPatient_VL6180x *Tof;
};
#endif  // SENSOR_COMPRESSION_HPP
