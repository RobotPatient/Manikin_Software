#include <Arduino.h>
// This needs to be included after the arduino include because of build reasons
// see issue
// [#23](https://github.com/RobotPatient/Manikin_Software_Libraries/issues/23)
#include <SdFat.h>

#include "BreathingControl.hpp"

BreathingControl *control;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  control = new BreathingControl();
}

void loop() {
  Serial.println("Loop");
  control->loop();
  delay(1000);
}
