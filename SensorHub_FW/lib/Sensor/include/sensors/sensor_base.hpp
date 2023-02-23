#ifndef SENSOR_BASE_H
#define SENSOR_BASE_H

#include <SDP810.h>
#include <RobotPatient_VL6180x.h>
#include <i2c_helper.h>

typedef struct {
  uint16_t buffer[8];
  uint8_t numOfBytes;
} SensorData;

class UniversalSensor {
 public:
  explicit UniversalSensor(i2c_peripheral_t i2c_peripheral) {}
  virtual void Initialize() = 0;
  virtual SensorData GetSensorData() = 0;
  virtual void Uninitialize() = 0;
 private:
  i2c_peripheral_t peripheral_;
};

#endif  // SENSOR_BASE_H
