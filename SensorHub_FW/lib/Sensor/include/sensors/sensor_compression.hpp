#ifndef SENSOR_COMPRESSION_HPP
#define SENSOR_COMPRESSION_HPP

#include <sensors/sensor_base.hpp>
#include <RobotPatient_VL6180x.h>

class CompressionSensor : public UniversalSensor {
 public:
  explicit CompressionSensor(i2c_peripheral_t i2c_peripheral)
                              : UniversalSensor(i2c_peripheral) {
    i2c_handle_ = new I2CDriver(i2c_peripheral, i2c_speed_400KHz, kSensorI2CAddress_);
    Tof_ = new RobotPatient_VL6180x(i2c_handle_);
  }
  void Initialize() override;
  SensorData GetSensorData() override;
  void Uninitialize() override;
  ~CompressionSensor() {
    Uninitialize();
  }
 private:
  const uint8_t kSensorI2CAddress_ = 0x29;
  SensorData sensor_data_ {};
  I2CDriver* i2c_handle_;
  RobotPatient_VL6180x *Tof_;
};
#endif  // SENSOR_COMPRESSION_HPP
