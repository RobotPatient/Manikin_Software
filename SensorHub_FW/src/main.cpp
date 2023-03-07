#ifdef __arm__
#include <Arduino.h>

#include <sensorhub_settings.hpp>
#include <sensor_helper.hpp>
/* Create handle for sensor connector PORT_A.
 * This will be used in setup() and loop()*/
UniversalSensor *connector_port_a;
UniversalSensor *connector_port_b;
I2CDriver i2c_handle_port_a = I2CDriver(&wireSensorA, ki2cSpeed_100KHz);
I2CDriver i2c_handle_port_b = I2CDriver(&wireSensorB, ki2cSpeed_100KHz);

void printResults(String Prefix, SensorData data){
  for(int i =0; i< data.numOfBytes/2; i++) {
    Serial.print(Prefix+"R:");
    Serial.print(data.buffer[i]);
  }
  if(data.numOfBytes == 1){
    Serial.print(data.buffer[0]);
  }
  Serial.print("\n");

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial)
    continue;
  wireBackbone.begin();
  InitI2CPins();
  i2c_handle_port_a.init();
  i2c_handle_port_b.init();
  connector_port_a = new FingerPositionSensor(&i2c_handle_port_a);
  connector_port_a->Initialize();
  connector_port_b = new DifferentialPressureSensor(&i2c_handle_port_a);
  connector_port_b->Initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  SensorData ResultSensA = connector_port_a->GetSensorData();
  printResults("A_",ResultSensA);
  SensorData ResultSensB = connector_port_b->GetSensorData();
  printResults("B_",ResultSensB);
  delay(100);
}

#else
int main() {
  while (1)
    continue;
}
#endif
