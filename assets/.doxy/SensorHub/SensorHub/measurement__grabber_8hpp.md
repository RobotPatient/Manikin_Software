

# File measurement\_grabber.hpp



[**FileList**](files.md) **>** [**SensorHub\_FW**](dir_8a3376b0b822df0b6266211cee305325.md) **>** [**src**](dir_d67209f9e2b4e51eca02397244f6bfa8.md) **>** [**measurement\_grabber.hpp**](measurement__grabber_8hpp.md)

[Go to the source code of this file](measurement__grabber_8hpp_source.md)



* `#include <FreeRTOS.h>`
* `#include <queue.h>`
* `#include <task.h>`
* `#include <sensor_helper.hpp>`
* `#include <volume_calculations.hpp>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**module**](namespacemodule.md) <br> |
| namespace | [**measurement\_grabber**](namespacemodule_1_1measurement__grabber.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**MeasurementGrabber**](classmodule_1_1measurement__grabber_1_1MeasurementGrabber.md) <br>_MeasurementGrabberTask gives uniform interface for the sensor.. This way parameters like sampletime, measurementqueue and sensortype can be set._  |
| struct | [**MeasurementGrabberData**](structmodule_1_1measurement__grabber_1_1MeasurementGrabberData.md) <br>_Struct type used internally to pass arguments between class and MeasurementGrabberTask._  |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**dataSizePressureSensor**](measurement__grabber_8hpp.md#define-datasizepressuresensor)  4<br> |

## Macro Definition Documentation





### define dataSizePressureSensor 

```C++
#define dataSizePressureSensor 4
```




------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/measurement_grabber.hpp`

