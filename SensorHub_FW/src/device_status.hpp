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

/**
 * @brief The string used by snprintf to fill the buffer with a fixed format.
 *        This will provide the response of the Status command!
 */
constexpr const char kStatusFormatString[] =
    "{\"Status\": \"%s\", \"DeviceType\": %d, \"DeviceID:\": %d, "
    "\"PortASenType\": %d, "
    "\"PortBSenType\": %d, \"PortASampleRate\": %d, \"PortBSampleRate\": %d}";

/**
 * @brief The strings which will be inserted in to the format string depending on
 *        whether the system is initialized or not.
 */
inline constexpr const char kUninitializedStatus[] = "Unitialized";
inline constexpr const char kInitializedStatus[] = "Initialized";

/**
 * @brief Integers are easier than strings to compare. Therefore these integers
 *        are macro's to select the above listed strings.
 */
inline constexpr uint8_t kDeviceInitialized = 1;
inline constexpr uint8_t kDeviceUnitialized = 0;

/**
 * @brief The flash cursor position for the ID data
 */
inline constexpr uint8_t kIDLocationInFlash = 0;
// Some specifications of the device
inline constexpr uint8_t kNumOfSensorPorts = 2;

// Port indices used by the functions below!
inline constexpr uint8_t kSensorPortAIndex = 0;
inline constexpr uint8_t kSensorPortBIndex = 1;
inline constexpr uint8_t kBBPortIndex = 2;

class DeviceStatus {
 public:
  /**
   * @brief Set the device type (SensorHub, ActuatorHub, etc.)
   * @param device_type The device type to set (see the above listed DeviceTypes enum)
  */
  void SetDeviceType(DeviceTypes device_type) { device_type_ = device_type; }

  /**
   * @brief Set the unique device id (will write it to flash memory)
   * @param ID The ID to write to flash/set
  */
  void SetDeviceID(const uint8_t ID) {
    if (log_ != NULL) {
      log_->setcursorpos(kIDLocationInFlash);
      log_->writebyte(ID);
    }
  }

  /**
   * @brief Get the unique device id (read from flash)
   * @return The ID read from flash
  */
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

  /**
   * @brief Get the device status, collected from other functions of this class and 
   *        the DeviceProperties class.
   * 
   * @param write_buffer The write_buffer to write the parsed status string in to!
   * @param write_buffer_size The size of the write_buffer array passed in to the arguments
  */
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

  /**
   * @brief Add the device properties classes, as this provides easier access to the
   *        properties like sample rate, sensortype's etc. It will add this to an internal 
   *        array.
   * @param device_properties A pointer to a device_properties instance.
  */
  void AddDeviceManager(DeviceProperties* device_properties) {
    if (num_of_devices < kNumOfSensorPorts && device_properties != NULL) {
      devices_[num_of_devices] = device_properties;
      num_of_devices++;
    }
  }
  
  /**
   * @brief Attach a logger object to this class. So the DeviceID functions can read and write
   *        the id to/from flash.
   * @param log_module A pointer to a logger instance
  */
  void AttachFlashLogger(hal::log::Logger* log_module) { log_ = log_module; }

 private:
  uint8_t num_of_devices = 0;
  DeviceTypes device_type_;
  hal::log::Logger* log_ = NULL;
  DeviceProperties* devices_[kNumOfSensorPorts];
};
}  // namespace module::status
#endif