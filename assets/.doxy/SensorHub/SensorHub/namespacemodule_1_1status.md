

# Namespace module::status



[**Namespace List**](namespaces.md) **>** [**module**](namespacemodule.md) **>** [**status**](namespacemodule_1_1status.md)




















## Classes

| Type | Name |
| ---: | :--- |
| class | [**DeviceStatus**](classmodule_1_1status_1_1DeviceStatus.md) <br> |






## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr uint8\_t | [**kBBPortIndex**](#variable-kbbportindex)   = = 2<br> |
|  constexpr uint8\_t | [**kDeviceInitialized**](#variable-kdeviceinitialized)   = = 1<br>_Integers are easier than strings to compare. Therefore these integers are macro's to select the above listed strings._  |
|  constexpr uint8\_t | [**kDeviceUnitialized**](#variable-kdeviceunitialized)   = = 0<br> |
|  constexpr uint8\_t | [**kIDLocationInFlash**](#variable-kidlocationinflash)   = = 0<br>_The flash cursor position for the ID data._  |
|  constexpr const char | [**kInitializedStatus**](#variable-kinitializedstatus)   = = "Initialized"<br> |
|  constexpr uint8\_t | [**kNumOfSensorPorts**](#variable-knumofsensorports)   = = 2<br> |
|  constexpr uint8\_t | [**kSensorPortAIndex**](#variable-ksensorportaindex)   = = 0<br> |
|  constexpr uint8\_t | [**kSensorPortBIndex**](#variable-ksensorportbindex)   = = 1<br> |
|  constexpr const char | [**kStatusFormatString**](#variable-kstatusformatstring)   = =
    "{\"Status\": \"%s\", \"DeviceType\": %d, \"DeviceID:\": %d, "
    "\"PortASenType\": %d, "
    "\"PortBSenType\": %d, \"PortASampleRate\": %d, \"PortBSampleRate\": %d}"<br>_The string used by snprintf to fill the buffer with a fixed format. This will provide the response of the Status command!_  |
|  constexpr const char | [**kUninitializedStatus**](#variable-kuninitializedstatus)   = = "Unitialized"<br>_The strings which will be inserted in to the format string depending on whether the system is initialized or not._  |












































## Public Attributes Documentation




### variable kBBPortIndex 

```C++
constexpr uint8_t module::status::kBBPortIndex;
```






### variable kDeviceInitialized 

```C++
constexpr uint8_t module::status::kDeviceInitialized;
```






### variable kDeviceUnitialized 

```C++
constexpr uint8_t module::status::kDeviceUnitialized;
```






### variable kIDLocationInFlash 

```C++
constexpr uint8_t module::status::kIDLocationInFlash;
```






### variable kInitializedStatus 

```C++
constexpr const char module::status::kInitializedStatus[];
```






### variable kNumOfSensorPorts 

```C++
constexpr uint8_t module::status::kNumOfSensorPorts;
```






### variable kSensorPortAIndex 

```C++
constexpr uint8_t module::status::kSensorPortAIndex;
```






### variable kSensorPortBIndex 

```C++
constexpr uint8_t module::status::kSensorPortBIndex;
```






### variable kStatusFormatString 

```C++
constexpr const char module::status::kStatusFormatString[];
```






### variable kUninitializedStatus 

```C++
constexpr const char module::status::kUninitializedStatus[];
```




------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/device_status.hpp`

