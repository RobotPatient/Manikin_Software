#ifdef __arm__
#include <Arduino.h>

#include <sensorhub_settings.hpp>
#include <sensor_helper.hpp>
/* Create handle for sensor connector PORT_A.
 * This will be used in setup() and loop()*/
UniversalSensor *connector_port_a;
I2CDriver i2c_handle_port_a = I2CDriver(&wireSensorA, ki2cSpeed_400KHz);

void printResults(SensorData data){
  for(int i =0; i< data.numOfBytes/2; i++) {
    Serial.print("R:");
    Serial.print(data.buffer[i]);
  }
  Serial.print("\n");

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial)
    continue;
  wireBackbone.begin();
  wireSensorA.begin();
  wireSensorB.begin();
  InitI2CPins();
  connector_port_a = new CompressionSensor(&i2c_handle_port_a);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  SensorData result = connector_port_a->GetSensorData();
  printResults(result);
  delay(100);
}

#else
int main() {
  while (1)
    continue;
}
#endif
