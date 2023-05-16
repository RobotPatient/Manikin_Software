#ifndef STATUS_HPP
#define STATUS_HPP
#include <FlashAsEEPROM.h>
#include <DeviceManager.hpp>

const char kStatusFormatString[] = "{\"Status\": \"%s\", \"DeviceType\": %d, \"DeviceID:\": %d, \"PortASenType\": %d, "
                                    "\"PORTBSENTYPE\": %d, \"PORTASAMPLETIME\": %d, \"PORTBSAMPLETIME\": %d}";
inline constexpr uint32_t kDeviceIDEEPROMIndex = 0;
inline constexpr char kUninitializedStatus[] = "Unitialized";
inline constexpr char kInitializedStatus[] = "Initialized";

inline constexpr uint8_t DeviceInitialized = 1;
inline constexpr uint8_t DeviceUnitialized = 0;

// Some specifications of the device
inline constexpr uint8_t kDeviceType = SENSORHUB; 
inline constexpr uint8_t kNumOfI2CPorts = 3;
inline constexpr uint8_t kNumOfSensorPorts = 2;

class Status {
 public:
  void SetDeviceID(uint8_t ID) { EEPROM.write(kDeviceIDEEPROMIndex, ID); }

  uint8_t GetDeviceID() { return EEPROM.read(kDeviceIDEEPROMIndex); }

  void GetDeviceStatus(char* write_buffer, const uint32_t write_buffer_size) {
     uint8_t DeviceStatus = DeviceInitialized;
      if(num_of_devices < kNumOfSensorPorts) {
        DeviceStatus = DeviceUnitialized;
      }
      uint8_t DeviceId = 0;
      uint8_t DeviceType = kDeviceType;
      uint8_t sampletime[kNumOfSensorPorts];
      uint8_t sensortype[kNumOfSensorPorts];
      for(uint8_t i =0; i < num_of_devices; i++) {
        const bool DeviceInitialized = (Devices[i] != NULL);
        if(DeviceInitialized){
            sampletime[i] = Devices[i]->GetSampleTime();
            sensortype[i] = Devices[i]->GetSensorType();
        } else {
            sampletime[i] = 0;
            sensortype[i] = 0;
        }
      }
      const char *status_string = DeviceStatus ? kInitializedStatus : kUninitializedStatus;
      snprintf(write_buffer, write_buffer_size, kStatusFormatString, status_string,
                         DeviceType, DeviceId, sensortype[0], sensortype[1],
                         sampletime[0], sampletime[1]);
  }

  void AddDeviceManager(DeviceManager* devMgr) {
    if(num_of_devices < kNumOfSensorPorts && devMgr != NULL) {
        Devices[num_of_devices] = devMgr;
        num_of_devices++;
    }
  }

 private:
  uint8_t num_of_devices = 0;
  DeviceManager *Devices[kNumOfSensorPorts];
};
#endif