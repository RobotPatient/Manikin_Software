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
#ifndef COMPRESSION_HPP
#define COMPRESSION_HPP

#include <Arduino.h>
#include <Servo.h>

#include <motor.hpp>

#include "BreathingData.hpp"

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
 * Use PA19, funtion E
 * GCLK?, TC3/WO[1]
 */
#define COMPRESSION_PORT hal::gpio::GPIOPort::kGPIOPortA
#define COMPRESSION_PIN 21
#define COMPRESSION_FUNC hal::gpio::GPIOPinFunction::kGPIOFunctionE
#define COMPRESSION_GCLKx 3
#define COMPRESSION_TCx 3
#define COMPRESSION_WOx 1

class Compression {
 public:
  /**
   * @brief Construct a new Compression object
   *
   */
  Compression();
  ~Compression();

  void init();
  void StartCompression();
  void setDutyCycle(int);

 private:
  actuator::Motor *motor_;
  Servo *servo_;
  SampleData CurrentDepthPoint_;
};

#endif
