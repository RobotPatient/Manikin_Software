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
 /**
  * @brief Init and add the SensorObjPool to the DeviceProperties.
  * @param obj_pool Array with size defined in device_sensor_lib_settings.hpp 
  *                 containing static sensor obj allocations
 */
  void Init(UniversalSensor* obj_pool[kNumOfSupportedSensors]) { SensorObjPool_ = obj_pool; }

  /**
   * @brief Link the i2c_handle and SensorQueue to internall shadow copies..
   *        This way the sensors can be allocated using these handles!
   * @param i2c_handle The handle to a I2CDriver instance
   * @param SensorQueue The handle to the measurement queue to post sensorvalue's in
  */
  void SetupI2C(I2CDriver* i2c_handle, QueueHandle_t* SensorQueue) {
    DeviceI2CPort_ = i2c_handle;
    SensorQueue_ = SensorQueue;
  }

  /**
   * @brief Change the sensor.
   * @param Device The Sensortype to change to
  */
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
  
  /**
   * @brief Set the sample rate of the polling task in ms.
   * @param sample_time the sample rate to set in milliseconds.
  */
  void SetSampleTime(const uint8_t sample_time) { PortGrabber_.SetSampleTime(sample_time); }

  /**
   * @brief Resume sensor measurements after polling task is suspended
  */
  void ResumeSensor() { PortGrabber_.ResumePollingTask(); }

  /**
   * @brief Suspend sensor measurements polling task
  */
  void PauseSensor() { PortGrabber_.SuspendPollingTask(); }

  /**
   * @brief Get the current sample rate
   * @return sample rate in milliseconds
  */
  uint8_t GetSampleTime() { return PortGrabber_.GetSampleTime(); }

  /**
   * @brief Get the current selected sensortype
   * @return The sensortype
  */
  uint8_t GetSensorType() { return CurrentSensorType_; }

 private:
  /**
   * @note The handles used to create and control the polling task
  */
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