#include <sensors/sensor_fingerposition.hpp>

void FingerPositionSensor::Initialize() {
  ads7138_handle_->initDefaultRead();
}

SensorData FingerPositionSensor::GetSensorData() {
  const uint8_t kADC_channels_to_read = 8;
  // 12-bit adc, 16-bit variable is the smallest size that fits this 12-bits.
  // 16-bit is 2-bytes therefore the channels times 2
  const uint8_t kAmount_of_bytes_to_read = 2*kADC_channels_to_read;
  sensor_data_.numOfBytes = kAmount_of_bytes_to_read;
  ads7138_handle_->readADC(sensor_data_.buffer);
  return sensor_data_;
}

void FingerPositionSensor::Uninitialize() {}

