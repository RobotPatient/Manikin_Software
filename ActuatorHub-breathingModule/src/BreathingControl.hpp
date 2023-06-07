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
#ifndef BREATHINGCONTROL_HPP
#define BREATHINGCONTROL_HPP

#define DEBUG_SERIAL

#include <Arduino.h>

#include <gpio.hpp>

#include "Breathing.hpp"
#include "BreathingData.hpp"
#include "Compression.hpp"

/// @brief
class BreathingControl {
 public:
  BreathingControl();

  ~BreathingControl();

  void start();
  void stop();

  void loop();

  void setStartStopBit(bool);

 private:
  bool startStop();
  void printData();

  BreathingData *breathingData_;
  Compression *compression_;
  Breathing *breathing_;
  volatile bool isBreathing_ = true;
  volatile bool start_;
  volatile bool stop_;
};

#endif
