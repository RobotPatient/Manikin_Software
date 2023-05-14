#ifdef __arm__
#include <Arduino.h>
#include <device_settings.hpp>
#include <ServiceProtocol.hpp>
#include <FreeRTOS.h>
#include <ringbuffer.hpp>
#include <Status.hpp>
#ifdef ENABLE_LOGGER
#include <hal_exception.hpp>

using namespace hal::exception;
using namespace hal::log;

static Logger* log_inst;
static LoggerSettings Log_module_settings;
#endif

static xTaskHandle PollTask;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial)
    continue;
  wireBackbone.begin();
  InitI2CPins();
  i2c_handle_port_a.Init();
  i2c_handle_port_b.Init();
  ServiceProtocolQueue = xQueueCreateStatic( QUEUE_LENGTH, ITEM_SIZE, ucQueueStorageArea, &StaticServiceProtocolQueueStruct );
  PortAMgr.Init(Sensors_objPool1);
  PortBMgr.Init(Sensors_objPool2);
  PortBMgr.SetupI2C(&i2c_handle_port_b, &ServiceProtocolQueue);
  PortAMgr.SetupI2C(&i2c_handle_port_a, &ServiceProtocolQueue);
  PortAMgr.AssignSensorToI2CPort(TypefingerPositionSensor);
  PortBMgr.AssignSensorToI2CPort(TypeDifferentialPressureSensor);
  #ifdef ENABLE_LOGGER
  InitSerialLogger(log_inst, &Log_module_settings, &Serial);
  //InitFlashLogger(log_inst, &Log_module_settings, "test.txt");
  hal::exception::attachLogger(log_inst);
  #endif
  usb_service_protocol::Init(USBRegisters, kNumOfRegisters);
  usb_service_protocol::SetPollingTask(&PollTask);
  vTaskStartScheduler();
}

void loop() {
  // SensorData ResultSensA = connector_port_a->GetSensorData();
  // printResults("A_",ResultSensA);
  // SensorData ResultSensB = connector_port_b->GetSensorData();
  // printResults("B_",ResultSensB);
}

#else
int main() {
  while (1)
    continue;
}
#endif
