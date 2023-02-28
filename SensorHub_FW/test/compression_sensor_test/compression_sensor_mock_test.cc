#include <i2c_helper.hpp>
#include <sensor_compression.hpp>
#include "compression_sen_testclass.hpp"

TEST(CompressionSensor, initCalls) {
  CompressionSenTestclass compr_sen_mock;
  EXPECT_CALL(compr_sen_mock, Uninitialize());
  compr_sen_mock.Uninitialize();
}

TEST(CompressionSensor, GetSensorData) {
  CompressionSenTestclass compr_sen_mock;
  EXPECT_CALL(compr_sen_mock, GetSensorData());
  compr_sen_mock.GetSensorData();
}

TEST(CompressionSensor, Uninitialize) {
  CompressionSenTestclass compr_sen_mock;
  EXPECT_CALL(compr_sen_mock, Uninitialize());
  compr_sen_mock.Uninitialize();
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}