#ifdef __arm__
#include <Arduino.h>
#include <device_settings.hpp>
#include <hal_exception.hpp>
#include <service_protocol.hpp>

void setup() {
  Serial.begin(9600);
  while (!Serial)
    continue;

  /**
   * @note Initialize the I2C drivers first!
   *       Then the pins!
  */
  i2c_handle_port_a.Init();
  i2c_handle_port_b.Init();
  InitI2CPins();

  /**
   * @note Create command queue for transmitting sensordata from grabber task to the service protocol cb methods
  */
  serviceProtocolQueue = xQueueCreateStatic( kServiceProtocolQueueLength, kServiceProtocolQueueItemSize,
                                            serviceProtocolQueueStorageArea, &staticServiceProtocolQueueStruct);
  
  /**
   * @note Initiate the sensorports, assign i2c ports with message queue and assign initial sensor!
  */
  portAProperties.Init(sensorsObjPool1);
  portBProperties.Init(sensorsObjPool2);
  portAProperties.SetupI2C(&i2c_handle_port_a, &serviceProtocolQueue);
  portBProperties.SetupI2C(&i2c_handle_port_b, &serviceProtocolQueue);
  portAProperties.AssignSensorToI2CPort(TypeCompressionSensor);
  portBProperties.AssignSensorToI2CPort(TypeDifferentialPressureSensor);

  /**
   * @note Initiate Status module and flash memory for storing the ID.
  */
  systemStatus.SetDeviceType(kDeviceTypeSensorHub);
  systemStatus.AddDeviceManager(&portAProperties);
  systemStatus.AddDeviceManager(&portBProperties);
  InitSerialExceptionLogger();
  InitExternalFlashMemory();
  InitStatusLogger("ID.txt");
  systemStatus.AttachFlashLogger(statusLoggerInst);

  /**
   * @note Initiate the Exception module
  */
  hal::exception::attachLogger(exceptionLoggerInst);
  
  /**
   * @note Initiate the USB service protocol
  */
  usb_service_protocol::Init(USBRegisters, kNumOfRegisters);
  usb_service_protocol::SetPollingTask(&USBServiceProtocolReadTask);
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
