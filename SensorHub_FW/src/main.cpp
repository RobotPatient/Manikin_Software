#ifdef __arm__
#include <Arduino.h>
#include <FreeRTOS.h>
#include <device_settings.hpp>
#include <hal_exception.hpp>
#include <service_protocol.hpp>

using namespace hal::exception;
using namespace hal::log;
using namespace module::status;
static xTaskHandle PollTask;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    continue;

  i2c_handle_port_a.Init();
  i2c_handle_port_b.Init();
  InitI2CPins();
  serviceProtocolQueue = xQueueCreateStatic( kServiceProtocolQueueLength, kServiceProtocolQueueItemSize,
                                            serviceProtocolQueueStorageArea, &staticServiceProtocolQueueStruct);
  portAProperties.Init(sensorsObjPool1);
  portBProperties.Init(sensorsObjPool2);
  portAProperties.SetupI2C(&i2c_handle_port_a, &serviceProtocolQueue);
  portBProperties.SetupI2C(&i2c_handle_port_b, &serviceProtocolQueue);
  portAProperties.AssignSensorToI2CPort(TypeCompressionSensor);
  portBProperties.AssignSensorToI2CPort(TypeDifferentialPressureSensor);
  systemStatus.SetDeviceType(kDeviceTypeSensorHub);
  systemStatus.AddDeviceManager(&portAProperties);
  systemStatus.AddDeviceManager(&portBProperties);
  InitSerialExceptionLogger();
  InitExternalFlashMemory();
  InitStatusLogger("ID.txt");
  systemStatus.AttachFlashLogger(statusLoggerInst);
  hal::exception::attachLogger(exceptionLoggerInst);
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
