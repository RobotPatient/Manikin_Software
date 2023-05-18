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
#ifdef __arm__
#include <device_settings.hpp>
#include <hal_exception.hpp>
#include <service_protocol.hpp>

void setup() {
  Serial.begin(115200);
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
  serviceProtocolQueue = xQueueCreateStatic(kServiceProtocolQueueLength, kServiceProtocolQueueItemSize,
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

void loop() {}

#else
int main() {
  while (1)
    continue;
}
#endif
