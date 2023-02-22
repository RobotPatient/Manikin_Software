#include <sensors/sensor_compression.hpp>

void compressionSensor::initialize(){
   ;
}

SensorData compressionSensor::getSensorData(){
   sensData.numOfBytes = 1;
   memset(sensData.buffer, 0, sensData.numOfBytes);
   return sensData;
}

void compressionSensor::deinitialize(){
   ;
}

