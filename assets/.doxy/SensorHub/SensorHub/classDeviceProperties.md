

# Class DeviceProperties



[**ClassList**](annotated.md) **>** [**DeviceProperties**](classDeviceProperties.md)





* `#include <device_properties.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**AssignSensorToI2CPort**](#function-assignsensortoi2cport) (uint8\_t Device) <br>_Change the sensor._  |
|  uint8\_t | [**GetSampleTime**](#function-getsampletime) () <br>_Get the current sample rate._  |
|  uint8\_t | [**GetSensorType**](#function-getsensortype) () <br>_Get the current selected sensortype._  |
|  void | [**Init**](#function-init) (UniversalSensor \* obj\_pool) <br>_Init and add the SensorObjPool to the_ [_**DeviceProperties**_](classDeviceProperties.md) _._ |
|  void | [**PauseSensor**](#function-pausesensor) () <br>_Suspend sensor measurements polling task._  |
|  void | [**ResumeSensor**](#function-resumesensor) () <br>_Resume sensor measurements after polling task is suspended._  |
|  void | [**SetSampleTime**](#function-setsampletime) (const uint8\_t sample\_time) <br>_Set the sample rate of the polling task in ms._  |
|  void | [**SetupI2C**](#function-setupi2c) (I2CDriver \* i2c\_handle, QueueHandle\_t \* SensorQueue) <br>_Link the i2c\_handle and SensorQueue to internall shadow copies.. This way the sensors can be allocated using these handles!_  |




























## Public Functions Documentation




### function AssignSensorToI2CPort 

_Change the sensor._ 
```C++
inline void DeviceProperties::AssignSensorToI2CPort (
    uint8_t Device
) 
```





**Parameters:**


* `Device` The Sensortype to change to 




        



### function GetSampleTime 

_Get the current sample rate._ 
```C++
inline uint8_t DeviceProperties::GetSampleTime () 
```





**Returns:**

sample rate in milliseconds 





        



### function GetSensorType 

_Get the current selected sensortype._ 
```C++
inline uint8_t DeviceProperties::GetSensorType () 
```





**Returns:**

The sensortype 





        



### function Init 

_Init and add the SensorObjPool to the_ [_**DeviceProperties**_](classDeviceProperties.md) _._
```C++
inline void DeviceProperties::Init (
    UniversalSensor * obj_pool
) 
```





**Parameters:**


* `obj_pool` Array with size defined in [**device\_sensor\_lib\_settings.hpp**](device__sensor__lib__settings_8hpp.md) containing static sensor obj allocations 




        



### function PauseSensor 

```C++
inline void DeviceProperties::PauseSensor () 
```






### function ResumeSensor 

```C++
inline void DeviceProperties::ResumeSensor () 
```






### function SetSampleTime 

_Set the sample rate of the polling task in ms._ 
```C++
inline void DeviceProperties::SetSampleTime (
    const uint8_t sample_time
) 
```





**Parameters:**


* `sample_time` the sample rate to set in milliseconds. 




        



### function SetupI2C 

_Link the i2c\_handle and SensorQueue to internall shadow copies.. This way the sensors can be allocated using these handles!_ 
```C++
inline void DeviceProperties::SetupI2C (
    I2CDriver * i2c_handle,
    QueueHandle_t * SensorQueue
) 
```





**Parameters:**


* `i2c_handle` The handle to a I2CDriver instance 
* `SensorQueue` The handle to the measurement queue to post sensorvalue's in 




        

------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/device_properties.hpp`

