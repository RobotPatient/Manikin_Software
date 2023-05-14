#ifndef SERVICEPROTOCOL_HPP
#define SERVICEPROTOCOL_HPP
#include <Arduino.h>
#include <ringbuffer.hpp>
#include <usb_service_protocol.hpp>
#include "Status.hpp"
#include <FreeRTOS.h>
#include <queue.h>

using namespace usb_service_protocol;

inline constexpr char status[] = "OK";
const char kStatusFormatString[] = "{\"Status\": \"%s\", \"DeviceType\": %d, \"PortASenType\": %d, "
                                    "\"PORTBSENTYPE\": %d, \"PORTASAMPLETIME\": %d, \"PORTBSAMPLETIME\": %d}";
const char kStreamFormatString[] = "{\"NumOfShorts\": %d, \"SampleNum\": %d, \"Sensor\": %d, \"Buf\": [";
const char kStreamDataFormatString[] = "{ \"Val\": %d}";
const char kHelpUsageString[] = "**********************************HELP************************************\r\n" \
                                "CMDS: STATUS - Prints: status of the system, connected devices and sampletime\r\n" \
                                "      SETPORT [DeviceType portA] [DeviceType portB] [DeviceType BackBone] \r\n"    \
                                "             - Setup (sensor) software drivers on the selected port in argument\r\n" \
                                "      SETID [UniqueDeviceID] - Sets a unique identifier for the system\r\n"          \
                                "      STREAM - Stream current sensor measurements to this console\r";


static char MessageBuffer[1024];
inline constexpr uint8_t kNumOfRegisters = 6;

void ComposeJsonDataReq(SensorData *data){
  uint8_t num_of_shorts = (data->num_of_bytes)/2;
  int cnt = snprintf(MessageBuffer, 1024, kStreamFormatString, num_of_shorts, data->sample_num, data->sensor_id);
for(uint8_t i = 0; i < num_of_shorts; i++) {
cnt += snprintf(MessageBuffer+cnt, 1024-cnt, kStreamDataFormatString, data->buffer[i]);
if(i != num_of_shorts-1)
	cnt += snprintf(MessageBuffer+cnt, 1024-cnt, ",");
}
snprintf(MessageBuffer+cnt, 1024-cnt, "]}");
}


const char* CMD_STATUS_CB(char** args, int num_of_args) {
  DeviceStatus Status = PortAMgr.GetDeviceStatus();
  snprintf(MessageBuffer, 128, kStatusFormatString, status, 
                         Status.DeviceType, Status.PortASensorType, Status.PortBSensorType,
                         Status.PortASampleTime, Status.PortBSampleTime);
  return MessageBuffer;
}

const char* CMD_SETPORT_CB(char** args, int num_of_args) {
  return "!OK CMD SETPORTCB";
}

const char* CMD_SETID_CB(char** args, int num_of_args) {
  return "!OK CMD SETID_CB";
}

const char* CMD_STREAM_CB(char** args, int num_of_args) {
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
  return "!E can't receive message from queue";
}

const char* CMD_HELP_CB(char** args, int num_of_args) {
return kHelpUsageString;
}

USBServiceProtocolRegisters USBRegisters[kNumOfRegisters]{{"STATUS", 0, false, CMD_STATUS_CB},
                                               {"SETPORT", 3, false, CMD_SETPORT_CB},
                                               {"SETID", 1, false, CMD_SETID_CB},
                                               {"STREAM", 0, true, CMD_STREAM_CB},
                                               {"HELP", 0, false, CMD_HELP_CB}};

#endif