#include <i2c_helper.hpp>
#include <sensor_fingerposition.hpp>
#include <gmock/gmock.h>

uint8_t arb_testbuffer[16] = {0x05, 0x00, 0x85, 0x99, 
                              0x91, 0x74, 0x55, 0x14};

inline uint16_t ProcessedVal(uint8_t *buffer){
  return (buffer[0] << 4) | (buffer[1] >> 4);
}
uint16_t arb_testbuffer_processed[8] = {ProcessedVal(arb_testbuffer), ProcessedVal(arb_testbuffer+2), ProcessedVal(arb_testbuffer+4),
                                        ProcessedVal(arb_testbuffer+6), ProcessedVal(arb_testbuffer+8),ProcessedVal(arb_testbuffer+10),
                                        ProcessedVal(arb_testbuffer+12), ProcessedVal(arb_testbuffer+14)};

uint16_t arb_testbuffer_reindexed[8] = {arb_testbuffer_processed[LOWER], arb_testbuffer_processed[MID_L], arb_testbuffer_processed[MID_M],
                                        arb_testbuffer_processed[MID_H], arb_testbuffer_processed[RE_L], arb_testbuffer_processed[RE_H],
                                        arb_testbuffer_processed[LI_L], arb_testbuffer_processed[LI_H]};
int bufferindex =0;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;

uint16_t assembleRegister(uint8_t opcode, uint8_t regAddr) {
  uint16_t asmb_register = regAddr | (opcode << 8);
  return asmb_register; 
}

void returnReadBytesMethod(uint8_t *buffer, uint8_t num_of_bytes){
memcpy(buffer, arb_testbuffer+bufferindex, num_of_bytes);
bufferindex += 2;
}

TEST(FingerPositionTest, initCalls) {
  /* Generated Parameters*/
  const uint16_t reg1 = assembleRegister(SET_BIT, PIN_CFG);
  const uint8_t data1 = 0x00;
  const uint16_t reg2 = assembleRegister(SET_BIT, GENERAL_CFG);
  const uint8_t data2 = 0x02;
  const uint16_t reg3 = assembleRegister(SET_BIT, AUTO_SEQ_CH_SEL);
  const uint8_t data3 = 0xFF;
  const uint16_t reg4 = assembleRegister(SET_BIT, SEQUENCE_CFG);
  const uint8_t data4 = 0x01;
  /* Initialize handles and classes*/
  i2c_testClass class_mock;
  I2CDriver i2c_handle_mock;
  FingerPositionSensor FingerPosSensor = FingerPositionSensor(&i2c_handle_mock);
  /* Setup mock calls*/
  EXPECT_CALL(i2c_handle_mock, change_address(ADS7138_ADDR));
  {
    InSequence Seq;
    EXPECT_CALL(i2c_handle_mock, write_reg(reg1, data1));
    EXPECT_CALL(i2c_handle_mock, write_reg(reg2, data2));
    EXPECT_CALL(i2c_handle_mock, write_reg(reg3, data3));
    EXPECT_CALL(i2c_handle_mock, write_reg(reg4, data4));
  }
  /* Run the "real" call*/
  FingerPosSensor.Initialize();
  Mock::VerifyAndClearExpectations(&i2c_handle_mock);
}

TEST(FingerPositionTest, GetSensorData) {
  using ::testing::_;
  /* Initialize handles and classes*/
  const uint16_t reg1 = assembleRegister(SET_BIT, SEQUENCE_CFG);
  const uint8_t data1 = 1 << 4;
  const uint16_t reg2 = assembleRegister(CLEAR_BIT, SEQUENCE_CFG);
  const uint8_t data2 = 1 << 4;
  i2c_testClass class_mock;
  I2CDriver i2c_handle_mock;
  FingerPositionSensor FingerPosSensor = FingerPositionSensor(&i2c_handle_mock);
  /* Setup mock calls*/
  {
    InSequence seq;
    EXPECT_CALL(i2c_handle_mock, write_reg(reg1, data1));
    EXPECT_CALL(i2c_handle_mock, read_bytes(_,TWO_BYTE)).Times(8).WillRepeatedly(Invoke(returnReadBytesMethod));
    EXPECT_CALL(i2c_handle_mock, write_reg(reg2, data2));
  }
  SensorData data = FingerPosSensor.GetSensorData();
  EXPECT_EQ(16, data.numOfBytes);
  for(uint16_t i =0; i< 8; i++){
    EXPECT_EQ(arb_testbuffer_reindexed[i], data.buffer[i]);
  }
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