#ifndef SENSOR_DIFFERENTIALPRESSURE_H
#define SENSOR_DIFFERENTIALPRESSURE_H

#include <sensors/sensor_base.hpp>
#include <SDP810.h>

class DifferentialPressureSensor : public UniversalSensor {
 public:
  explicit DifferentialPressureSensor(i2c_peripheral_t i2c_peripheral)
      : UniversalSensor(i2c_peripheral) {
    i2c_handle_ = new I2CDriver(i2c_peripheral, i2c_speed_400KHz, kSensorI2CAddress_);
    sdp810_handle_ = new SDP810(i2c_handle_);
  }
  void Initialize() override;
  SensorData GetSensorData() override;
  void Uninitialize() override;
  ~DifferentialPressureSensor() {
    Uninitialize();
  }
 private:
  const uint8_t kSensorI2CAddress_ = 0x25;
  I2CDriver *i2c_handle_;
  SDP810 *sdp810_handle_;
  SensorData sensor_data_{};
};
#endif  // SENSOR_DIFFERENTIALPRESSURE_H
