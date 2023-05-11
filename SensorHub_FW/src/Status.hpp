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



static CompressionSensor CompressionSens;
static DifferentialPressureSensor DiffSensor;
static FingerPositionSensor FingerposSensor;

/* The queue is to be created to hold a maximum of 10 uint64_t
variables. */
#define QUEUE_LENGTH    5
#define ITEM_SIZE       sizeof( SensorData )
uint8_t ucQueueStorageArea[ QUEUE_LENGTH * ITEM_SIZE ];

/* The variable used to hold the queue's data structure. */
static StaticQueue_t StaticServiceProtocolQueueStruct;
QueueHandle_t ServiceProtocolQueue;
static MeasurementGrabber PortAGrabber;
static xTaskHandle PortAGrabTask;

UniversalSensor* Sensors[3] = {&CompressionSens, &DiffSensor, &FingerposSensor};


typedef struct{
I2CDriver* I2CPort;
UniversalSensor* SensorHandle;
}I2CDevices;

typedef enum {
    SensorPortA,
    SensorPortB,
    MainBoardPort
} DevicePorts;

typedef struct{
    uint8_t DeviceType;
    uint8_t PortASensorType;
    uint8_t PortBSensorType;
    uint8_t PortASampleTime;
    uint8_t PortBSampleTime;
}DeviceStatus;

class DeviceManager {
 public:
  DeviceStatus GetDeviceStatus() {
    DeviceStatus Status;
    Status.DeviceType = SENSORHUB;
    if(Devices[SensorPortA].SensorHandle == NULL){
      Status.PortASensorType = 0;
      Status.PortBSensorType = 0;
    } else {
     Status.PortASensorType = Devices[SensorPortA].SensorHandle->GetSensorType();
     Status.PortBSensorType = Devices[SensorPortB].SensorHandle->GetSensorType();
    }
    return Status;
  }
  void SetupI2C(DevicePorts Port, I2CDriver* i2c_handle){
    Devices[Port].I2CPort = i2c_handle;
    ServiceProtocolQueue = xQueueCreateStatic( QUEUE_LENGTH, ITEM_SIZE, ucQueueStorageArea, &StaticServiceProtocolQueueStruct );
  }
  void AssignSensorToI2CPort(DevicePorts Port, SensorTypes Device) {
    if( Devices[Port].SensorHandle != NULL && Devices[Port].SensorHandle != Sensors[Device]){
        PortAGrabber.ChangeSensor(Sensors[Device]);
        Devices[Port].SensorHandle = Sensors[Device];
        Devices[Port].SensorHandle->Initialize(Devices[Port].I2CPort);
        PortAGrabber.ResumePollingTask();
    } else if (Devices[Port].SensorHandle == NULL) {
        Devices[Port].SensorHandle = Sensors[Device];
        Devices[Port].SensorHandle->Initialize(Devices[Port].I2CPort);
        PortAGrabber.SetupPollTask(Devices[Port].SensorHandle, 10, &ServiceProtocolQueue, &PortAGrabTask);
    }
  }
  void SetSampleTime(const uint8_t sample_time){
    PortAGrabber.SetSampleTime(sample_time);
  }

  uint8_t GetSampleTime(){
      return PortAGrabber.GetSampleTime();
  }
 private:
  I2CDevices Devices[3];
};



#endif