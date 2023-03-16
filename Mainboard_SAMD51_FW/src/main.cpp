#include <Arduino.h>
#include <Wire.h>
#include <mainboard_settings.hpp>
#include <Adafruit_SPIFlash.h>
#include <SdFat.h>

Adafruit_FlashTransport_QSPI flashTransport(kSpiFramClkPin, kSpiFramCsPin, kSpiFramData0Pin,
                                            kSpiFramData1Pin, kSpiFramData2Pin, kSpiFramData3Pin);


Adafruit_SPIFlash flash(&flashTransport);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(100); // wait for native usb
  }
  // put your setup code here, to run once:
  spi_bus.begin();
  flash.begin();
  Serial.print("JEDEC ID: 0x");
  Serial.println(flash.getJEDECID(), HEX);
  Serial.print("Flash size: ");
  Serial.print(flash.size() / 1024);
  Serial.println(" KB");

}

void loop() {
  // put your main code here, to run repeatedly:
}