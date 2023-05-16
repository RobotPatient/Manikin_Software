#ifdef __arm__
#include <Arduino.h>
#include <Wire.h>
#include <device_settings.hpp>
#include <ServiceProtocol.hpp>
#include <FreeRTOS.h>
#include <hal_log.hpp>
#include <hal_exception.hpp>

using namespace hal::exception;
using namespace hal::log;

static xTaskHandle PollTask;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    continue;

  i2c_handle_port_a.Init();
  i2c_handle_port_b.Init();
  InitI2CPins();
  ServiceProtocolQueue = xQueueCreateStatic( QUEUE_LENGTH, ITEM_SIZE, ucQueueStorageArea, &StaticServiceProtocolQueueStruct );
  PortAMgr.Init(Sensors_objPool1);
  PortBMgr.Init(Sensors_objPool2);
  PortAMgr.SetupI2C(&i2c_handle_port_a, &ServiceProtocolQueue);
  PortBMgr.SetupI2C(&i2c_handle_port_b, &ServiceProtocolQueue);
  PortAMgr.AssignSensorToI2CPort(TypeCompressionSensor);
  PortBMgr.AssignSensorToI2CPort(TypeDifferentialPressureSensor);
  DevStatus.SetDeviceType(SENSORHUB);
  DevStatus.AddDeviceManager(&PortAMgr);
  DevStatus.AddDeviceManager(&PortBMgr);
  InitSerialExceptionLogger();
  InitFlashMem();
  InitStatusLogger("ID.txt");
  DevStatus.AddFlashLogger(StatusLoggerInst);
  hal::exception::attachLogger(ExceptionLoggerInst);
  usb_service_protocol::Init(USBRegisters, kNumOfRegisters);
  usb_service_protocol::SetPollingTask(&PollTask);
  vTaskStartScheduler();
}

void loop() {

}

#else
int main() {
  while (1)
    continue;
}
#endif
