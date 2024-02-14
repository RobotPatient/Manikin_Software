

# Class module::status::DeviceStatus



[**ClassList**](annotated.md) **>** [**module**](namespacemodule.md) **>** [**status**](namespacemodule_1_1status.md) **>** [**DeviceStatus**](classmodule_1_1status_1_1DeviceStatus.md)





* `#include <device_status.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**AddDeviceManager**](#function-adddevicemanager) ([**DeviceProperties**](classDeviceProperties.md) \* device\_properties) <br>_Add the device properties classes, as this provides easier access to the properties like sample rate, sensortype's etc. It will add this to an internal array._  |
|  void | [**AttachFlashLogger**](#function-attachflashlogger) (hal::log::Logger \* log\_module) <br>_Attach a logger object to this class. So the DeviceID functions can read and write the id to/from flash._  |
|  uint8\_t | [**GetDeviceID**](#function-getdeviceid) () <br>_Get the unique device id (read from flash)_  |
|  void | [**GetDeviceStatus**](#function-getdevicestatus) (char \* write\_buffer, const uint32\_t write\_buffer\_size) <br>_Get the device status, collected from other functions of this class and the_ [_**DeviceProperties**_](classDeviceProperties.md) _class._ |
|  void | [**SetDeviceID**](#function-setdeviceid) (const uint8\_t ID) <br>_Set the unique device id (will write it to flash memory)_  |
|  void | [**SetDeviceType**](#function-setdevicetype) ([**DeviceTypes**](device__status_8hpp.md#enum-devicetypes) device\_type) <br>_Set the device type (SensorHub, ActuatorHub, etc.)_  |




























## Public Functions Documentation




### function AddDeviceManager 

_Add the device properties classes, as this provides easier access to the properties like sample rate, sensortype's etc. It will add this to an internal array._ 
```C++
inline void module::status::DeviceStatus::AddDeviceManager (
    DeviceProperties * device_properties
) 
```





**Parameters:**


* `device_properties` A pointer to a device\_properties instance. 




        



### function AttachFlashLogger 

_Attach a logger object to this class. So the DeviceID functions can read and write the id to/from flash._ 
```C++
inline void module::status::DeviceStatus::AttachFlashLogger (
    hal::log::Logger * log_module
) 
```





**Parameters:**


* `log_module` A pointer to a logger instance 




        



### function GetDeviceID 

_Get the unique device id (read from flash)_ 
```C++
inline uint8_t module::status::DeviceStatus::GetDeviceID () 
```





**Returns:**

The ID read from flash 





        



### function GetDeviceStatus 

_Get the device status, collected from other functions of this class and the_ [_**DeviceProperties**_](classDeviceProperties.md) _class._
```C++
inline void module::status::DeviceStatus::GetDeviceStatus (
    char * write_buffer,
    const uint32_t write_buffer_size
) 
```





**Parameters:**


* `write_buffer` The write\_buffer to write the parsed status string in to! 
* `write_buffer_size` The size of the write\_buffer array passed in to the arguments 




        



### function SetDeviceID 

_Set the unique device id (will write it to flash memory)_ 
```C++
inline void module::status::DeviceStatus::SetDeviceID (
    const uint8_t ID
) 
```





**Parameters:**


* `ID` The ID to write to flash/set 




        



### function SetDeviceType 

_Set the device type (SensorHub, ActuatorHub, etc.)_ 
```C++
inline void module::status::DeviceStatus::SetDeviceType (
    DeviceTypes device_type
) 
```





**Parameters:**


* `device_type` The device type to set (see the above listed DeviceTypes enum) 




        

------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/device_status.hpp`

