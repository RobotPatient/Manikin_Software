#include <sensors/sensor_differentialpressure.hpp>

void DifferentialPressureSensor::Initialize() {}

SensorData DifferentialPressureSensor::GetSensorData() {
  sensor_data_.numOfBytes = 1;
  memset(sensor_data_.buffer, 0, sensor_data_.numOfBytes);
  return sensor_data_;
}

void DifferentialPressureSensor::Uninitialize() {}

