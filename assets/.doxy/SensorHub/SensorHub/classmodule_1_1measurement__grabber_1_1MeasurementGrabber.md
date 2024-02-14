

# Class module::measurement\_grabber::MeasurementGrabber



[**ClassList**](annotated.md) **>** [**module**](namespacemodule.md) **>** [**measurement\_grabber**](namespacemodule_1_1measurement__grabber.md) **>** [**MeasurementGrabber**](classmodule_1_1measurement__grabber_1_1MeasurementGrabber.md)



_MeasurementGrabberTask gives uniform interface for the sensor.. This way parameters like sampletime, measurementqueue and sensortype can be set._ 

* `#include <measurement_grabber.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**ChangeSensor**](#function-changesensor) (UniversalSensor \* Sensor) <br>_Change the current sensor to a new given one._  |
|  uint8\_t | [**GetSampleTime**](#function-getsampletime) () <br>_Get the currently set sampletime._  |
|  void | [**ResumePollingTask**](#function-resumepollingtask) () <br>_Resume the created polling task._  |
|  void | [**SetSampleTime**](#function-setsampletime) (const uint8\_t SampleTime) <br>_Set a new sampletime._  |
|  void | [**SetupPollTask**](#function-setuppolltask) (UniversalSensor \* Sensor, uint8\_t SampleTime, xQueueHandle \* queue, xTaskHandle \* Task) <br>_Initialize the polling task for reading the sensor with set sample time.._  |
|  void | [**SuspendPollingTask**](#function-suspendpollingtask) () <br>_Suspend the created polling task._  |




























## Public Functions Documentation




### function ChangeSensor 

```C++
inline void module::measurement_grabber::MeasurementGrabber::ChangeSensor (
    UniversalSensor * Sensor
) 
```






### function GetSampleTime 

```C++
inline uint8_t module::measurement_grabber::MeasurementGrabber::GetSampleTime () 
```






### function ResumePollingTask 

```C++
inline void module::measurement_grabber::MeasurementGrabber::ResumePollingTask () 
```






### function SetSampleTime 

```C++
inline void module::measurement_grabber::MeasurementGrabber::SetSampleTime (
    const uint8_t SampleTime
) 
```






### function SetupPollTask 

```C++
inline void module::measurement_grabber::MeasurementGrabber::SetupPollTask (
    UniversalSensor * Sensor,
    uint8_t SampleTime,
    xQueueHandle * queue,
    xTaskHandle * Task
) 
```






### function SuspendPollingTask 

```C++
inline void module::measurement_grabber::MeasurementGrabber::SuspendPollingTask () 
```




------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/measurement_grabber.hpp`

