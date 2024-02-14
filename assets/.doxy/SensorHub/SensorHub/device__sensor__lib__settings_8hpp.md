

# File device\_sensor\_lib\_settings.hpp



[**FileList**](files.md) **>** [**SensorHub\_FW**](dir_8a3376b0b822df0b6266211cee305325.md) **>** [**src**](dir_d67209f9e2b4e51eca02397244f6bfa8.md) **>** [**device\_sensor\_lib\_settings.hpp**](device__sensor__lib__settings_8hpp.md)

[Go to the source code of this file](device__sensor__lib__settings_8hpp_source.md)



* `#include <stdint.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**SensorTypes**](#enum-sensortypes)  <br>_The currently supported sensors._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr uint8\_t | [**kNumOfSupportedSensors**](#variable-knumofsupportedsensors)   = = 4<br>_The number of sensors supported!_  |












































## Public Types Documentation




### enum SensorTypes 

```C++
enum SensorTypes {
    TypeNone,
    TypeCompressionSensor,
    TypeDifferentialPressureSensor,
    TypefingerPositionSensor
};
```



## Public Attributes Documentation




### variable kNumOfSupportedSensors 

_The number of sensors supported!_ 
```C++
constexpr uint8_t kNumOfSupportedSensors;
```





**Note:**

This needs to be changed when a new sensors is added! 





        

------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/device_sensor_lib_settings.hpp`

