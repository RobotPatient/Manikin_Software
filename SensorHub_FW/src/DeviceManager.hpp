#ifndef DEVICEMANAGER_HPP
#define DEVICEMANAGER_HPP
#include <stdint.h>
#include <device_settings.hpp>
#include <measurement_grabber.hpp>

typedef enum {
  TypeNone,
  TypeCompressionSensor,
  TypeDifferentialPressureSensor,
  TypefingerPositionSensor
} SensorTypes;

typedef struct {
  I2CDriver* I2CPort;
  UniversalSensor* SensorHandle;
} I2CDevices;

class DeviceManager {
 public:
  void Init(UniversalSensor* obj_pool[3]) { Sensors = obj_pool; }
  void SetupI2C(I2CDriver* i2c_handle, QueueHandle_t* SensorQueue) {
    I2CDevice_.I2CPort = i2c_handle;
    SensorQueue_ = SensorQueue;
  }
  void AssignSensorToI2CPort(SensorTypes Device) {
    if (I2CDevice_.SensorHandle != NULL &&
        I2CDevice_.SensorHandle != Sensors[Device]) {
      PortGrabber.ChangeSensor(Sensors[Device]);
      I2CDevice_.SensorHandle = Sensors[Device];
      I2CDevice_.SensorHandle->Initialize(I2CDevice_.I2CPort);
      PortGrabber.ResumePollingTask();
    } else if (I2CDevice_.SensorHandle == NULL) {
      I2CDevice_.SensorHandle = Sensors[Device];
      I2CDevice_.SensorHandle->Initialize(I2CDevice_.I2CPort);
      PortGrabber.SetupPollTask(I2CDevice_.SensorHandle, 10, SensorQueue_,
                                &PortGrabTask);
    }
    currentSensorType = Device;
  }
  void SetSampleTime(const uint8_t sample_time) {
    PortGrabber.SetSampleTime(sample_time);
  }
  void ResumeSensor() {
    PortGrabber.ResumePollingTask();
  }
  void PauseSensor() {
    PortGrabber.SuspendPollingTask();
  }
  uint8_t GetSampleTime() { return PortGrabber.GetSampleTime(); }

  uint8_t GetSensorType() { return currentSensorType; }

 private:
  uint8_t currentSensorType;
  I2CDevices I2CDevice_;
  MeasurementGrabber PortGrabber;
  xTaskHandle PortGrabTask;
  QueueHandle_t* SensorQueue_;
  UniversalSensor** Sensors;
};

#endif