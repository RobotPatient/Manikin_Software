#include <Arduino.h>

#include "BreathingControl.hpp"

BreathingControl *control;

void setup() { control = new BreathingControl(); }

void loop() { control->loop(); }
