#include <Arduino.h>
#include <Wire.h>
#include <FreeRTOS.h>

#include <SensorHubSettings.h>
#include <SDP810.h>
#include <RobotPatient_VL6180x.h>
#include <ADS7138.hpp>
#include <sensor_helper.hpp>

#define VL6180X_ADDRESS 0x29



i2c_driver VL6180_Handle (&WireSensorB, i2c_speed_400KHz, VL6180X_ADDRESS);
i2c_driver SDP810_Handle (&WireSensorA, i2c_speed_400KHz, SDP_addr);

SDP810 sdp_sensor(&SDP810_Handle);
RobotPatient_VL6180x VL8160_sensor(&VL6180_Handle);



void reportDevicesOn(TwoWire *wire, byte deviceAddress)
{
  Serial.print("Address: 0x");
  Serial.println(deviceAddress, HEX);
  wire->beginTransmission(deviceAddress);
  int error = wire->endTransmission();
  Serial.print(" error: ");
  Serial.println(error);

  if (error == 0)
  {
    Serial.println("Sensor found..");
  }
  else
  {
    Serial.println("No sensor found..");
  }
}

void ReadDataBus(){

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial);
  
  WireBackbone.begin();
  WireSensorA.begin();
  WireSensorB.begin();
 // Serial.print(p);
  sensor Connector1(&WireSensorA);
  Connector1.setSensorType(Sensor_compression);
  InitI2CPins();
  reportDevicesOn(&WireSensorB, VL6180X_ADDRESS);
}

void loop() {
  // put your main code here, to run repeatedly:
}