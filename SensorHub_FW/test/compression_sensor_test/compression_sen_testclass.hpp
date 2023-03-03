#ifndef COMPRESSION_SEN_TESTCLASS_H
#define COMPRESSION_SEN_TESTCLASS_H

#include <stdint.h>
#include <string.h>
#include "gmock/gmock.h"  // Brings in Google Mock.
#include "./i2c_helper.hpp"

enum vl6180x_als_gain
{ // Data sheet shows gain values as binary list

  GAIN_20 = 0, // Actual ALS Gain of 20
  GAIN_10,     // Actual ALS Gain of 10.32
  GAIN_5,      // Actual ALS Gain of 5.21
  GAIN_2_5,    // Actual ALS Gain of 2.60
  GAIN_1_67,   // Actual ALS Gain of 1.72
  GAIN_1_25,   // Actual ALS Gain of 1.28
  GAIN_1,      // Actual ALS Gain of 1.01
  GAIN_40,     // Actual ALS Gain of 40

};

struct VL6180xIdentification
{
  uint8_t idModel;
  uint8_t idModelRevMajor;
  uint8_t idModelRevMinor;
  uint8_t idModuleRevMajor;
  uint8_t idModuleRevMinor;
  uint16_t idDate;
  uint16_t idTime;
};


class RobotPatient_VL6180x_mock {
 public:
    MOCK_METHOD(uint8_t, init, ());
    MOCK_METHOD(void, VL6180xDefautSettings,());
    MOCK_METHOD(uint8_t, getDistance, ());
    MOCK_METHOD(float, getAmbientLight, (vl6180x_als_gain VL6180X_ALS_GAIN));
    MOCK_METHOD(void, getIdentification, (struct VL6180xIdentification *temp));
    MOCK_METHOD(uint8_t, changeAddress, (uint8_t old_address, uint8_t new_address));
};

#endif // COMPRESSION_SEN_TESTCLASS_H