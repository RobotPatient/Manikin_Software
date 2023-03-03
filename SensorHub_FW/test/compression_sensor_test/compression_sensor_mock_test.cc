#include "./test_code/sensor_compression.hpp"
#include "compression_sen_testclass.hpp"

TEST(compressionTest, initCalls) {
  RobotPatient_VL6180x_mock tof;
  CompressionSensor_test ctest(&tof); 

  EXPECT_CALL(tof, init());
  EXPECT_CALL(tof, VL6180xDefautSettings());
  ctest.Initialize();
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}