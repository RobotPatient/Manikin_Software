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
  explicit Sensor(i2c_peripheral_t i2c_peripheral);
  void SetSensorType(SensorTypes sensor_type);
  const SensorData GetSensorData();
 private:
  i2c_peripheral_t peripheral_;
  FingerPositionSensor *finger_position_sensor_ = nullptr;
  DifferentialPressureSensor *differential_pressure_sensor_ = nullptr;
  CompressionSensor *compression_sensor_ = nullptr;
  UniversalSensor *current_sensor_ = nullptr;
};

#endif  // SENSOR_HELPER_H
