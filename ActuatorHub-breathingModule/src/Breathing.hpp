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
#ifndef BREATHING_HPP
#define BREATHING_HPP

#include "Breathing.hpp"
#include "Compression.hpp"

/**
 * D4_FLASHMOSI PA08
 * D2_FLASHMISO PA09
 * D3_FLASHSCK PA14
 *
 * PA20 ACT1
 * PA18 ACT2
 * PA19 ACT3
 * PA21 ACT4
 */

/*
 * Use PA20, funtion F
 * GCLK4, TCC0/WO[6]
 */
#define BREATHING_PORT hal::gpio::GPIOPort::kGPIOPortA
#define BREATHING_PIN 20
#define BREATHING_FUNC hal::gpio::GPIOPinFunction::kGPIOFunctionF
#define BREATHING_GCLKx 4
#define BREATHING_TCCx 0
#define BREATHING_WOx 6

class Breathing {
 public:
  Breathing();
  ~Breathing();

  void init();
  void startBreathing();
  void stopBreathing();
  void setDutyCyle(int dutyCycle);

 private:
  actuator::Motor *motor_;
};

#endif
