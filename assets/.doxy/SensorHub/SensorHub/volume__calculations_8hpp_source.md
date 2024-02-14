

# File volume\_calculations.hpp

[**File List**](files.md) **>** [**SensorHub\_FW**](dir_8a3376b0b822df0b6266211cee305325.md) **>** [**src**](dir_d67209f9e2b4e51eca02397244f6bfa8.md) **>** [**volume\_calculations.hpp**](volume__calculations_8hpp.md)

[Go to the documentation of this file](volume__calculations_8hpp.md)

```C++

#ifndef VOLUME_CALCULATIONS_HPP
#define VOLUME_CALCULATIONS_HPP
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#define M3TOCM3_FACTOR 1e6
#define INTERVAL_TO_FREQ 1000
#define AIR_WEIGHT 1.20
#define AIR_DRAG 0.61
#define VOLUME_CALC_CONSTANT 0.94  // sqrt(1-(hose-orifice ratio)^4)
#define ORIFICE_SURFACE 7.069e-6
#define MAX_PROCESSABLE_SENSOR_VAL 500
#define CORRECTION_FACTOR 1.78
#define FLOW_ERROR 2000

static uint8_t timeDifference;
static uint32_t stopTime;
double sampleVolume;
static double totalVolume;

uint16_t CalculateTotalVolume(uint16_t differentialPressure);
double CalculateAirBlownInLungs(uint16_t differentialPressure);
double sqrt(double input);

uint16_t CalculateTotalVolume(uint16_t differentialPressure) {
  if (differentialPressure == 0) {
    totalVolume = 0;
  } else if (differentialPressure > 0) {
    timeDifference = xTaskGetTickCount() - stopTime;
    // calculate the amount of air blown into the lungs since the last sample
    sampleVolume = CalculateAirBlownInLungs(differentialPressure);
    totalVolume = totalVolume + sampleVolume;
  } else if (differentialPressure >= MAX_PROCESSABLE_SENSOR_VAL) {
    totalVolume = FLOW_ERROR;  // error, the differential pressure was to high to precisely measure the air flow
  }
  stopTime = xTaskGetTickCount();
  return totalVolume;
}

double CalculateAirBlownInLungs(uint16_t differentialPressure) {
  // calculate the air volume based on NEN-EN-ISO 5167-2
  double airVolume =
      ((AIR_DRAG / VOLUME_CALC_CONSTANT * ORIFICE_SURFACE * sqrt(2 * differentialPressure * AIR_WEIGHT)) *
       CORRECTION_FACTOR * M3TOCM3_FACTOR) /
      (INTERVAL_TO_FREQ / timeDifference);
  return airVolume;
}

double sqrt(double input) {
  const double error = 0.1;  // precision
  double approxSquareRoot = input;

  while ((approxSquareRoot - input / approxSquareRoot) > error)  // loop until precision satisfied
  {
    approxSquareRoot = (approxSquareRoot + input / approxSquareRoot) / 2;
  }
  return approxSquareRoot;
}
#endif

```

