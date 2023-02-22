#include <sensors/sensor_fingerposition.hpp>

void fingerPositionSensor::initialize(){
   ;
}

SensorData fingerPositionSensor::getSensorData(){
   sensData.numOfBytes = 1;
   memset(sensData.buffer, 0, sensData.numOfBytes);
   return sensData;
}

void fingerPositionSensor::deinitialize(){
   ;
}

