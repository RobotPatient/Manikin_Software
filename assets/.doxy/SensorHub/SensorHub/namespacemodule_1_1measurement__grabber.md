

# Namespace module::measurement\_grabber



[**Namespace List**](namespaces.md) **>** [**module**](namespacemodule.md) **>** [**measurement\_grabber**](namespacemodule_1_1measurement__grabber.md)




















## Classes

| Type | Name |
| ---: | :--- |
| class | [**MeasurementGrabber**](classmodule_1_1measurement__grabber_1_1MeasurementGrabber.md) <br>_MeasurementGrabberTask gives uniform interface for the sensor.. This way parameters like sampletime, measurementqueue and sensortype can be set._  |
| struct | [**MeasurementGrabberData**](structmodule_1_1measurement__grabber_1_1MeasurementGrabberData.md) <br>_Struct type used internally to pass arguments between class and MeasurementGrabberTask._  |






## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr uint8\_t | [**kSensorPollingTaskStackSize**](#variable-ksensorpollingtaskstacksize)   = = 150<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**MeasurementGrabberTask**](#function-measurementgrabbertask) (void \* PvParameter) <br>_MeasurementGrabberTask which reads the sensor measurements and sends the results to the queue assigned in the_ [_**MeasurementGrabberData**_](structmodule_1_1measurement__grabber_1_1MeasurementGrabberData.md) _in the arguments (void \*PvParameter)._ |




























## Public Attributes Documentation




### variable kSensorPollingTaskStackSize 

```C++
constexpr uint8_t module::measurement_grabber::kSensorPollingTaskStackSize;
```



## Public Functions Documentation




### function MeasurementGrabberTask 

```C++
void module::measurement_grabber::MeasurementGrabberTask (
    void * PvParameter
) 
```




------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/measurement_grabber.hpp`

