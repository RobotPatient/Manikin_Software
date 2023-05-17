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
  void SetDeviceType(DeviceTypes device_type) { device_type_ = device_type; }
  void SetDeviceID(const uint8_t ID) {
    log_->setcursorpos(kIDLocationInFlash);
    log_->writebyte(ID);
  }

  uint8_t GetDeviceID() {
    char ID;
    log_->setcursorpos(kIDLocationInFlash);
    log_->readbyte(&ID);
    return ID;
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
    const char* status_string = DeviceStatus ? kInitializedStatus : kUninitializedStatus;
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
  hal::log::Logger* log_;
  DeviceProperties* devices_[kNumOfSensorPorts];
};
}  // namespace module::status
#endif