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

#include "BreathingControl.hpp"

#include <RingBuffer.h>

BreathingControl::BreathingControl() {
  breathingData_ = new BreathingData();
  breathing_ = new Breathing();
  compression_ = new Compression();
}

BreathingControl::~BreathingControl() {
  delete breathingData_;
  delete breathing_;
  delete compression_;
}

void BreathingControl::start() {
  breathing_->startBreathing();
  compression_->StartCompression();
}

void BreathingControl::loop() {
  if (!isBreathing_) {
    return;
  }
#ifdef DEBUG_SERIAL
  Serial.print("Index: ");
  Serial.print(breathingData_->nextDataPoint().index);
  Serial.print(" Bpoint = ");
  Serial.print(breathingData_->getCurrent().breathingPoint);
  Serial.print(" Cpoint = ");
  Serial.println(breathingData_->getCurrent().compressionPoint);
#endif

  // TODO(Thomas): check when to stop and start breathing process
  compression_->setDutyCycle(breathingData_->getCurrent().compressionPoint);
  breathing_->setDutyCyle(breathingData_->getCurrent().breathingPoint);
}