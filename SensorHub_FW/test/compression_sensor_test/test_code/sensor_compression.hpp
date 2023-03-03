#ifndef SENSOR_COMPRESSION_HPP
#define SENSOR_COMPRESSION_HPP

#include "sensor_base.hpp"
#include "../compression_sen_testclass.hpp"



class CompressionSensor_test {
 public:
  // explicit CompressionSensor(i2c_peripheral_t i2c_peripheral) {
  //   // i2c_handle_ = new I2CDriver_mock(i2c_peripheral, ki2cSpeed_400KHz, kSensorI2CAddress_);
  //   Tof_ = new RobotPatient_VL6180x_mock();
  // }

  CompressionSensor_test(RobotPatient_VL6180x_mock* tof) : Tof_(tof) {}

  void Initialize();
  SensorData GetSensorData();
  void Uninitialize();
  ~CompressionSensor_test() {
    Uninitialize();
  }
 private:
  const uint8_t kSensorI2CAddress_ = 0x29;
  SensorData sensor_data_{};
  I2CDriver_mock *i2c_handle_;
  RobotPatient_VL6180x_mock *Tof_;
};
#endif  // SENSOR_COMPRESSION_HPP
