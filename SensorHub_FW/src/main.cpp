#ifdef __arm__
#include <Arduino.h>
#include <Wire.h>
#include <FreeRTOS.h>
#endif

#include <SensorHubSettings.h>
#include <sensor_helper.hpp>

void setup() {
  // put your setup code here, to run once:
  #ifdef __arm__
  Serial.begin(9600);

  while (!Serial);
  
  WireBackbone.begin();
  WireSensorA.begin();
  WireSensorB.begin();
  InitI2CPins();
  #endif
 // Serial.print(p);
  //sensor Connector1(&WireSensorA);
  //Connector1.setSensorType(Sensor_compression);

}

void loop() {
  // put your main code here, to run repeatedly:
}

#ifndef __arm__
int main(){
  setup();
  while(1){
    loop();
  }
}
#endif