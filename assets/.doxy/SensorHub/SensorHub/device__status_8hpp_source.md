

# File device\_status.hpp

[**File List**](files.md) **>** [**SensorHub\_FW**](dir_8a3376b0b822df0b6266211cee305325.md) **>** [**src**](dir_d67209f9e2b4e51eca02397244f6bfa8.md) **>** [**device\_status.hpp**](device__status_8hpp.md)

[Go to the documentation of this file](device__status_8hpp.md)

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
#ifndef STATUS_HPP
#define STATUS_HPP
#include <device_properties.hpp>
#include <device_status.hpp>
#include <hal_log.hpp>

typedef enum { kDeviceTypeUndefined, kDeviceTypeSensorHub, kDeviceTypeActuatorHub } DeviceTypes;

namespace module::status {

constexpr const char kStatusFormatString[] =
    "{\"Status\": \"%s\", \"DeviceType\": %d, \"DeviceID:\": %d, "
    "\"PortASenType\": %d, "
    "\"PortBSenType\": %d, \"PortASampleRate\": %d, \"PortBSampleRate\": %d}";

inline constexpr const char kUninitializedStatus[] = "Unitialized";
inline constexpr const char kInitializedStatus[] = "Initialized";

inline constexpr uint8_t kDeviceInitialized = 1;
inline constexpr uint8_t kDeviceUnitialized = 0;

inline constexpr uint8_t kIDLocationInFlash = 0;
// Some specifications of the device
inline constexpr uint8_t kNumOfSensorPorts = 2;

// Port indices used by the functions below!
inline constexpr uint8_t kSensorPortAIndex = 0;
inline constexpr uint8_t kSensorPortBIndex = 1;
inline constexpr uint8_t kBBPortIndex = 2;

class DeviceStatus {
 public:
  void SetDeviceType(DeviceTypes device_type) { device_type_ = device_type; }

  void SetDeviceID(const uint8_t ID) {
    if (log_ != NULL) {
      log_->setcursorpos(kIDLocationInFlash);
      log_->writebyte(ID);
    }
  }

  uint8_t GetDeviceID() {
    if (log_ != NULL) {
      char ID;
      log_->setcursorpos(kIDLocationInFlash);
      log_->readbyte(&ID);
      return ID;
    } else {
      return 0;
    }
  }

  void GetDeviceStatus(char* write_buffer, const uint32_t write_buffer_size) {
    uint8_t DeviceStatus = kDeviceInitialized;
    if (num_of_devices < kNumOfSensorPorts) {
      DeviceStatus = kDeviceUnitialized;
    }
    uint8_t device_id = GetDeviceID();
    uint8_t device_type = device_type_;
    uint8_t sample_rate[kNumOfSensorPorts];
    uint8_t sensortype[kNumOfSensorPorts];
    for (uint8_t port_index = 0; port_index < num_of_devices; port_index++) {
      const bool device_initialized = (devices_[port_index] != NULL);
      if (device_initialized) {
        sample_rate[port_index] = devices_[port_index]->GetSampleTime();
        sensortype[port_index] = devices_[port_index]->GetSensorType();
      } else {
        sample_rate[port_index] = 0;
        sensortype[port_index] = 0;
      }
    }
    const char* status_string = (DeviceStatus == kDeviceInitialized) ? kInitializedStatus : kUninitializedStatus;
    snprintf(write_buffer, write_buffer_size, kStatusFormatString, status_string, device_type, device_id,
             sensortype[kSensorPortAIndex], sensortype[kSensorPortBIndex], sample_rate[kSensorPortAIndex],
             sample_rate[kSensorPortBIndex]);
  }

  void AddDeviceManager(DeviceProperties* device_properties) {
    if (num_of_devices < kNumOfSensorPorts && device_properties != NULL) {
      devices_[num_of_devices] = device_properties;
      num_of_devices++;
    }
  }
  
  void AttachFlashLogger(hal::log::Logger* log_module) { log_ = log_module; }

 private:
  uint8_t num_of_devices = 0;
  DeviceTypes device_type_;
  hal::log::Logger* log_ = NULL;
  DeviceProperties* devices_[kNumOfSensorPorts];
};
}  // namespace module::status
#endif

```

