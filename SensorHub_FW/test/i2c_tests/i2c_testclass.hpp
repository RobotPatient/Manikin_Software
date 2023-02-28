#ifndef I2C_TESTCLASS_H
#define I2C_TESTCLASS_H
#include <stdint.h>
#include <string.h>
#include <gmock/gmock.h>  // Brings in Google Mock.

class i2c_testClass{
    public:
    MOCK_METHOD0(begin, void());
    MOCK_METHOD1(beginTransmission, void(uint8_t address));
    MOCK_METHOD1(write, size_t(uint8_t ucData));
    MOCK_METHOD2(write, size_t(const uint8_t *data, size_t quantity));
    MOCK_METHOD0(endTransmission, void());
    MOCK_METHOD2(requestFrom, uint8_t(uint8_t address, size_t quantity));
    MOCK_METHOD2(readBytes, size_t( uint8_t *buffer, size_t length));
    MOCK_METHOD0(read, int());
    MOCK_METHOD3(requestFrom, uint8_t(uint8_t address, size_t quantity, bool stopBit));
    MOCK_METHOD1(endTransmission, uint8_t(bool stopBit));
};
#endif