#ifndef STATUS_HPP
#define STATUS_HPP
#include <stdint.h>
#include <device_settings.hpp>
#include <measurement_grabber.hpp>

typedef enum {
UNDEFINED,
SENSORHUB,
ACTUATORHUB
} DeviceTypes;


typedef enum {
    TypeCompressionSensor,
    TypeDifferentialPressureSensor,
    TypefingerPositionSensor
} SensorTypes;



typedef struct{
I2CDriver* I2CPort;
UniversalSensor* SensorHandle;
}I2CDevices;


typedef struct{
    uint8_t DeviceType;
    uint8_t PortASensorType;
    uint8_t PortBSensorType;
    uint8_t PortASampleTime;
    uint8_t PortBSampleTime;
}DeviceStatus;

class DeviceManager {
 public:
 void Init(UniversalSensor* obj_pool[3]){
  Sensors = obj_pool;
 }
  DeviceStatus GetDeviceStatus() {
    DeviceStatus Status;
    Status.DeviceType = SENSORHUB;
    if(I2CDevice_.SensorHandle == NULL){
      Status.PortASensorType = 0;
      Status.PortBSensorType = 0;
    } else {
     Status.PortASensorType = I2CDevice_.SensorHandle->GetSensorType();
     Status.PortBSensorType = I2CDevice_.SensorHandle->GetSensorType();
    }
    return Status;
  }
  void SetupI2C(I2CDriver* i2c_handle, QueueHandle_t* SensorQueue){
    I2CDevice_.I2CPort = i2c_handle;
    SensorQueue_ = SensorQueue;
  }
  void AssignSensorToI2CPort(SensorTypes Device) {
    if( I2CDevice_.SensorHandle != NULL && I2CDevice_.SensorHandle != Sensors[Device]){
        PortGrabber.ChangeSensor(Sensors[Device]);
        I2CDevice_.SensorHandle = Sensors[Device];
        I2CDevice_.SensorHandle->Initialize(I2CDevice_.I2CPort);
        PortGrabber.ResumePollingTask();
    } else if (I2CDevice_.SensorHandle == NULL) {
        I2CDevice_.SensorHandle = Sensors[Device];
        I2CDevice_.SensorHandle->Initialize(I2CDevice_.I2CPort);
        PortGrabber.SetupPollTask(I2CDevice_.SensorHandle, 10, SensorQueue_, &PortGrabTask);
    }
  }
  void SetSampleTime(const uint8_t sample_time){
    PortGrabber.SetSampleTime(sample_time);
  }

  uint8_t GetSampleTime(){
      return PortGrabber.GetSampleTime();
  }
 private:
  I2CDevices I2CDevice_;
  MeasurementGrabber PortGrabber;
  xTaskHandle PortGrabTask;
  QueueHandle_t *SensorQueue_;
  UniversalSensor **Sensors;
};



#endif