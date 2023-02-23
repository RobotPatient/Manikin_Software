#include <sensors/sensor_fingerposition.hpp>

void FingerPositionSensor::Initialize() {}

SensorData FingerPositionSensor::GetSensorData() {
  sensor_data_.numOfBytes = 1;
  memset(sensor_data_.buffer, 0, sensor_data_.numOfBytes);
  return sensor_data_;
}

void FingerPositionSensor::Uninitialize() {}

