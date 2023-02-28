#ifndef COMPRESSION_SEN_TESTCLASS_H
#define COMPRESSION_SEN_TESTCLASS_H

#include <stdint.h>
#include <string.h>
#include "gmock/gmock.h"  // Brings in Google Mock.

class CompressionSenTestclass {
 public:
    MOCK_METHOD0(Initialize, void());
    MOCK_METHOD0(GetSensorData, SensorData());
    MOCK_METHOD0(Uninitialize, void());
};

#endif // COMPRESSION_SEN_TESTCLASS_H