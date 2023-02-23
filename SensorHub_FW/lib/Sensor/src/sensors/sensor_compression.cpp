#include <sensors/sensor_compression.hpp>

void CompressionSensor::Initialize() {}

SensorData CompressionSensor::GetSensorData() {
  sensor_data_.numOfBytes = 1;
  memset(sensor_data_.buffer, 0, sensor_data_.numOfBytes);
  return sensor_data_;
}

void CompressionSensor::Uninitialize() {}

