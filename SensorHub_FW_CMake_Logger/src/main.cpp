#include <hal_gpio.h>
#include <hal_usb_serial.h>
#include <lib/hw/bsp/board_api.h>
#include <task.h>
#include <timers.h>
#include <tusb.h>
#include "FreeRTOS.h"
#include "board_defines.h"
#include "clock_init.h"
#include "usb_service_protocol.hpp"

const char help_usage_str[] =
    "**********************************HELP************************************ \r\n" \
    "CMDS: STATUS - Prints: status of the system, connected devices and " \
    "sampletime\r\n" \
    "      SETPORT [DeviceType portA] [DeviceType portB] [DeviceType BackBone] " \
    "\r\n" \
    "             - Setup (sensor) software drivers on the selected port in " \
    "argument\r\n" \
    "      SETID [UniqueDeviceID] - Sets a unique identifier for the system\r\n" \
    "      STREAM - Stream current sensor measurements to this console\r\n" \
    "      SETSR [Sample rate port a] [Sample rate port b] - Sets the sample rate for \r\n" \
    "                                         port a in b in milliseconds (10-1000 ms)\r";

const char* helpcmd_cb(char** args, int num_of_args) {
  return help_usage_str;
}

const char* testcmd_cb(char** args, int num_of_args) {
  return "Hello World!";
}

usb_service_protocol::USBServiceProtocolRegisters Protoreg[2] = {{"TEST", 0, false, testcmd_cb},
                                                                 {"HELP", 0, false, helpcmd_cb}};

static void usb_device_task(void* pvArg) {
  usb_serial_init(USB_SERIAL_INST, USB_CLK_SOURCE_USE_DEFAULT, CONF_CPU_FREQUENCY);
  vTaskDelay(10 / portTICK_PERIOD_MS);
  while (1) {
    // put this thread to waiting state until there is new events
    usb_serial_poll_task();
  }
}

static void blink(void* pvArg) {
  GPIO_SET_PIN_MODE(HB_LED, GPIO_MODE_OUTPUT);
  while (1) {
    // There are data available
    vTaskDelay(500 / portTICK_PERIOD_MS);
    GPIO_TOGGLE_PIN_OUTPUT(HB_LED);
  }
}

static void usb_proto_task(void* pvArg) {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  usb_service_protocol::USBServiceProtocol<USB_SERIAL_INST, 100, 2> usbproto;
  usbproto.init(Protoreg);
  while (1) {
    usbproto.polling_task();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

int main(void) {
  /*
     * Set the main clock to 48MHz
     */
  Clock_Init();

  /*
   * Set the MUX for the USB pins
   */
  GPIO_SET_PIN_MODE(USBDM, GPIO_MODE_OUTPUT);
  GPIO_SET_PIN_MODE(USBDP, GPIO_MODE_OUTPUT);
  GPIO_SET_PIN_LVL(USBDM, GPIO_LOW);
  GPIO_SET_PIN_LVL(USBDP, GPIO_LOW);
  GPIO_SET_PIN_MODE(USBDM, GPIO_MODE_G);
  GPIO_SET_PIN_MODE(USBDP, GPIO_MODE_G);

  xTaskCreateStatic(usb_device_task, "usbd", USB_DEVICE_TASK_STACK_SIZE, NULL, configMAX_PRIORITIES - 1,
                    usb_device_stack, &usb_device_taskdef);
  xTaskCreateStatic(blink, "blinky", configMINIMAL_STACK_SIZE, NULL, 1, blinky_stack, &blinky_taskdef);
  xTaskCreateStatic(usb_proto_task, "usbwrtask", configNORMAL_STACK_SIZE, NULL, 2, usb_write_stack, &usb_write_taskdef);
  vTaskStartScheduler();

  while (1) {}
}
