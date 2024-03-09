#include <hal_gpio.h>
#include <hal_usb_serial.h>
#include <lib/hw/bsp/board_api.h>
#include <task.h>
#include <timers.h>
#include <tusb.h>
#include "FreeRTOS.h"
#include "clock_init.h"
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

/**
 * Example settings,
 * ECHO_PUT_CH_NUM is the number of characters being buffered
 * ECHO_PUT_READ_UNTIL_NEWLINE stops reading when newline character comes in
*/
#define ECHO_PUT_CH_NUM 1
#define ECHO_PUT_READ_UNTIL_NEWLINE 0

#define USB_DEVICE_TASK_STACK_SIZE 2048

StackType_t usb_device_stack[USB_DEVICE_TASK_STACK_SIZE];
StaticTask_t usb_device_taskdef;

StackType_t blinky_stack[configMINIMAL_STACK_SIZE];
StaticTask_t blinky_taskdef;

StackType_t usb_write_stack[configMINIMAL_STACK_SIZE];
StaticTask_t usb_write_taskdef;

static void usb_device_task(void* param) {
  (void)param;

  usb_serial_init(USB_SERIAL_INST, USB_CLK_SOURCE_USE_DEFAULT, CONF_CPU_FREQUENCY);
  vTaskDelay(10 / portTICK_PERIOD_MS);
  while (1) {
    // put this thread to waiting state until there is new events
    usb_serial_poll_task();
  }
}

static void usb_write_task(void* param) {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  while (1) {
    while (usb_serial_available(USB_SERIAL_INST)) {
      usb_serial_write_string(USB_SERIAL_INST, "Hello World!\n");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelay(10/portTICK_PERIOD_MS);
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
  xTaskCreateStatic(usb_write_task, "usbwrtask", configMINIMAL_STACK_SIZE, NULL, 1, usb_write_stack, &usb_write_taskdef);
  vTaskStartScheduler();

  while (1) {}
}
