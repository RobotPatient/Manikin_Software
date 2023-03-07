#include <i2c_helper.hpp>
#include <sensor_compression.hpp>
#include <gmock/gmock.h>

#define DISTANCE_READING_DUM_VAL 17
#define BUFFER_BYTES             1

using ::testing::Return;
using ::testing::InSequence;

void InitVL6180xCalls(I2CDriver* i2c_handle_mock){
  EXPECT_CALL(*i2c_handle_mock, read_reg(VL6180X_SYSTEM_FRESH_OUT_OF_RESET))
  .WillOnce(Return(0x01));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x0207, 0x01));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x0208, 0x01));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x0096, 0x00));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x0097, 0xfd));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00e3, 0x00));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00e4, 0x04));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00e5, 0x02)); 
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00e6, 0x01));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00e7, 0x03)); 
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00f5, 0x02));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00d9, 0x05));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00db, 0xce));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00dc, 0x03));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00dd, 0xf8));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x009f, 0x00));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00a3, 0x3c));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00b7, 0x00));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00bb, 0x3c));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00b2, 0x09));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00ca, 0x09));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x0198, 0x01));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x01b0, 0x17));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x01ad, 0x00));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x00ff, 0x05));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x0100, 0x05));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x0199, 0x05));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x01a6, 0x1b));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x01ac, 0x3e));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x01a7, 0x1f));
  EXPECT_CALL(*i2c_handle_mock, write_reg(0x0030, 0x00));
}

void VL6180xDefaultSettingsCalls(I2CDriver* i2c_handle_mock){

  // Set GPIO1 high when sample complete
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, (4 << 3) | (4)));

  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSTEM_MODE_GPIO1, 0x10));               // Set GPIO1 high when sample complete
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30)); // Set Avg sample period
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSALS_ANALOGUE_GAIN, 0x46));            // Set the ALS gain
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSRANGE_VHV_REPEAT_RATE, 0xFF));        // Set auto calibration period (Max = 255)/(OFF = 0)
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x63));       // Set ALS integration time to 100ms
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSRANGE_VHV_RECALIBRATE, 0x01));        // perform a single temperature calibration
  
  // Optional settings from datasheet
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09)); // Set default ranging inter-measurement period to 100ms
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSALS_INTERMEASUREMENT_PERIOD, 0x0A));   // Set default ALS inter-measurement period to 100ms
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x24));     // Configures interrupt on ‘New Sample Ready threshold event’
  // Additional settings defaults from community
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME, 0x32));
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSRANGE_RANGE_CHECK_ENABLES, 0x10 | 0x01));
  EXPECT_CALL(*i2c_handle_mock, write_reg16(VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE, 0x7B));
  EXPECT_CALL(*i2c_handle_mock, write_reg16(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x64));
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30));
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_SYSALS_ANALOGUE_GAIN, 0x40));
  EXPECT_CALL(*i2c_handle_mock, write_reg(VL6180X_FIRMWARE_RESULT_SCALER, 0x01));
}

TEST(compressionTest, initCalls) {
  using ::testing::Mock;
  i2c_testClass class_mock;
  I2CDriver i2c_handle_mock;
  CompressionSensor CompSensor = CompressionSensor(&i2c_handle_mock);
  EXPECT_CALL(i2c_handle_mock, change_address(SENSOR_ADDR));
  {
    InSequence seq;
    InitVL6180xCalls(&i2c_handle_mock);
    VL6180xDefaultSettingsCalls(&i2c_handle_mock);

  }
  CompSensor.Initialize();
  Mock::VerifyAndClearExpectations(&i2c_handle_mock);
}

TEST(compressionTest, GetSensorData) {
  using ::testing::Mock;
  i2c_testClass class_mock;
  I2CDriver i2c_handle_mock;
  SensorData ExpectedOutput;
  ExpectedOutput.buffer[0] = 0xAF;
  ExpectedOutput.numOfBytes = 1;
  CompressionSensor CompSensor = CompressionSensor(&i2c_handle_mock);
  EXPECT_CALL(i2c_handle_mock, write_reg(VL6180X_SYSRANGE_START, 0x01));
  EXPECT_CALL(i2c_handle_mock, write_reg(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07));
  EXPECT_CALL(i2c_handle_mock, read_reg(VL6180X_RESULT_RANGE_VAL)).WillOnce(Return(0xAF));
  SensorData data = CompSensor.GetSensorData();
  EXPECT_EQ(ExpectedOutput.numOfBytes, data.numOfBytes);
  EXPECT_EQ(ExpectedOutput.buffer[0], data.buffer[0]);
  Mock::VerifyAndClearExpectations(&i2c_handle_mock);  
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}