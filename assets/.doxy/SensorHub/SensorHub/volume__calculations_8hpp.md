

# File volume\_calculations.hpp



[**FileList**](files.md) **>** [**SensorHub\_FW**](dir_8a3376b0b822df0b6266211cee305325.md) **>** [**src**](dir_d67209f9e2b4e51eca02397244f6bfa8.md) **>** [**volume\_calculations.hpp**](volume__calculations_8hpp.md)

[Go to the source code of this file](volume__calculations_8hpp_source.md)



* `#include <FreeRTOS.h>`
* `#include <queue.h>`
* `#include <task.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  double | [**sampleVolume**](#variable-samplevolume)  <br> |


## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  uint32\_t | [**stopTime**](#variable-stoptime)  <br> |
|  uint8\_t | [**timeDifference**](#variable-timedifference)  <br> |
|  double | [**totalVolume**](#variable-totalvolume)  <br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  double | [**CalculateAirBlownInLungs**](#function-calculateairblowninlungs) (uint16\_t differentialPressure) <br> |
|  uint16\_t | [**CalculateTotalVolume**](#function-calculatetotalvolume) (uint16\_t differentialPressure) <br> |
|  double | [**sqrt**](#function-sqrt) (double input) <br> |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**AIR\_DRAG**](volume__calculations_8hpp.md#define-air_drag)  0.61<br> |
| define  | [**AIR\_WEIGHT**](volume__calculations_8hpp.md#define-air_weight)  1.20<br> |
| define  | [**CORRECTION\_FACTOR**](volume__calculations_8hpp.md#define-correction_factor)  1.78<br> |
| define  | [**FLOW\_ERROR**](volume__calculations_8hpp.md#define-flow_error)  2000<br> |
| define  | [**INTERVAL\_TO\_FREQ**](volume__calculations_8hpp.md#define-interval_to_freq)  1000<br> |
| define  | [**M3TOCM3\_FACTOR**](volume__calculations_8hpp.md#define-m3tocm3_factor)  1e6<br> |
| define  | [**MAX\_PROCESSABLE\_SENSOR\_VAL**](volume__calculations_8hpp.md#define-max_processable_sensor_val)  500<br> |
| define  | [**ORIFICE\_SURFACE**](volume__calculations_8hpp.md#define-orifice_surface)  7.069e-6<br> |
| define  | [**VOLUME\_CALC\_CONSTANT**](volume__calculations_8hpp.md#define-volume_calc_constant)  0.94<br> |

## Public Attributes Documentation




### variable sampleVolume 

```C++
double sampleVolume;
```



## Public Static Attributes Documentation




### variable stopTime 

```C++
uint32_t stopTime;
```






### variable timeDifference 

```C++
uint8_t timeDifference;
```






### variable totalVolume 

```C++
double totalVolume;
```



## Public Functions Documentation




### function CalculateAirBlownInLungs 

```C++
double CalculateAirBlownInLungs (
    uint16_t differentialPressure
) 
```






### function CalculateTotalVolume 

```C++
uint16_t CalculateTotalVolume (
    uint16_t differentialPressure
) 
```






### function sqrt 

```C++
double sqrt (
    double input
) 
```



## Macro Definition Documentation





### define AIR\_DRAG 

```C++
#define AIR_DRAG 0.61
```






### define AIR\_WEIGHT 

```C++
#define AIR_WEIGHT 1.20
```






### define CORRECTION\_FACTOR 

```C++
#define CORRECTION_FACTOR 1.78
```






### define FLOW\_ERROR 

```C++
#define FLOW_ERROR 2000
```






### define INTERVAL\_TO\_FREQ 

```C++
#define INTERVAL_TO_FREQ 1000
```






### define M3TOCM3\_FACTOR 

```C++
#define M3TOCM3_FACTOR 1e6
```






### define MAX\_PROCESSABLE\_SENSOR\_VAL 

```C++
#define MAX_PROCESSABLE_SENSOR_VAL 500
```






### define ORIFICE\_SURFACE 

```C++
#define ORIFICE_SURFACE 7.069e-6
```






### define VOLUME\_CALC\_CONSTANT 

```C++
#define VOLUME_CALC_CONSTANT 0.94
```




------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/volume_calculations.hpp`

