#include <sensors/sensor_differentialpressure.hpp>

void DifferentialPressureSensor::Initialize() {
  sdp810_handle_->begin();
}

SensorData DifferentialPressureSensor::GetSensorData() {
  sdp810_handle_->read();
  sensor_data_.numOfBytes = 2;
  sensor_data_.buffer[0] = sdp810_handle_->getRaw();
  return sensor_data_;
}

void DifferentialPressureSensor::Uninitialize() {}

