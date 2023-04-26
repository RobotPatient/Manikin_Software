#include <Arduino.h>

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
