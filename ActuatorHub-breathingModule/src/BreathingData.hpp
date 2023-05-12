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
#ifndef BREATHINGDATA_HPP
#define BREATHINGDATA_HPP

#include <Arduino.h>

#include <ringbuffer.hpp>

/**
 * @brief SIZEOF_DATA needs to be changed according to the amount of data that
 * the Infant CPR Simulator needs in order to have a single cycle through its
 * breathing loop.
 */
#define SIZEOF_DATA 100

typedef uint16_t datapoint_t;

typedef struct SampleData {
  uint16_t index;
  datapoint_t breathingPoint;
  datapoint_t compressionPoint;
} SampleData;

class BreathingData {
 public:
  BreathingData() {
    for (int i = 0; i < SIZEOF_DATA; i++) {
      breathingBuffer_.push(newDefaultDataPoint(i));
    }
    breathingBuffer_.resetRead();
  }

  SampleData& nextDataPoint() {
    return currentDataPoint_ = breathingBuffer_.read();
  }

  SampleData& getCurrent() { return currentDataPoint_; }

 protected:
  /**
   * @brief Helper function to create a new sample data point on index [i] Only
   * used if no data points are present (mostly for testing)
   * @param i the index of the new data point
   * @return A new SampleData point
   */
  SampleData newDefaultDataPoint(int i) {
    struct SampleData defaultData;
    defaultData.index = i;
    defaultData.breathingPoint = i % 100;
    defaultData.compressionPoint = i % 100;
    return defaultData;
  }

 private:
  RingBufferT<SampleData, SIZEOF_DATA> breathingBuffer_;
  SampleData currentDataPoint_;
};

#endif
