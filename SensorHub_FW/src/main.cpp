#ifdef __arm__
#include <Arduino.h>

#include <sensorhub_settings.hpp>
#include <sensor_helper.hpp>
/* Create handle for sensor connector PORT_A.
 * This will be used in setup() and loop()*/
Sensor connector_port_a(&wireSensorA);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial)
    continue;
  wireBackbone.begin();
  wireSensorA.begin();
  wireSensorB.begin();
  InitI2CPins();
  connector_port_a.SetSensorType(SensorTypes::kCompressionSensor);
}

void loop() {
  // put your main code here, to run repeatedly:

}

#else
int main() {
  while (1)
    continue;
}
#endif
