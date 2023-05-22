/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/
#ifndef SERVICEPROTOCOL_HPP
#define SERVICEPROTOCOL_HPP
#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <device_settings.hpp>
#include <device_status.hpp>
#include <ringbuffer.hpp>
#include <usb_service_protocol.hpp>

/**
 * @brief Format strings, used to create the responses with!
*/
const char kStreamFormatString[] = "{\"NumOfShorts\": %d, \"SampleNum\": %d, \"Sensor\": %d, \"Buf\": [";
const char kStreamDataFormatString[] = "{ \"Val\": %d}";
const char kHelpUsageString[] = "**********************************HELP************************************"
                                "\r\n"
                                "CMDS: STATUS - Prints: status of the system, connected devices and "
                                "sampletime\r\n"
                                "      SETPORT [DeviceType portA] [DeviceType portB] [DeviceType BackBone] "
                                "\r\n"
                                "             - Setup (sensor) software drivers on the selected port in "
                                "argument\r\n"
                                "      SETID [UniqueDeviceID] - Sets a unique identifier for the system\r\n"
                                "      STREAM - Stream current sensor measurements to this console\r\n"
                                "      SETSR [Sample rate port a] [Sample rate port b] - Sets the sample rate for \r\n"
                                "                                         port a in b in milliseconds (10-1000 ms)\r";
const char kSetPortFormatString[] = "!OK Port A set to: %d, Port B set to: %d, Port BB set to: %d";
const char kInvalidArgumentValues[] = "!E Invalid arguments entered!";
const char kSetIDFormatString[] = "!OK Device id is set to: %d";
const char kSetSampleTimeFormatString[] = "!OK Sampletime on Port A set to: %d, Port B set to: %d";

/**
 * @brief Statically assigned (output) messagebuffer, used to store the output of commands in.
 *        Gets used by multiple callback methods.
*/
inline constexpr int kMessageBufferSize = 1024;
static char MessageBuffer[kMessageBufferSize];

/**
 * @brief The num of registers defined in the usb service protocol registers array
*/
inline constexpr uint8_t kNumOfRegisters = 7;

/**
 * @brief Argument parser constants
*/
inline constexpr uint8_t kNumOfArgumentsSetPort = 3;
inline constexpr int kUpperRangeArgSetPort = kNumOfSupportedSensors-1;
inline constexpr int kLowerRangeArgSetPort = 0;
inline constexpr uint8_t kNumOfArgumentsSetID = 1;
inline constexpr int kUpperRangeArgSetID = 255;
inline constexpr int kLowerRangeArgSetID = 0;
inline constexpr uint8_t kNumOfArgumentsSetSR = 2;
inline constexpr int kUpperRangeArgSetSR = 1000;
inline constexpr int kLowerRangeArgSetSR = 10;
inline constexpr uint8_t ParseOK = 1;
inline constexpr uint8_t ParseFail = 0;

/**
 * @brief Struct used to pass options for 
 *        argument parsing between methods
 * 
 * @note num_of_arguments is the number of arguments to check
 * @note upper_range is the highest integer number the argument can be
 * @note lower_range is the lowest integer number the argument can be
 * 
 * @note The argument has to be in between these two numbers!
*/
typedef struct {
  uint8_t num_of_arguments;
  int upper_range;
  int lower_range;
} ArgSpecs;

/**
 * @brief This function will convert the string type arguments to an integer
 *        and check if the argument value falls within the given Argspec range
 * 
 * @param argument An two dimensional array containing the string argument values.
 * @param buffer An integer buffer to save the parsed arguments to.
 * @param ArgSpec A struct containing the number of arguments to parse, 
 *                the permitted max value of the argument and 
 *                the permitted min value of the argument
 * @return 1 on Succes! 0 on Fail!
 * 
 * @note All arguments are checked with the same upper and lower range from the ArgSpec struct!
 * 
*/
uint8_t ParseEnteredArgumentsToInt(char** argument, int* buffer, const ArgSpecs ArgSpec) {
  for (uint8_t arg_num = 0; arg_num < ArgSpec.num_of_arguments; arg_num++) {
    buffer[arg_num] = atoi(argument[arg_num]);
    if (buffer[arg_num] < ArgSpec.lower_range || buffer[arg_num] > ArgSpec.upper_range) {
      return ParseFail;
    }
  }
  return ParseOK;
}

/**
 * @brief This function will compose a Json formatted string of the sensordata and put it in to the messagebuffer
 * 
 * @param data The SensorData to convert to Json String
*/
void ComposeJsonFormattedStringOfSensorData(SensorData* data) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  uint8_t num_of_shorts = data->num_of_bytes > 1 ? (data->num_of_bytes) / 2 : 1;
  int writecnt = snprintf(MessageBuffer, kMessageBufferSize, kStreamFormatString, num_of_shorts, data->sample_num, data->sensor_id);
  for (uint8_t sensor_short_num = 0; sensor_short_num < num_of_shorts; sensor_short_num++) {
    writecnt += snprintf(MessageBuffer + writecnt, kMessageBufferSize - writecnt, kStreamDataFormatString, data->buffer[sensor_short_num]);
    if (sensor_short_num != num_of_shorts - 1)
      writecnt += snprintf(MessageBuffer + writecnt, kMessageBufferSize - writecnt, ",");
  }
  snprintf(MessageBuffer + writecnt, kMessageBufferSize - writecnt, "]}");
}

/**
 * @brief Callback function for the STATUS command. This function will be ran when STATUS command is entered.
 *        The STATUS command return the status of the system in a formatted JSON string.
 * 
 * @param args A two dimensional array containing the arguments entered after the command
 * @param num_of_args The number of arguments entered after the command
 * 
 * @return Command response string, which will be a json formatted status string.
*/
const char* CMD_STATUS_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  systemStatus.GetDeviceStatus(MessageBuffer, kMessageBufferSize);
  return MessageBuffer;
}

/**
 * @brief Callback function for the SETPORT command. This function will be ran when SETPORT command is entered.
 *        The SETPORT command sets the sensortypes for the given ports..
 * 
 * @param args A two dimensional array containing the arguments entered after the command
 * @param num_of_args The number of arguments entered after the command
 * 
 * @return Command response string, which will be a !OK ..[confirmation of entered arguments] if arguments where valid numbers
 *         !E Invalid arguments entered if the arguments where not valid!
*/
const char* CMD_SETPORT_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  int argBuffer[kNumOfI2CPorts];
  const ArgSpecs SetPortSpecs = {kNumOfI2CPorts, kUpperRangeArgSetPort, kLowerRangeArgSetPort};
  const bool ArgumentsAreValid = ParseEnteredArgumentsToInt(args, argBuffer, SetPortSpecs);
  if (ArgumentsAreValid) {
    portAProperties.AssignSensorToI2CPort((SensorTypes)argBuffer[module::status::kSensorPortAIndex]);
    portBProperties.AssignSensorToI2CPort((SensorTypes)argBuffer[module::status::kSensorPortBIndex]);
    snprintf(MessageBuffer, kMessageBufferSize, kSetPortFormatString, argBuffer[module::status::kSensorPortAIndex],
             argBuffer[module::status::kSensorPortBIndex], argBuffer[module::status::kBBPortIndex]);
    return MessageBuffer;
  } else {
    return kInvalidArgumentValues;
  }
}

/**
 * @brief Callback function for the SETID command. This function will be ran when SETID command is entered.
 *        The SETID command sets an unique identifier for this subsystem (ID will be written to external flash!).
 *        ID can be read using STATUS command!
 * 
 * @param args A two dimensional array containing the arguments entered after the command
 * @param num_of_args The number of arguments entered after the command
 * 
 * @return Command response string, which will be a !OK ..[confirmation of entered arguments] if arguments where valid numbers
 *         !E Invalid arguments entered if the arguments where not valid!
*/
const char* CMD_SETID_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  int argBuffer[kNumOfArgumentsSetID];
  const ArgSpecs SetIDSpecs = {kNumOfArgumentsSetID, kUpperRangeArgSetID, kLowerRangeArgSetID};
  const bool ArgumentsAreValid = ParseEnteredArgumentsToInt(args, argBuffer, SetIDSpecs);
  if (ArgumentsAreValid) {
    systemStatus.SetDeviceID(argBuffer[0]);
    snprintf(MessageBuffer, kMessageBufferSize, kSetIDFormatString, argBuffer[0]);
    return MessageBuffer;
  } else {
    return kInvalidArgumentValues;
  }
}

/**
 * @brief Callback function for the STREAM command. This function will be ran when STREAM command is entered.
 *        This command is an stream command, meaning it will run repeatedly until \r (ENTER) is detected in serial console.
 *        The Stream command returns the sensordata of each sensor in a formatted json string.
 * 
 * @param args A two dimensional array containing the arguments entered after the command
 * @param num_of_args The number of arguments entered after the command
 * 
 * @return Command response string, which will be a json formatted string!
*/
const char* CMD_STREAM_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  SensorData data;
  while (1) {
    // Receive the SensorData from the message queue
    if (xQueueReceive(serviceProtocolQueue, &(data), (TickType_t)10) == pdPASS) {
      ComposeJsonFormattedStringOfSensorData
    (&data);
      return MessageBuffer;
    }
  }
  return "!E can't receive message from queue";
}

/**
 * @brief Callback function for the HELP command. This function will be ran when HELP command is entered.
 *        The HELP command return the usage of the console in a formatted string.
 * 
 * @param args A two dimensional array containing the arguments entered after the command
 * @param num_of_args The number of arguments entered after the command
 * 
 * @return Command response string, which will be a formatted string explaining how to use this console!
*/
const char* CMD_HELP_CB(char** args, int num_of_args) {
  return kHelpUsageString;
}

/**
 * @brief Callback function for the SETSR command. This function will be ran when SETSR command is entered.
 *        The SETSR command sets the sample rate for the sensorports.
 * 
 * @param args A two dimensional array containing the arguments entered after the command
 * @param num_of_args The number of arguments entered after the command
 * 
 * @return Command response string, which will be a !OK ..[confirmation of entered arguments] if arguments where valid numbrs
 *         !E Invalid arguments entered if the arguments where not valid!
*/
const char* CMD_SETSR_CB(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', kMessageBufferSize);
  int argBuffer[kNumOfArgumentsSetSR];
  const ArgSpecs SetIDSpecs = {kNumOfArgumentsSetSR, kUpperRangeArgSetSR, kLowerRangeArgSetSR};
  const bool ArgumentsAreValid = ParseEnteredArgumentsToInt(args, argBuffer, SetIDSpecs);
  if (ArgumentsAreValid) {
    portAProperties.SetSampleTime(argBuffer[0]);
    portBProperties.SetSampleTime(argBuffer[1]);
    snprintf(MessageBuffer, kMessageBufferSize, kSetSampleTimeFormatString, argBuffer[0], argBuffer[1]);
    return MessageBuffer;
  } else {
    return kInvalidArgumentValues;
  }
}

/**
 * @brief STATIC declaration of all the CB methods and command information
 * @note The format of the struct is { "String of the command", Number of Arguments, Command is stream command, Pointer to callback method}
 * @note Stream command means that command will be ran repeatedly when entered. Command can be stopped by pressing enter key or '\r' character
*/
static usb_service_protocol::USBServiceProtocolRegisters USBRegisters[kNumOfRegisters]{ {"STATUS", 0, false, CMD_STATUS_CB}, 
                                                                                        {"SETPORT", kNumOfArgumentsSetPort, false, CMD_SETPORT_CB}, 
                                                                                        {"SETID", kNumOfArgumentsSetID, false, CMD_SETID_CB},
                                                                                        {"STREAM", 0, true, CMD_STREAM_CB}, 
                                                                                        {"SETSR", kNumOfArgumentsSetSR, false, CMD_SETSR_CB}, 
                                                                                        {"HELP", 0, false, CMD_HELP_CB}};
#endif