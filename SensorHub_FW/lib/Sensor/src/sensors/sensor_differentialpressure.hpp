#ifndef SENSOR_DIFFERENTIALPRESSURE_H
#define SENSOR_DIFFERENTIALPRESSURE_H

#include <sensors/sensor_base.hpp>
#include <SDP810.h>

class differentialPressureSensor : public universal_sensor{
    public:
    differentialPressureSensor(i2c_peripheral_t i2c_peripheral) : universal_sensor(i2c_peripheral){
          i2c_handle = new i2c_driver(i2c_peripheral, i2c_speed_400KHz, i2c_addr);
          diffPressure = new SDP810(i2c_handle);
    }
    void initialize(){
        ;
    }
    SensorData getSensorData(){
        sensData.numOfBytes = 1;
        memset(sensData.buffer, 0, sensData.numOfBytes);
        return sensData;
    }
    void deinitialize(){
        ;
    }
    ~differentialPressureSensor(){
        deinitialize();
    }
    private:
    i2c_driver* i2c_handle;
    const uint8_t i2c_addr = 0x29;
    SDP810* diffPressure;
    SensorData sensData;
};
#endif