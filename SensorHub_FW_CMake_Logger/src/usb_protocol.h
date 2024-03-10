#ifndef USB_PROTOCOL_H
#define USB_PROTOCOL_H
#include <stdlib.h>
#include <sensor_base.hpp>

namespace usb_service_protocol {
/**
 * @brief Format strings, used to create the responses with!
*/
const char stream_format_str[] = "{\"NumOfShorts\": %d, \"SampleNum\": %d, \"Sensor\": %d, \"Buf\": [";
const char stream_data_format_str[] = "{ \"Val\": %d}";
const char help_usage_str[] = "**********************************HELP************************************"
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

const char set_port_format_str[] = "!OK Port A set to: %d, Port B set to: %d, Port BB set to: %d";
const char invalid_argument_values[] = "!E Invalid arguments entered!";
const char set_id_format_str[] = "!OK Device id is set to: %d";
const char set_sample_time_format_str[] = "!OK Sampletime on Port A set to: %d, Port B set to: %d";

/**
 * @brief Statically assigned (output) messagebuffer, used to store the output of commands in.
 *        Gets used by multiple callback methods.
*/
#define MESSAGE_BUFFER_SIZE 512
char MessageBuffer[MESSAGE_BUFFER_SIZE];



/**
 * @brief The num of registers defined in the usb service protocol registers array
*/
#define NUM_OF_REGISTERS 7

/**
 * @brief Argument parser constants
*/
#define SET_PORT_NUM_OF_ARGUMENTS 3
#define SET_PORT_ARGUMENTS_UPPER_RANGE 3
#define SET_PORT_ARGUMENTS_LOWER_RANGE 0

#define SET_ID_NUM_OF_ARGUMENTS 1
#define SET_ID_ARGUMENTS_UPPER_RANGE 255
#define SET_ID_ARGUMENTS_LOWER_RANGE 0

#define SET_SR_NUM_OF_ARGUMENTS 2
#define SET_SR_ARGUMENTS_UPPER_RANGE 1000
#define SET_SR_ARGUMENTS_LOWER_RANGE 10

#define PARSE_OK 1
#define PARSE_FAIL 0

#define STREAM_CMD 1
#define NON_STREAM_CMD 0

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
 */
const uint8_t parse_entered_arguments_to_int(char** argument, int* buffer, const ArgSpecs ArgSpec) {
  for (uint8_t arg_num = 0; arg_num < ArgSpec.num_of_arguments; arg_num++) {
    buffer[arg_num] = atoi(argument[arg_num]);
    const uint8_t argument_is_not_within_range = (buffer[arg_num] < ArgSpec.lower_range || buffer[arg_num] > ArgSpec.upper_range);
    if (argument_is_not_within_range) {
      return PARSE_FAIL;
    }
  }
  return PARSE_OK;
}

/**
 * @brief This function will compose a Json formatted string of the sensordata and put it in to the messagebuffer
 * 
 * @param data The SensorData to convert to Json String
*/
void create_json_from_sensor_data(SensorData* data) {
  memset(MessageBuffer, '\0', MESSAGE_BUFFER_SIZE);
  uint8_t num_of_shorts = data->num_of_bytes > 1 ? (data->num_of_bytes) / 2 : 1;
  int writecnt = snprintf(MessageBuffer, MESSAGE_BUFFER_SIZE, stream_format_str, num_of_shorts, data->sample_num, data->sensor_id);
  for (uint8_t sensor_short_num = 0; sensor_short_num < num_of_shorts; sensor_short_num++) {
    writecnt += snprintf(MessageBuffer + writecnt, MESSAGE_BUFFER_SIZE - writecnt, stream_data_format_str, data->buffer[sensor_short_num]);
    if (sensor_short_num != num_of_shorts - 1)
      writecnt += snprintf(MessageBuffer + writecnt, MESSAGE_BUFFER_SIZE - writecnt, ",");
  }
  snprintf(MessageBuffer + writecnt, MESSAGE_BUFFER_SIZE - writecnt, "]}");
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
const char* status_cmd_cb(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', MESSAGE_BUFFER_SIZE);
//   systemStatus.GetDeviceStatus(MessageBuffer, kMessageBufferSize);
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
const char* setport_cmd_cb(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', MESSAGE_BUFFER_SIZE);
  int argBuffer[NUM_OF_I2C_PORTS];
  const ArgSpecs SetPortSpecs = {NUM_OF_I2C_PORTS, SET_PORT_ARGUMENTS_UPPER_RANGE, SET_PORT_ARGUMENTS_LOWER_RANGE};
  const bool ArgumentsAreValid = parse_entered_arguments_to_int(args, argBuffer, SetPortSpecs);
  if (ArgumentsAreValid) {
    // portAProperties.AssignSensorToI2CPort((SensorTypes)argBuffer[module::status::kSensorPortAIndex]);
    // portBProperties.AssignSensorToI2CPort((SensorTypes)argBuffer[module::status::kSensorPortBIndex]);
    snprintf(MessageBuffer, MESSAGE_BUFFER_SIZE, set_port_format_str, argBuffer[0],
             argBuffer[1], argBuffer[2]);
    return MessageBuffer;
  } else {
    return invalid_argument_values;
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
const char* setid_cmd_cb(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', MESSAGE_BUFFER_SIZE);
  int argBuffer[SET_ID_NUM_OF_ARGUMENTS];
  const ArgSpecs SetIDSpecs = {SET_ID_NUM_OF_ARGUMENTS, SET_ID_ARGUMENTS_UPPER_RANGE, SET_ID_ARGUMENTS_LOWER_RANGE};
  const uint8_t ArgumentsAreValid = parse_entered_arguments_to_int(args, argBuffer, SetIDSpecs);
  if (ArgumentsAreValid) {
    // systemStatus.SetDeviceID(argBuffer[0]);
    snprintf(MessageBuffer, MESSAGE_BUFFER_SIZE, set_id_format_str, argBuffer[0]);
    return MessageBuffer;
  } else {
    return invalid_argument_values;
  }
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
const char* setsr_cmd_cb(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', MESSAGE_BUFFER_SIZE);
  int argBuffer[SET_SR_NUM_OF_ARGUMENTS];
  const ArgSpecs SetIDSpecs = {SET_SR_NUM_OF_ARGUMENTS, SET_SR_ARGUMENTS_UPPER_RANGE, SET_SR_ARGUMENTS_LOWER_RANGE};
  const bool ArgumentsAreValid = parse_entered_arguments_to_int(args, argBuffer, SetIDSpecs);
  if (ArgumentsAreValid) {
    // portAProperties.SetSampleTime(argBuffer[0]);
    // portBProperties.SetSampleTime(argBuffer[1]);
    snprintf(MessageBuffer, MESSAGE_BUFFER_SIZE, set_sample_time_format_str, argBuffer[0], argBuffer[1]);
    return MessageBuffer;
  } else {
    return invalid_argument_values;
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
const char* stream_cmd_cb(char** args, int num_of_args) {
  memset(MessageBuffer, '\0', MESSAGE_BUFFER_SIZE);
//   SensorData data;
//   while (1) {
//     // Receive the SensorData from the message queue
//     if (xQueueReceive(serviceProtocolQueue, &(data), (TickType_t)10) == pdPASS) {
//       ComposeJsonFormattedStringOfSensorData
//     (&data);
//       return MessageBuffer;
//     }
//   }
  return "!E can't receive message from queue";
}



const char* helpcmd_cb(char** args, int num_of_args) {
  return help_usage_str;
}

const char* testcmd_cb(char** args, int num_of_args) {
  return "Hello World!";
}






usb_service_protocol::USBServiceProtocolRegisters Protoreg[NUM_OF_REGISTERS] = {{"TEST", 0, NON_STREAM_CMD, testcmd_cb},
                                                                 {"STATUS", 0, NON_STREAM_CMD, status_cmd_cb}, 
                                                                 {"SETPORT", SET_PORT_NUM_OF_ARGUMENTS, NON_STREAM_CMD, setport_cmd_cb}, 
                                                                 {"SETID", SET_ID_NUM_OF_ARGUMENTS, NON_STREAM_CMD, setid_cmd_cb},
                                                                 {"STREAM", 0, STREAM_CMD, stream_cmd_cb}, 
                                                                 {"SETSR", SET_SR_NUM_OF_ARGUMENTS, NON_STREAM_CMD, setsr_cmd_cb}, 
                                                                 {"HELP", 0, NON_STREAM_CMD, helpcmd_cb}};

} /* namespace usb_service_protocol */

#endif  /* USB_PROTOCOL_H */