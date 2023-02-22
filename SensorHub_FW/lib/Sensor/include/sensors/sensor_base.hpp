#ifndef SENSOR_BASE_H
#define SENSOR_BASE_H

#include <SDP810.h>
#include <RobotPatient_VL6180x.h>
#include <i2c_helper.h>


typedef enum {
Sensor_compression, Sensor_differentialpressure, Sensor_fingerposition
}SensorTypes;

typedef struct {
uint16_t buffer[8];
uint8_t numOfBytes;
}SensorData;


class universal_sensor {
 public:
  explicit universal_sensor(i2c_peripheral_t i2c_peripheral) {}
  virtual void initialize() = 0;
  virtual SensorData getSensorData() = 0;
  virtual void deinitialize() = 0;
 private:
  i2c_peripheral_t peripheral;
};

#endif  // SENSOR_BASE_H
