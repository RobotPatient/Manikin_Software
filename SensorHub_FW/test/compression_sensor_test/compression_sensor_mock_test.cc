#include "./test_code/sensor_compression.hpp"
#include "compression_sen_testclass.hpp"

#define DISTANCE_READING_DUM_VAL 17
#define BUFFER_BYTES             1

TEST(compressionTest, initCalls) {
  RobotPatient_VL6180x_mock tof;
  CompressionSensor_test ctest(&tof); 

  EXPECT_CALL(tof, init());
  EXPECT_CALL(tof, VL6180xDefautSettings());
  ctest.Initialize();
}

TEST(compressionTest, GetSensorData) {
  RobotPatient_VL6180x_mock tof;
  CompressionSensor_test ctest(&tof); 

  EXPECT_CALL(tof, getDistance())
    .Times(1)
    .WillOnce(testing::Return((uint8_t)DISTANCE_READING_DUM_VAL));

  SensorData dat = ctest.GetSensorData();
  EXPECT_EQ(dat.numOfBytes, BUFFER_BYTES);
  EXPECT_EQ(dat.buffer[0], DISTANCE_READING_DUM_VAL);
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}