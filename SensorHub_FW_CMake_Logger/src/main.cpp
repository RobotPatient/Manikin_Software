/**
 * @file main.cpp
 * @author Victor Hogeweij (hogeweyv@gmail.com, vo.hogeweij@student.han.nl)
 * @brief Barebones Measurement Logger with USB Functionality for the BabySimulator project
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

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include <task.h>
#include <timers.h>

/* Universal hal includes */
#include <hal_usb_serial.h>
#include <hal_gpio.h>

/* Manikin_software_libraries includes */
#include "usb_service_protocol.hpp"

/* Project includes */
#include "clock_init.h"
#include "board_defines.h"
#include "usb_protocol.h"


/**
 * @brief This task will function as USB daemon, this had to be a task, as the poll_task func is not compatible with timers :)
 * 
 * @param pvArg This is left unused, but it is left in as FreeRTOS requires this parameter
 */
static void usb_device_task(void* pvArg) {
  usb_serial_init(USB_SERIAL_INST, USB_CLK_SOURCE_USE_DEFAULT, CONF_CPU_FREQUENCY);
  vTaskDelay(10 / portTICK_PERIOD_MS);
  while (1) {
    // put this thread to waiting state until there is new events
    usb_serial_poll_task();
  }
}

/**
 * @brief This task functions as heartbeat task, it helps with debugging (ofc for more debugging and general development a debug probe was used)
 * 
 * @param pvArg This is left unused, but it is left in as FreeRTOS requires this parameter
 */
static void blink(void* pvArg) {
  GPIO_SET_PIN_MODE(HB_LED, GPIO_MODE_OUTPUT);
  while (1) {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    GPIO_TOGGLE_PIN_OUTPUT(HB_LED);
  }
}


/**
 * @brief This task runs our custom serial protocol on top of the serial usb stack.
 * 
 * @param pvArg This is left unused, but it is left in as FreeRTOS requires this parameter
 */
static void usb_proto_task(void* pvArg) {
  /* Wait to make sure that USB stack has initialized properly */
  vTaskDelay(100 / portTICK_PERIOD_MS);
  /* Create the USB service protocol parser object */
  usb_service_protocol::USBServiceProtocol<USB_SERIAL_INST, USB_SERIAL_RX_BUF_SIZE, NUM_OF_REGISTERS> usbproto;
  /* Intialize the USB service protocol parser with our custom commandset (called Protoreg, see usb_protocol.h)*/
  usbproto.init(usb_service_protocol::Protoreg);
  while (1) {
    /* Poll the USB Serial for new characters and run the commands if appropriate! */
    usbproto.polling_task();
    /* The timing is not critical for this task, that's why the blocking state is achieved with vTaskDelays */
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

int main(void) {
  /*
   * Set the main clock to 48MHz, instead of the default 8 MHz
   */
  Clock_Init();

  int res = init_usb_pins();
  
  if(res != UHAL_STATUS_OK) {
    while(1);
  }

  TaskHandle_t usb_device_task_handle = xTaskCreateStatic(usb_device_task, "usbd", USB_DEVICE_TASK_STACK_SIZE, NULL, configMAX_PRIORITIES - 1,
                                                          usb_device_stack, &usb_device_taskdef);
  if(usb_device_task_handle == NULL) {
    while(1);
  }
  
  TaskHandle_t blinky_task_handle = xTaskCreateStatic(blink, "blinky", configMINIMAL_STACK_SIZE, NULL, 1, blinky_stack, &blinky_taskdef);

  if(blinky_task_handle == NULL) {
    while(1);
  }

  TaskHandle_t usb_protocol_task_handle = xTaskCreateStatic(usb_proto_task, "usbprototask", configNORMAL_STACK_SIZE, NULL, 2, usb_write_stack, &usb_write_taskdef);
  
  if(usb_protocol_task_handle == NULL) {
    while(1);
  }
  
  vTaskStartScheduler();

  while (1) {}
  vTaskDelete(blinky_task_handle);
  vTaskDelete(usb_protocol_task_handle);
  vTaskDelete(usb_device_task_handle);
}