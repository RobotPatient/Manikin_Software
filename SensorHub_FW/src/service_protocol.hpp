#ifndef SERVICEPROTOCOL_HPP
#define SERVICEPROTOCOL_HPP
#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <device_status.hpp>
#include <ringbuffer.hpp>
#include <usb_service_protocol.hpp>

using namespace usb_service_protocol;

inline constexpr char status[] = "OK";

const char kStreamFormatString[] = "{\"NumOfShorts\": %d, \"SampleNum\": %d, \"Sensor\": %d, \"Buf\": [";
const char kStreamDataFormatString[] = "{ \"Val\": %d}";
const char kHelpUsageString[] =
    "**********************************HELP************************************"
    "\r\n"
    "CMDS: STATUS - Prints: status of the system, connected devices and "
    "sampletime\r\n"
    "      SETPORT [DeviceType portA] [DeviceType portB] [DeviceType BackBone] "
    "\r\n"
    "             - Setup (sensor) software drivers on the selected port in "
    "argument\r\n"
    "      SETID [UniqueDeviceID] - Sets a unique identifier for the system\r\n"
    "      STREAM - Stream current sensor measurements to this console\r";

const char kSetPortFormatString[] = "!OK Port A set to: %d, Port B set to: %d, Port BB set to: %d";
const char kInvalidArgumentValues[] = "!E Invalid arguments entered!";
const char kSetIDFormatString[] = "!OK Device id is set to: %d";
const char kSetSampleTimeFormatString[] = "!OK Sampletime on Port A set to: %d, Port B set to: %d";

inline constexpr int kMessageBufferSize = 1024;
static char MessageBuffer[kMessageBufferSize];
inline constexpr int kUpperRangeArgSetPort = kNumOfSupportedSensors-1;
inline constexpr int kLowerRangeArgSetPort = 0;

inline constexpr uint8_t kNumOfRegisters = 7;
inline constexpr uint8_t ParseOK = 1;
inline constexpr uint8_t ParseFail = 0;

typedef struct {
  uint8_t num_of_arguments;
  int upper_range;
  int lower_range;
} ArgSpecs;

uint8_t ParseEnteredArgumentsToInt(char** argument, int* buffer, const ArgSpecs ArgSpec) {
  for (uint8_t i = 0; i < ArgSpec.num_of_arguments; i++) {
    buffer[i] = atoi(argument[i]);
    if (buffer[i] < ArgSpec.lower_range || buffer[i] > ArgSpec.upper_range) {
      return ParseFail;
    }
  }
  return ParseOK;
}

void ComposeJsonDataReq(SensorData* data) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  uint8_t num_of_shorts = data->num_of_bytes > 1 ? (data->num_of_bytes) / 2 : 1;
  int cnt = snprintf(MessageBuffer, kMessageBufferSize, kStreamFormatString, num_of_shorts, data->sample_num,
                     data->sensor_id);
  for (uint8_t i = 0; i < num_of_shorts; i++) {
    cnt += snprintf(MessageBuffer + cnt, kMessageBufferSize - cnt, kStreamDataFormatString, data->buffer[i]);
    if (i != num_of_shorts - 1)
      cnt += snprintf(MessageBuffer + cnt, kMessageBufferSize - cnt, ",");
  }
  snprintf(MessageBuffer + cnt, kMessageBufferSize - cnt, "]}");
}

const char* CMD_STATUS_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  systemStatus.GetDeviceStatus(MessageBuffer, 1024);
  return MessageBuffer;
}

const char* CMD_SETPORT_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  int argBuffer[kNumOfI2CPorts];
  const ArgSpecs SetPortSpecs = {kNumOfI2CPorts, kUpperRangeArgSetPort, kLowerRangeArgSetPort};
  if (ParseEnteredArgumentsToInt(args, argBuffer, SetPortSpecs)) {
    portAProperties.AssignSensorToI2CPort((SensorTypes)argBuffer[module::status::kSensorPortAIndex]);
    portBProperties.AssignSensorToI2CPort((SensorTypes)argBuffer[module::status::kSensorPortBIndex]);
    snprintf(MessageBuffer, kMessageBufferSize, kSetPortFormatString, argBuffer[module::status::kSensorPortAIndex],
             argBuffer[module::status::kSensorPortBIndex], argBuffer[module::status::kBBPortIndex]);
    return MessageBuffer;
  } else {
    return kInvalidArgumentValues;
  }
}

const char* CMD_SETID_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  int argBuffer[1];
  const ArgSpecs SetIDSpecs = {1, 0xff, 0x00};
  if (ParseEnteredArgumentsToInt(args, argBuffer, SetIDSpecs)) {
    systemStatus.SetDeviceID(argBuffer[0]);
    snprintf(MessageBuffer, 1024, kSetIDFormatString, argBuffer[0]);
    return MessageBuffer;
  } else {
    return kInvalidArgumentValues;
  }
}

const char* CMD_STREAM_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  SensorData data;
  while (1) {
    if (xQueueReceive(serviceProtocolQueue, &(data), (TickType_t)10) == pdPASS) {
      ComposeJsonDataReq(&data);
      return MessageBuffer;
    }
  }
  return "!E can't receive message from queue";
}

const char* CMD_HELP_CB(char** args, int num_of_args) {
  return kHelpUsageString;
}

const char* CMD_SETSR_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  int argBuffer[2];
  const ArgSpecs SetIDSpecs = {2, 1000, 10};
  if (ParseEnteredArgumentsToInt(args, argBuffer, SetIDSpecs)) {
    portAProperties.SetSampleTime(argBuffer[0]);
    portBProperties.SetSampleTime(argBuffer[1]);
    snprintf(MessageBuffer, 1024, kSetSampleTimeFormatString, argBuffer[0], argBuffer[1]);
    return MessageBuffer;
  } else {
    return kInvalidArgumentValues;
  }
}

static USBServiceProtocolRegisters USBRegisters[kNumOfRegisters]{
    {"STATUS", 0, false, CMD_STATUS_CB}, {"SETPORT", 3, false, CMD_SETPORT_CB}, {"SETID", 1, false, CMD_SETID_CB},
    {"STREAM", 0, true, CMD_STREAM_CB},  {"SETSR", 2, false, CMD_SETSR_CB},     {"HELP", 0, false, CMD_HELP_CB}};

#endif