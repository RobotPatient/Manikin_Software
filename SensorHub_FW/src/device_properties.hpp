#ifndef DEVICEMANAGER_HPP
#define DEVICEMANAGER_HPP
#include <device_sensor_lib_settings.hpp>
#include <measurement_grabber.hpp>

class DeviceProperties {
 public:
  void Init(UniversalSensor* obj_pool[kNumOfSupportedSensors]) { SensorObjPool_ = obj_pool; }
  void SetupI2C(I2CDriver* i2c_handle, QueueHandle_t* SensorQueue) {
    DeviceI2CPort_ = i2c_handle;
    SensorQueue_ = SensorQueue;
  }
  void AssignSensorToI2CPort(SensorTypes Device) {
    if (SensorHandle_ != NULL && SensorHandle_ != SensorObjPool_[Device]) {
      PortGrabber_.ChangeSensor(SensorObjPool_[Device]);
      SensorHandle_ = SensorObjPool_[Device];
      SensorHandle_->Initialize(DeviceI2CPort_);
      PortGrabber_.ResumePollingTask();
    } else if (SensorHandle_ == NULL) {
      SensorHandle_ = SensorObjPool_[Device];
      SensorHandle_->Initialize(DeviceI2CPort_);
      PortGrabber_.SetupPollTask(SensorHandle_, 10, SensorQueue_,
                                 &PortGrabTask_);
    }
    CurrentSensorType_ = Device;
  }
  void SetSampleTime(const uint8_t sample_time) {
    PortGrabber_.SetSampleTime(sample_time);
  }
  void ResumeSensor() { PortGrabber_.ResumePollingTask(); }
  void PauseSensor() { PortGrabber_.SuspendPollingTask(); }
  uint8_t GetSampleTime() { return PortGrabber_.GetSampleTime(); }

  uint8_t GetSensorType() { return CurrentSensorType_; }

 private:
  I2CDriver* DeviceI2CPort_;
  UniversalSensor* SensorHandle_;
  uint8_t CurrentSensorType_;
  MeasurementGrabber PortGrabber_;
  xTaskHandle PortGrabTask_;
  QueueHandle_t* SensorQueue_;
  UniversalSensor** SensorObjPool_;
};

#endif