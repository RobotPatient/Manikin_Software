

# File service\_protocol.hpp



[**FileList**](files.md) **>** [**SensorHub\_FW**](dir_8a3376b0b822df0b6266211cee305325.md) **>** [**src**](dir_d67209f9e2b4e51eca02397244f6bfa8.md) **>** [**service\_protocol.hpp**](service__protocol_8hpp.md)

[Go to the source code of this file](service__protocol_8hpp_source.md)



* `#include <Arduino.h>`
* `#include <FreeRTOS.h>`
* `#include <queue.h>`
* `#include <device_settings.hpp>`
* `#include <device_status.hpp>`
* `#include <ringbuffer.hpp>`
* `#include <usb_service_protocol.hpp>`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**ArgSpecs**](structArgSpecs.md) <br>_Struct used to pass options for argument parsing between methods._  |






## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr uint8\_t | [**ParseFail**](#variable-parsefail)   = = 0<br> |
|  constexpr uint8\_t | [**ParseOK**](#variable-parseok)   = = 1<br> |
|  const char | [**kHelpUsageString**](#variable-khelpusagestring)   = = "\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*HELP\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*"
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
                                "                                         port a in b in milliseconds (10-1000 ms)\r"<br> |
|  const char | [**kInvalidArgumentValues**](#variable-kinvalidargumentvalues)   = = "!E Invalid arguments entered!"<br> |
|  constexpr int | [**kLowerRangeArgSetID**](#variable-klowerrangeargsetid)   = = 0<br> |
|  constexpr int | [**kLowerRangeArgSetPort**](#variable-klowerrangeargsetport)   = = 0<br> |
|  constexpr int | [**kLowerRangeArgSetSR**](#variable-klowerrangeargsetsr)   = = 10<br> |
|  constexpr int | [**kMessageBufferSize**](#variable-kmessagebuffersize)   = = 1024<br>_Statically assigned (output) messagebuffer, used to store the output of commands in. Gets used by multiple callback methods._  |
|  constexpr uint8\_t | [**kNumOfArgumentsSetID**](#variable-knumofargumentssetid)   = = 1<br> |
|  constexpr uint8\_t | [**kNumOfArgumentsSetPort**](#variable-knumofargumentssetport)   = = 3<br>_Argument parser constants._  |
|  constexpr uint8\_t | [**kNumOfArgumentsSetSR**](#variable-knumofargumentssetsr)   = = 2<br> |
|  constexpr uint8\_t | [**kNumOfRegisters**](#variable-knumofregisters)   = = 7<br>_The num of registers defined in the usb service protocol registers array._  |
|  const char | [**kSetIDFormatString**](#variable-ksetidformatstring)   = = "!OK Device id is set to: %d"<br> |
|  const char | [**kSetPortFormatString**](#variable-ksetportformatstring)   = = "!OK Port A set to: %d, Port B set to: %d, Port BB set to: %d"<br> |
|  const char | [**kSetSampleTimeFormatString**](#variable-ksetsampletimeformatstring)   = = "!OK Sampletime on Port A set to: %d, Port B set to: %d"<br> |
|  const char | [**kStreamDataFormatString**](#variable-kstreamdataformatstring)   = = "{ \"Val\": %d}"<br> |
|  const char | [**kStreamFormatString**](#variable-kstreamformatstring)   = = "{\"NumOfShorts\": %d, \"SampleNum\": %d, \"Sensor\": %d, \"Buf\": ["<br>_Format strings, used to create the responses with!_  |
|  constexpr int | [**kUpperRangeArgSetID**](#variable-kupperrangeargsetid)   = = 255<br> |
|  constexpr int | [**kUpperRangeArgSetPort**](#variable-kupperrangeargsetport)   = = [**kNumOfSupportedSensors**](device__sensor__lib__settings_8hpp.md#variable-knumofsupportedsensors)-1<br> |
|  constexpr int | [**kUpperRangeArgSetSR**](#variable-kupperrangeargsetsr)   = = 1000<br> |


## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  char | [**MessageBuffer**](#variable-messagebuffer)  <br> |
|  usb\_service\_protocol::USBServiceProtocolRegisters | [**USBRegisters**](#variable-usbregisters)   = { {"STATUS", 0, false, [**CMD\_STATUS\_CB**](service__protocol_8hpp.md#function-cmd_status_cb)}, 
                                                                                        {"SETPORT", kNumOfArgumentsSetPort, false, [**CMD\_SETPORT\_CB**](service__protocol_8hpp.md#function-cmd_setport_cb)}, 
                                                                                        {"SETID", kNumOfArgumentsSetID, false, [**CMD\_SETID\_CB**](service__protocol_8hpp.md#function-cmd_setid_cb)},
                                                                                        {"STREAM", 0, true, [**CMD\_STREAM\_CB**](service__protocol_8hpp.md#function-cmd_stream_cb)}, 
                                                                                        {"SETSR", kNumOfArgumentsSetSR, false, [**CMD\_SETSR\_CB**](service__protocol_8hpp.md#function-cmd_setsr_cb)}, 
                                                                                        {"HELP", 0, false, [**CMD\_HELP\_CB**](service__protocol_8hpp.md#function-cmd_help_cb)}}<br>_STATIC declaration of all the CB methods and command information._  |














## Public Functions

| Type | Name |
| ---: | :--- |
|  const char \* | [**CMD\_HELP\_CB**](#function-cmd_help_cb) (char \*\* args, int num\_of\_args) <br>_Callback function for the HELP command. This function will be ran when HELP command is entered. The HELP command return the usage of the console in a formatted string._  |
|  const char \* | [**CMD\_SETID\_CB**](#function-cmd_setid_cb) (char \*\* args, int num\_of\_args) <br>_Callback function for the SETID command. This function will be ran when SETID command is entered. The SETID command sets an unique identifier for this subsystem (ID will be written to external flash!). ID can be read using STATUS command!_  |
|  const char \* | [**CMD\_SETPORT\_CB**](#function-cmd_setport_cb) (char \*\* args, int num\_of\_args) <br>_Callback function for the SETPORT command. This function will be ran when SETPORT command is entered. The SETPORT command sets the sensortypes for the given ports.._  |
|  const char \* | [**CMD\_SETSR\_CB**](#function-cmd_setsr_cb) (char \*\* args, int num\_of\_args) <br>_Callback function for the SETSR command. This function will be ran when SETSR command is entered. The SETSR command sets the sample rate for the sensorports._  |
|  const char \* | [**CMD\_STATUS\_CB**](#function-cmd_status_cb) (char \*\* args, int num\_of\_args) <br>_Callback function for the STATUS command. This function will be ran when STATUS command is entered. The STATUS command return the status of the system in a formatted JSON string._  |
|  const char \* | [**CMD\_STREAM\_CB**](#function-cmd_stream_cb) (char \*\* args, int num\_of\_args) <br>_Callback function for the STREAM command. This function will be ran when STREAM command is entered. This command is an stream command, meaning it will run repeatedly until \r (ENTER) is detected in serial console. The Stream command returns the sensordata of each sensor in a formatted json string._  |
|  void | [**ComposeJsonFormattedStringOfSensorData**](#function-composejsonformattedstringofsensordata) (SensorData \* data) <br>_This function will compose a Json formatted string of the sensordata and put it in to the messagebuffer._  |
|  uint8\_t | [**ParseEnteredArgumentsToInt**](#function-parseenteredargumentstoint) (char \*\* argument, int \* buffer, const [**ArgSpecs**](structArgSpecs.md) ArgSpec) <br>_This function will convert the string type arguments to an integer and check if the argument value falls within the given Argspec range._  |




























## Public Attributes Documentation




### variable ParseFail 

```C++
constexpr uint8_t ParseFail;
```






### variable ParseOK 

```C++
constexpr uint8_t ParseOK;
```






### variable kHelpUsageString 

```C++
const char kHelpUsageString[];
```






### variable kInvalidArgumentValues 

```C++
const char kInvalidArgumentValues[];
```






### variable kLowerRangeArgSetID 

```C++
constexpr int kLowerRangeArgSetID;
```






### variable kLowerRangeArgSetPort 

```C++
constexpr int kLowerRangeArgSetPort;
```






### variable kLowerRangeArgSetSR 

```C++
constexpr int kLowerRangeArgSetSR;
```






### variable kMessageBufferSize 

```C++
constexpr int kMessageBufferSize;
```






### variable kNumOfArgumentsSetID 

```C++
constexpr uint8_t kNumOfArgumentsSetID;
```






### variable kNumOfArgumentsSetPort 

```C++
constexpr uint8_t kNumOfArgumentsSetPort;
```






### variable kNumOfArgumentsSetSR 

```C++
constexpr uint8_t kNumOfArgumentsSetSR;
```






### variable kNumOfRegisters 

```C++
constexpr uint8_t kNumOfRegisters;
```






### variable kSetIDFormatString 

```C++
const char kSetIDFormatString[];
```






### variable kSetPortFormatString 

```C++
const char kSetPortFormatString[];
```






### variable kSetSampleTimeFormatString 

```C++
const char kSetSampleTimeFormatString[];
```






### variable kStreamDataFormatString 

```C++
const char kStreamDataFormatString[];
```






### variable kStreamFormatString 

```C++
const char kStreamFormatString[];
```






### variable kUpperRangeArgSetID 

```C++
constexpr int kUpperRangeArgSetID;
```






### variable kUpperRangeArgSetPort 

```C++
constexpr int kUpperRangeArgSetPort;
```






### variable kUpperRangeArgSetSR 

```C++
constexpr int kUpperRangeArgSetSR;
```



## Public Static Attributes Documentation




### variable MessageBuffer 

```C++
char MessageBuffer[kMessageBufferSize];
```






### variable USBRegisters 

_STATIC declaration of all the CB methods and command information._ 
```C++
usb_service_protocol::USBServiceProtocolRegisters USBRegisters[kNumOfRegisters];
```





**Note:**

The format of the struct is { "String of the command", Number of Arguments, Command is stream command, Pointer to callback method} 




**Note:**

Stream command means that command will be ran repeatedly when entered. Command can be stopped by pressing enter key or '\r' character 





        
## Public Functions Documentation




### function CMD\_HELP\_CB 

_Callback function for the HELP command. This function will be ran when HELP command is entered. The HELP command return the usage of the console in a formatted string._ 
```C++
const char * CMD_HELP_CB (
    char ** args,
    int num_of_args
) 
```





**Parameters:**


* `args` A two dimensional array containing the arguments entered after the command 
* `num_of_args` The number of arguments entered after the command



**Returns:**

Command response string, which will be a formatted string explaining how to use this console! 





        



### function CMD\_SETID\_CB 

_Callback function for the SETID command. This function will be ran when SETID command is entered. The SETID command sets an unique identifier for this subsystem (ID will be written to external flash!). ID can be read using STATUS command!_ 
```C++
const char * CMD_SETID_CB (
    char ** args,
    int num_of_args
) 
```





**Parameters:**


* `args` A two dimensional array containing the arguments entered after the command 
* `num_of_args` The number of arguments entered after the command



**Returns:**

Command response string, which will be a !OK ..[confirmation of entered arguments] if arguments where valid numbers !E Invalid arguments entered if the arguments where not valid! 





        



### function CMD\_SETPORT\_CB 

_Callback function for the SETPORT command. This function will be ran when SETPORT command is entered. The SETPORT command sets the sensortypes for the given ports.._ 
```C++
const char * CMD_SETPORT_CB (
    char ** args,
    int num_of_args
) 
```





**Parameters:**


* `args` A two dimensional array containing the arguments entered after the command 
* `num_of_args` The number of arguments entered after the command



**Returns:**

Command response string, which will be a !OK ..[confirmation of entered arguments] if arguments where valid numbers !E Invalid arguments entered if the arguments where not valid! 





        



### function CMD\_SETSR\_CB 

_Callback function for the SETSR command. This function will be ran when SETSR command is entered. The SETSR command sets the sample rate for the sensorports._ 
```C++
const char * CMD_SETSR_CB (
    char ** args,
    int num_of_args
) 
```





**Parameters:**


* `args` A two dimensional array containing the arguments entered after the command 
* `num_of_args` The number of arguments entered after the command



**Returns:**

Command response string, which will be a !OK ..[confirmation of entered arguments] if arguments where valid numbrs !E Invalid arguments entered if the arguments where not valid! 





        



### function CMD\_STATUS\_CB 

_Callback function for the STATUS command. This function will be ran when STATUS command is entered. The STATUS command return the status of the system in a formatted JSON string._ 
```C++
const char * CMD_STATUS_CB (
    char ** args,
    int num_of_args
) 
```





**Parameters:**


* `args` A two dimensional array containing the arguments entered after the command 
* `num_of_args` The number of arguments entered after the command



**Returns:**

Command response string, which will be a json formatted status string. 





        



### function CMD\_STREAM\_CB 

_Callback function for the STREAM command. This function will be ran when STREAM command is entered. This command is an stream command, meaning it will run repeatedly until \r (ENTER) is detected in serial console. The Stream command returns the sensordata of each sensor in a formatted json string._ 
```C++
const char * CMD_STREAM_CB (
    char ** args,
    int num_of_args
) 
```





**Parameters:**


* `args` A two dimensional array containing the arguments entered after the command 
* `num_of_args` The number of arguments entered after the command



**Returns:**

Command response string, which will be a json formatted string! 





        



### function ComposeJsonFormattedStringOfSensorData 

_This function will compose a Json formatted string of the sensordata and put it in to the messagebuffer._ 
```C++
void ComposeJsonFormattedStringOfSensorData (
    SensorData * data
) 
```





**Parameters:**


* `data` The SensorData to convert to Json String 




        



### function ParseEnteredArgumentsToInt 

_This function will convert the string type arguments to an integer and check if the argument value falls within the given Argspec range._ 
```C++
uint8_t ParseEnteredArgumentsToInt (
    char ** argument,
    int * buffer,
    const ArgSpecs ArgSpec
) 
```





**Parameters:**


* `argument` An two dimensional array containing the string argument values. 
* `buffer` An integer buffer to save the parsed arguments to. 
* `ArgSpec` A struct containing the number of arguments to parse, the permitted max value of the argument and the permitted min value of the argument 



**Returns:**

1 on Succes! 0 on Fail!




**Note:**

All arguments are checked with the same upper and lower range from the ArgSpec struct! 





        

------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/service_protocol.hpp`

