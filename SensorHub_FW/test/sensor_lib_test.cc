#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <i2c_helper.h>
#define Mock_addr 0x29

using ::testing::Return;
using ::testing::InSequence;

TEST(I2CWrapperTest, initCallsRightMethods) {
    i2c_testClass i2c_peripheral_mock;
    I2CDriver driver_handle = I2CDriver(&i2c_peripheral_mock,
                                        i2c_speed_100KHz, Mock_addr);
    EXPECT_CALL(i2c_peripheral_mock, begin());
    driver_handle.init();
}

TEST(I2CWrapperTest, write_regCallsRightMethods) {
    /* Mock class and i2c_driver instantiation*/
    i2c_testClass i2c_peripheral_mock;
    I2CDriver driver_handle = I2CDriver(&i2c_peripheral_mock,
                                        i2c_speed_100KHz, Mock_addr);
    /* Parameters used in this test*/
    const uint16_t reg = 0x0530;
    const uint16_t data = 0x30;
    /* Generate mock method input parameters*/
    const uint8_t reg_upper_byte = (reg >> 8) & 0xFF;
    const uint8_t reg_lower_byte = (reg & 0xFF);
    /* The expected function calls*/
    EXPECT_CALL(i2c_peripheral_mock, beginTransmission(Mock_addr));
    {
        InSequence seq;
        EXPECT_CALL(i2c_peripheral_mock, write(reg_upper_byte));
        EXPECT_CALL(i2c_peripheral_mock, write(reg_lower_byte));
        EXPECT_CALL(i2c_peripheral_mock, write(data));
    }
    EXPECT_CALL(i2c_peripheral_mock, endTransmission());
    /* The object method which calls to mock methods under the hood*/
    driver_handle.write_reg(reg, data);
}

TEST(I2CWrapperTest, write_reg16CallsRightMethods) {
    /* Mock class and i2c_driver instantiation*/
    i2c_testClass i2c_peripheral_mock;
    I2CDriver driver_handle = I2CDriver(&i2c_peripheral_mock,
                                        i2c_speed_100KHz, Mock_addr);
    /* Parameters used in this test*/
    const uint16_t reg = 0x0510;
    const uint16_t data = 0x3050;
    /* Generate mock method input parameters*/
    const uint8_t reg_upper_byte = (reg >> 8) & 0xFF;
    const uint8_t reg_lower_byte = (reg & 0xFF);
    const uint8_t data_upper_byte = (data >> 8) & 0xFF;
    const uint8_t data_lower_byte = data & 0xFF;
    /* The expected function calls*/
    EXPECT_CALL(i2c_peripheral_mock, beginTransmission(Mock_addr));
    {
        InSequence seq;
        EXPECT_CALL(i2c_peripheral_mock, write(reg_upper_byte));
        EXPECT_CALL(i2c_peripheral_mock, write(reg_lower_byte));
        EXPECT_CALL(i2c_peripheral_mock, write(data_upper_byte));
        EXPECT_CALL(i2c_peripheral_mock, write(data_lower_byte));
    }
    EXPECT_CALL(i2c_peripheral_mock, endTransmission());
    /* The object method which calls to mock methods under the hood*/
    driver_handle.write_reg16(reg, data);
}

TEST(I2CWrapperTest, read_regCallsRightMethods) {
    /* Mock class and i2c_driver instantiation*/
    i2c_testClass i2c_peripheral_mock;
    I2CDriver driver_handle = I2CDriver(&i2c_peripheral_mock,
                                        i2c_speed_100KHz, Mock_addr);
    /* Parameters used in this test*/
    const uint16_t reg = 0x05;
    const uint8_t data_to_return = 0x53;
    /* Generate mock method input parameters*/
    const uint8_t reg_upper_byte = (reg >> 8) & 0xFF;
    const uint8_t reg_lower_byte = (reg & 0xFF);
    const uint8_t request_amount_of_bytes = 1;
    /* The expected function calls*/
    EXPECT_CALL(i2c_peripheral_mock, beginTransmission(Mock_addr));
    {
        InSequence seq;
        EXPECT_CALL(i2c_peripheral_mock, write(reg_upper_byte));
        EXPECT_CALL(i2c_peripheral_mock, write(reg_lower_byte));
    }
    EXPECT_CALL(i2c_peripheral_mock, endTransmission(false));
    EXPECT_CALL(i2c_peripheral_mock,
                requestFrom(Mock_addr, request_amount_of_bytes));
    EXPECT_CALL(i2c_peripheral_mock, read())
    .WillRepeatedly(Return(data_to_return));
    /* The object method which calls to mock methods under the hood*/
    uint8_t data_returned = driver_handle.read_reg(reg);
    /* Check if returned value matched the value that mock function returned*/
    EXPECT_EQ(data_returned, data_to_return);
}

TEST(I2CWrapperTest, read_reg16CallsRightMethods) {
    /* Mock class and i2c_driver instantiation*/
    i2c_testClass i2c_peripheral_mock;
    I2CDriver driver_handle = I2CDriver(&i2c_peripheral_mock,
                                        i2c_speed_100KHz, Mock_addr);
    /* Parameters used in this test*/
    const uint16_t reg = 0x0520;
    const uint16_t data_to_return = 0x5320;
    /* Generate mock method input parameters*/
    const uint8_t reg_upper_byte = (reg >> 8) & 0xFF;
    const uint8_t reg_lower_byte = (reg & 0xFF);
    const uint8_t data_upper_byte = (data_to_return >> 8) & 0xFF;
    const uint8_t data_lower_byte = (data_to_return & 0xFF);
    const uint8_t request_amount_of_bytes = 2;  // We request 16-bits of data
    /* The expected function calls*/
    EXPECT_CALL(i2c_peripheral_mock, beginTransmission(Mock_addr));
    {
        InSequence seq;
        EXPECT_CALL(i2c_peripheral_mock, write(reg_upper_byte));
        EXPECT_CALL(i2c_peripheral_mock, write(reg_lower_byte));
        EXPECT_CALL(i2c_peripheral_mock, endTransmission(false));
        EXPECT_CALL(i2c_peripheral_mock,
                    requestFrom(Mock_addr, request_amount_of_bytes));
        EXPECT_CALL(i2c_peripheral_mock, read())
        .WillOnce(Return(data_upper_byte));
        EXPECT_CALL(i2c_peripheral_mock, read())
        .WillOnce(Return(data_lower_byte));
    }
    /* The object method which calls to mock methods under the hood*/
    uint16_t data_returned = driver_handle.read_reg16(reg);
    /* Check if returned value matched the value that mock function returned*/
    EXPECT_EQ(data_returned, data_to_return);
}


int main(int argc, char **argv) {
    // ::testing::InitGoogleTest(&argc, argv);
    // if you plan to use GMock, replace the line above with
     ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS())
      continue;

    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}
