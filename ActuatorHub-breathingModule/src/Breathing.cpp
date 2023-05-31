/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Thomas van den Oever
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish,
 *distribute, sublicense, and/or sell copies of the Software, and to permit
 *persons to whom the Software is furnished to do so,
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
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 *OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 *OR OTHER DEALINGS IN THE SOFTWARE.
 ***********************************************************************************************/
#include "Breathing.hpp"

Breathing::Breathing() {
  /*
   * Use PA20, funtion F
   * GCLK4, TCC0
   */
  motor_ = new actuator::Motor(BREATHING_PORT, BREATHING_PIN, BREATHING_FUNC,
                               BREATHING_GCLKx, BREATHING_TCCx, BREATHING_WOx);
}

Breathing::~Breathing() {
  stopBreathing();
  delete motor_;
}

void Breathing::init() {}

void Breathing::startBreathing() { motor_->startRotate(); }

void Breathing::stopBreathing() { motor_->stopRotate(); }

void Breathing::setDutyCyle(int dutyCycle) { motor_->setDuctyCycle(dutyCycle); }