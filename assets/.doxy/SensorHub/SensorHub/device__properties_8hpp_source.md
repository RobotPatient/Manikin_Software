

# File device\_properties.hpp

[**File List**](files.md) **>** [**SensorHub\_FW**](dir_8a3376b0b822df0b6266211cee305325.md) **>** [**src**](dir_d67209f9e2b4e51eca02397244f6bfa8.md) **>** [**device\_properties.hpp**](device__properties_8hpp.md)

[Go to the documentation of this file](device__properties_8hpp.md)

```C++

/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/
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

  void AssignSensorToI2CPort(uint8_t Device) {
    if (SensorHandle_ != NULL && SensorHandle_ != SensorObjPool_[Device]) {
      PortGrabber_.ChangeSensor(SensorObjPool_[Device]);
      SensorHandle_ = SensorObjPool_[Device];
      SensorHandle_->Initialize(DeviceI2CPort_);
      PortGrabber_.ResumePollingTask();
    } else if (SensorHandle_ == NULL) {
      SensorHandle_ = SensorObjPool_[Device];
      SensorHandle_->Initialize(DeviceI2CPort_);
      PortGrabber_.SetupPollTask(SensorHandle_, 10, SensorQueue_, &PortGrabTask_);
    }
    CurrentSensorType_ = Device;
  }
  
  void SetSampleTime(const uint8_t sample_time) { PortGrabber_.SetSampleTime(sample_time); }

  void ResumeSensor() { PortGrabber_.ResumePollingTask(); }

  void PauseSensor() { PortGrabber_.SuspendPollingTask(); }

  uint8_t GetSampleTime() { return PortGrabber_.GetSampleTime(); }

  uint8_t GetSensorType() { return CurrentSensorType_; }

 private:
  I2CDriver* DeviceI2CPort_;
  UniversalSensor* SensorHandle_;
  module::measurement_grabber::MeasurementGrabber PortGrabber_;
  xTaskHandle PortGrabTask_;
  QueueHandle_t* SensorQueue_;
  // Object pool with statically assigned sensors.
  UniversalSensor** SensorObjPool_;
  // Keep track of currently set sensor type.
  uint8_t CurrentSensorType_;
};

#endif

```

