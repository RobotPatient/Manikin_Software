

# Struct module::measurement\_grabber::MeasurementGrabberData



[**ClassList**](annotated.md) **>** [**module**](namespacemodule.md) **>** [**measurement\_grabber**](namespacemodule_1_1measurement__grabber.md) **>** [**MeasurementGrabberData**](structmodule_1_1measurement__grabber_1_1MeasurementGrabberData.md)



_Struct type used internally to pass arguments between class and MeasurementGrabberTask._ 

* `#include <measurement_grabber.hpp>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint8\_t | [**SampleTime**](#variable-sampletime)  <br> |
|  UniversalSensor \* | [**Sensor**](#variable-sensor)  <br> |
|  xQueueHandle \* | [**queue**](#variable-queue)  <br> |












































## Public Attributes Documentation




### variable SampleTime 

```C++
uint8_t module::measurement_grabber::MeasurementGrabberData::SampleTime;
```






### variable Sensor 

```C++
UniversalSensor* module::measurement_grabber::MeasurementGrabberData::Sensor;
```






### variable queue 

```C++
xQueueHandle* module::measurement_grabber::MeasurementGrabberData::queue;
```




------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/measurement_grabber.hpp`

