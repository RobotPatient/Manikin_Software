/**
 * @file board_defines.h
 * @author Victor Hogeweij (hogeweyv@gmail.com, vo.hogeweij@student.han.nl)
 * @brief This file has the pin definitions and specific settings for this mcu/board
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024 by RobotPatient Simulators
 *
 * Authors: Victor Hogeweij
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
 */

#ifndef BOARD_DEFINES_H
#define BOARD_DEFINES_H

/*
* USB Pins
*/
#define USBDM GPIO_PIN_PA24
#define USBDP GPIO_PIN_PA25

/*
 * LED PIN
 */
#define HB_LED GPIO_PIN_PA2

/**
 * USB Serial Instance,
 * Multiple VCOM instances can be initialized by increasing the CFG_TUD_CDC define
*/
#define USB_SERIAL_INST USB_SERIAL_0

#define USB_SERIAL_RX_BUF_SIZE 100

#define USB_DEVICE_TASK_STACK_SIZE 2048

#define NUM_OF_I2C_PORTS 3

uhal_status_t init_usb_pins() {
   int retval = UHAL_STATUS_OK;
   
   /*
   * Set the MUX for the USB pins
   */
  retval |= GPIO_SET_PIN_MODE(USBDM, GPIO_MODE_OUTPUT);
  retval |= GPIO_SET_PIN_MODE(USBDP, GPIO_MODE_OUTPUT);
  retval |= GPIO_SET_PIN_LVL(USBDM, GPIO_LOW);
  retval |= GPIO_SET_PIN_LVL(USBDP, GPIO_LOW);
  retval |= GPIO_SET_PIN_MODE(USBDM, GPIO_MODE_G);
  retval |= GPIO_SET_PIN_MODE(USBDP, GPIO_MODE_G);
  return static_cast<uhal_status_t>(retval);
}


StackType_t usb_device_stack[USB_DEVICE_TASK_STACK_SIZE];
StaticTask_t usb_device_taskdef;

StackType_t blinky_stack[configMINIMAL_STACK_SIZE];
StaticTask_t blinky_taskdef;

StackType_t usb_write_stack[configNORMAL_STACK_SIZE];
StaticTask_t usb_write_taskdef;

#endif /* BOARD_DEFINES_H */