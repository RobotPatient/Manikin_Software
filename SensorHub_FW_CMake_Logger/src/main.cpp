#include <hal_gpio.h>
#include <hal_usb_serial.h>
#include "clock_init.h"
#include "FreeRTOS.h"
#include <task.h>
#include <timers.h>
#include <tusb.h>
#include <lib/hw/bsp/board_api.h>
/*
* USB Pins
*/
#define USBDM                       GPIO_PIN_PA24
#define USBDP                       GPIO_PIN_PA25

/*
 * LED PIN
 */
#define HB_LED GPIO_PIN_PA2

/**
 * USB Serial Instance,
 * Multiple VCOM instances can be initialized by increasing the CFG_TUD_CDC define
*/
#define USB_SERIAL_INST             USB_SERIAL_0

/**
 * Example settings,
 * ECHO_PUT_CH_NUM is the number of characters being buffered
 * ECHO_PUT_READ_UNTIL_NEWLINE stops reading when newline character comes in
*/
#define ECHO_PUT_CH_NUM             1
#define ECHO_PUT_READ_UNTIL_NEWLINE 0



StackType_t  usb_device_stack[4096];
StaticTask_t usb_device_taskdef;

StackType_t blinky_stack[configMINIMAL_STACK_SIZE];
StaticTask_t blinky_taskdef;

void HardFault_Handler() {

}


static void usb_device_task(void *param) {
  (void) param;

  // init device stack on configured roothub port
  // This should be called after scheduler/kernel is started.
  // Otherwise it could cause kernel issue since USB IRQ handler does use RTOS queue API
  // RTOS forever loop
  // while (1) {
  //   usb_serial_poll_task();
  //   vTaskDelay(1/portTICK_PERIOD_MS);
  // }
  usb_serial_init(USB_SERIAL_INST, USB_CLK_SOURCE_USE_DEFAULT, CONF_CPU_FREQUENCY);
  vTaskDelay(10/portTICK_PERIOD_MS);
  volatile int res = tud_init(BOARD_TUD_RHPORT);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

  // RTOS forever loop
  while (1) {
    // put this thread to waiting state until there is new events
    tud_task();

    // following code only run if tud_task() process at least 1 event
    tud_cdc_write_flush();
  }
}

void blink(void* pvArg) {
  GPIO_SET_PIN_MODE(HB_LED, GPIO_MODE_OUTPUT);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  while(1)     {
    {
      // There are data available
      while (tud_cdc_available()) {
        uint8_t buf[64];

        // read and echo back
        uint32_t count = tud_cdc_read(buf, sizeof(buf));
        (void) count;

        // Echo back
        // Note: Skip echo by commenting out write() and write_flush()
        // for throughput test e.g
        //    $ dd if=/dev/zero of=/dev/ttyACM0 count=10000
        tud_cdc_write(buf, count);
      }

      tud_cdc_write_flush();
    }

    // For ESP32-Sx this delay is essential to allow idle how to run and reset watchdog
    vTaskDelay(1);
  }
}

int main(void) {
    /*
     * Set the main clock to 48MHz
     */
    Clock_Init();


    for(uint32_t i=0; i< 10000; i++);
    /*
     * Set the MUX for the USB pins
     */
    GPIO_SET_PIN_MODE(USBDM, GPIO_MODE_OUTPUT);
    GPIO_SET_PIN_MODE(USBDP, GPIO_MODE_OUTPUT);
    GPIO_SET_PIN_LVL(USBDM, GPIO_LOW);
    GPIO_SET_PIN_LVL(USBDP, GPIO_LOW);
    GPIO_SET_PIN_MODE(USBDM, GPIO_MODE_G);
    GPIO_SET_PIN_MODE(USBDP, GPIO_MODE_G);

    xTaskCreateStatic(usb_device_task, "usbd", 4096, NULL, configMAX_PRIORITIES-1, usb_device_stack, &usb_device_taskdef);
  xTaskCreate(blink, "blinky", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();


    while (1) {
    }
}
