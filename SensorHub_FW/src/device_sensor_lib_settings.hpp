#ifndef DEVICE_SENSOR_LIB_SETTINGS_HPP
#define DEVICE_SENSOR_LIB_SETTINGS_HPP
#include <stdint.h>

/**
 * @brief The number of sensors supported!
 * @note This needs to be changed when a new sensors is added!
*/
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