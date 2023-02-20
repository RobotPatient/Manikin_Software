#ifndef SENSOR_FINGERPOSITION_H
#define SENSOR_FINGERPOSITION_H

#include <sensors/sensor_base.hpp>
#include <ADS7138.hpp>
class fingerPositionSensor : public universal_sensor{
    public:
    fingerPositionSensor(i2c_peripheral_t i2c_peripheral) : universal_sensor(i2c_peripheral){
          //static i2c_driver i2c_handle = i2c_driver(i2c_peripheral, i2c_speed_400KHz, i2c_addr);
          //ADS = new ADS7138(&i2c_handle);
    }
    void initialize(){
        ;
    }
    SensorData getSensorData(){
        sensData.numOfBytes = 8;
        memset(sensData.buffer, 0, sensData.numOfBytes);
        return sensData;
    }
    void deinitialize(){
        ;
    }
    ~fingerPositionSensor(){
        deinitialize();
    }
    private:  
    SensorData sensData;
    const uint8_t i2c_addr = 0x29;
    ADS7138* ADS;
};
#endif