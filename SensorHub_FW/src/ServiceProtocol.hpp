#ifndef SERVICEPROTOCOL_HPP
#define SERVICEPROTOCOL_HPP
#include <Arduino.h>
#include <ringbuffer.hpp>
#include <usb_service_protocol.hpp>
#include <FreeRTOS.h>
#include <queue.h>
#include <Status.hpp>

using namespace usb_service_protocol;

inline constexpr char status[] = "OK";

const char kStreamFormatString[] = "{\"NumOfShorts\": %d, \"SampleNum\": %d, \"Sensor\": %d, \"Buf\": [";
const char kStreamDataFormatString[] = "{ \"Val\": %d}";
const char kHelpUsageString[] = "**********************************HELP************************************\r\n" \
                                "CMDS: STATUS - Prints: status of the system, connected devices and sampletime\r\n" \
                                "      SETPORT [DeviceType portA] [DeviceType portB] [DeviceType BackBone] \r\n"    \
                                "             - Setup (sensor) software drivers on the selected port in argument\r\n" \
                                "      SETID [UniqueDeviceID] - Sets a unique identifier for the system\r\n"          \
                                "      STREAM - Stream current sensor measurements to this console\r";

const char kSetPortFormatString[] = "!OK Port A set to: %d, Port B set to: %d, Port BB set to: %d";
const char kInvalidArgumentValues[] = "!E Invalid arguments entered!";
const char kSetIDFormatString[] = "!OK Device id is set to: %d";

inline constexpr int MessageBufferSize = 1024;
static char MessageBuffer[MessageBufferSize];
inline constexpr int kUpperRangeArgSetPort = 0x03;
inline constexpr int kLowerRangeArgSetPort = 0x00;


inline constexpr uint8_t kNumOfRegisters = 6;
inline constexpr uint8_t ParseOK = 1;
inline constexpr uint8_t ParseFail = 0;



typedef struct {
  uint8_t num_of_arguments;
  int upper_range;
  int lower_range;
} ArgSpecs;

uint8_t ParseEnteredArgumentsToInt(char **argument, int *buffer, const ArgSpecs ArgSpec) {
for(uint8_t i =0; i < ArgSpec.num_of_arguments; i++) {
buffer[i] = atoi(argument[i]);
if(buffer[i] < ArgSpec.lower_range || buffer[i] > ArgSpec.upper_range) {
  return ParseFail;
}
}
return ParseOK;
}

void ComposeJsonDataReq(SensorData *data){
  memset(MessageBuffer, '\0', MessageBufferSize);
  uint8_t num_of_shorts = data->num_of_bytes > 1 ? (data->num_of_bytes)/2 : 1;
  int cnt = snprintf(MessageBuffer, MessageBufferSize, kStreamFormatString, num_of_shorts, data->sample_num, data->sensor_id);
for(uint8_t i = 0; i < num_of_shorts; i++) {
cnt += snprintf(MessageBuffer+cnt, MessageBufferSize-cnt, kStreamDataFormatString, data->buffer[i]);
if(i != num_of_shorts-1)
	cnt += snprintf(MessageBuffer+cnt, MessageBufferSize-cnt, ",");
}
snprintf(MessageBuffer+cnt, MessageBufferSize-cnt, "]}");
}


const char* CMD_STATUS_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', MessageBufferSize);
  DevStatus.GetDeviceStatus(MessageBuffer, 1024);
  return MessageBuffer;
}

const char* CMD_SETPORT_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', MessageBufferSize);
  int argBuffer[kNumOfI2CPorts];
  const ArgSpecs SetPortSpecs = {kNumOfI2CPorts, kUpperRangeArgSetPort, kLowerRangeArgSetPort};
  if (ParseEnteredArgumentsToInt(args, argBuffer, SetPortSpecs)) {
    PortAMgr.AssignSensorToI2CPort((SensorTypes) argBuffer[0]);
    PortBMgr.AssignSensorToI2CPort((SensorTypes) argBuffer[1]);
    snprintf(MessageBuffer, 1024, kSetPortFormatString, argBuffer[0], argBuffer[1], argBuffer[2]);
    return MessageBuffer;
  } else {
    return kInvalidArgumentValues;
  }
}

const char* CMD_SETID_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', MessageBufferSize);
  int argBuffer[1];
  const ArgSpecs SetIDSpecs = {1, 0xff, 0x00};
  if(ParseEnteredArgumentsToInt(args, argBuffer, SetIDSpecs)) {
    DevStatus.SetDeviceID(argBuffer[0]);
    snprintf(MessageBuffer, 1024, kSetIDFormatString, argBuffer[0]);
    return MessageBuffer;
  } else {
    return kInvalidArgumentValues;
  }
}

const char* CMD_STREAM_CB(char** args, int num_of_args) {
  // PortAMgr.ResumeSensor();
  // PortBMgr.ResumeSensor();
  memset(MessageBuffer, '\0', MessageBufferSize);
  SensorData data;
  while(1) {
  if( xQueueReceive( ServiceProtocolQueue,
                     &(data ),
                     ( TickType_t ) 10 ) == pdPASS )
  {
        ComposeJsonDataReq(&data);
        return MessageBuffer;
  }
  }
  // PortAMgr.PauseSensor();
  // PortBMgr.PauseSensor();
  return "!E can't receive message from queue";
}

const char* CMD_HELP_CB(char** args, int num_of_args) {
return kHelpUsageString;
}

static USBServiceProtocolRegisters USBRegisters[kNumOfRegisters]{{"STATUS", 0, false, CMD_STATUS_CB},
                                               {"SETPORT", 3, false, CMD_SETPORT_CB},
                                               {"SETID", 1, false, CMD_SETID_CB},
                                               {"STREAM", 0, true, CMD_STREAM_CB},
                                               {"HELP", 0, false, CMD_HELP_CB}};

#endif