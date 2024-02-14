

# File main.cpp

[**File List**](files.md) **>** [**SensorHub\_FW**](dir_8a3376b0b822df0b6266211cee305325.md) **>** [**src**](dir_d67209f9e2b4e51eca02397244f6bfa8.md) **>** [**main.cpp**](main_8cpp.md)

[Go to the documentation of this file](main_8cpp.md)

```C++

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

  i2c_handle_port_a.Init();
  i2c_handle_port_b.Init();
  InitI2CPins();

  serviceProtocolQueue = xQueueCreateStatic(kServiceProtocolQueueLength, kServiceProtocolQueueItemSize,
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

```

