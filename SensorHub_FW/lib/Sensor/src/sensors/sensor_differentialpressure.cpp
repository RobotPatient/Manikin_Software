#include <sensors/sensor_differentialpressure.hpp>

void differentialPressureSensor::initialize(){
    ;
}

SensorData differentialPressureSensor::getSensorData(){
   sensData.numOfBytes = 1;
   memset(sensData.buffer, 0, sensData.numOfBytes);
   return sensData;
}

void differentialPressureSensor::deinitialize(){
    ;
}

