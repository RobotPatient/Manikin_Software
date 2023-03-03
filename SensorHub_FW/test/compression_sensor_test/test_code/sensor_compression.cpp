#include "./sensor_compression.hpp"

void CompressionSensor_test::Initialize() {
  Tof_->init();
  Tof_->VL6180xDefautSettings();
}

SensorData CompressionSensor_test::GetSensorData() {
  uint8_t distance = Tof_->getDistance();
  sensor_data_.numOfBytes = 1;
  sensor_data_.buffer[0] = distance;
  return sensor_data_;
}

void CompressionSensor_test::Uninitialize() {}

