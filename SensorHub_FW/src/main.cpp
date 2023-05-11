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
  //DevMgr.SetupI2C(SensorPortA, &i2c_handle_port_a);
  DevMgr.SetupI2C(SensorPortB, &i2c_handle_port_b);
  //DevMgr.AssignSensorToI2CPort(SensorPortA, TypeCompressionSensor);
  DevMgr.AssignSensorToI2CPort(SensorPortB, TypefingerPositionSensor);
  #ifdef ENABLE_LOGGER
  hal::exception::Init();
  InitSerialLogger(log_inst, &Log_module_settings, &Serial);
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
