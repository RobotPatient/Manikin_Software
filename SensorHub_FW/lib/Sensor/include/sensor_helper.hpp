#ifndef SENSOR_HELPER_H
#define SENSOR_HELPER_H

#include <sensors/sensor_base.hpp>
#include <sensors/sensor_compression.hpp>
#include <sensors/sensor_differentialpressure.hpp>
#include <sensors/sensor_fingerposition.hpp>

class sensor{
    public:
     sensor(i2c_peripheral_t i2c_peripheral) : peripheral(i2c_peripheral){
            fingerPosition = new fingerPositionSensor(peripheral);
            diffPressure = new differentialPressureSensor(peripheral);
            compression = new compressionSensor(peripheral);
     }

    void setSensorType(SensorTypes sensorType){
        if(_sensor != nullptr)
            _sensor->deinitialize();
     switch(sensorType){
         case Sensor_compression: 
         _sensor = compression;
         break;
         case Sensor_differentialpressure:
         _sensor = diffPressure;
         break;
         case Sensor_fingerposition:
         _sensor = fingerPosition;
         break;
    }
     }
    private:
    i2c_peripheral_t peripheral;
    fingerPositionSensor* fingerPosition;
    differentialPressureSensor* diffPressure;
    compressionSensor* compression;
    universal_sensor* _sensor = nullptr;

};

#endif