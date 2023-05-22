/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
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
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/
#ifndef DEVICE_SETTINGS_HPP_
#define DEVICE_SETTINGS_HPP_
#ifdef __arm__
#include <Adafruit_SPIFlash.h>
#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include "Wire.h"
#include "wiring_private.h"


/** 
 * @brief All the handles for flash initialization and control
*/
inline constexpr uint8_t kSpiFramSSPin = 9;
Adafruit_FlashTransport_SPI flashTransport(kSpiFramSSPin, &SPI);
Adafruit_SPIFlash flash(&flashTransport);
static const SPIFlash_Device_t my_flash_devices[] = {MB85RS2MTA};

#endif
#endif  // DEVICE_SETTINGS_HPP_
