#ifndef DEVICE_SENSOR_LIB_SETTINGS_HPP
#define DEVICE_SENSOR_LIB_SETTINGS_HPP
#include <stdint.h>

inline constexpr uint8_t kNumOfSupportedSensors = 4;

/**
 * @brief The currently supported sensors
*/
typedef enum {
  TypeNone,
  TypeCompressionSensor,
  TypeDifferentialPressureSensor,
  TypefingerPositionSensor
} SensorTypes;


#endif