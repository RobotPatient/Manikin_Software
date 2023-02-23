#ifndef SENSOR_HELPER_H
#define SENSOR_HELPER_H

#include <sensors/sensor_base.hpp>
#include <sensors/sensor_compression.hpp>
#include <sensors/sensor_differentialpressure.hpp>
#include <sensors/sensor_fingerposition.hpp>

enum class SensorTypes {
  kCompressionSensor, kDifferentialPressureSensor, kFingerPositionSensor
};

class Sensor {
 public:
  explicit Sensor(i2c_peripheral_t i2c_peripheral) :
      peripheral_(i2c_peripheral) {
    finger_position_sensor_ = new FingerPositionSensor(peripheral_);
    differential_pressure_sensor_ = new DifferentialPressureSensor(peripheral_);
    compression_sensor_ = new CompressionSensor(peripheral_);
  }
  void SetSensorType(SensorTypes sensor_type) {
    /* When switching from one sensor to another,
     * the current sensor needs to be uninitialized */
    if (current_sensor_ != nullptr) current_sensor_->Uninitialize();
    switch (sensor_type) {
      case SensorTypes::kCompressionSensor:
        current_sensor_ = compression_sensor_;
        break;
      case SensorTypes::kDifferentialPressureSensor:
        current_sensor_ = differential_pressure_sensor_;
        break;
      case SensorTypes::kFingerPositionSensor:
        current_sensor_ = finger_position_sensor_;
        break;
    }
    current_sensor_->Initialize();
  }
 private:
  i2c_peripheral_t peripheral_;
  FingerPositionSensor *finger_position_sensor_;
  DifferentialPressureSensor *differential_pressure_sensor_;
  CompressionSensor *compression_sensor_;
  UniversalSensor *current_sensor_ = nullptr;

};

#endif  // SENSOR_HELPER_H
