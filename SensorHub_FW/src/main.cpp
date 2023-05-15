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
I2CDriver  i2c_handle_port_a = I2CDriver(&wireSensorA, kI2cSpeed_400KHz);
I2CDriver  i2c_handle_port_b = I2CDriver(&wireSensorB, kI2cSpeed_100KHz);

void setup() {
  // put your setup code here, to run once:

  //i2c_handle_port_b.Init();
  Serial.begin(9600);
  while (!Serial)
    continue;

  i2c_handle_port_a.Init();
  i2c_handle_port_b.Init();
  InitI2CPins();
  //delay(100);

  ServiceProtocolQueue = xQueueCreateStatic( QUEUE_LENGTH, ITEM_SIZE, ucQueueStorageArea, &StaticServiceProtocolQueueStruct );
  PortAMgr.Init(Sensors_objPool1);
  PortBMgr.Init(Sensors_objPool2);
  PortAMgr.SetupI2C(&i2c_handle_port_a, &ServiceProtocolQueue);
  PortBMgr.SetupI2C(&i2c_handle_port_b, &ServiceProtocolQueue);
  PortAMgr.AssignSensorToI2CPort(TypeCompressionSensor);
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
  // SensorData ResultSensA = FingerposSensor1.GetSensorData();
  // printResults("A_",ResultSensA);
  // SensorData ResultSensB = DiffSensor2.GetSensorData();
  // printResults("B_",ResultSensB);
  //Serial.println("Hello world!");
}

#else
int main() {
  while (1)
    continue;
}
#endif
