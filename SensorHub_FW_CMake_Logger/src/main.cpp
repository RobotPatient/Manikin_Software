#include <hal_gpio.h>
#include <hal_usb_serial.h>
#include "clock_init.h"
#include "FreeRTOS.h"
#include <task.h>
#include <timers.h>
#include <tusb.h>
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



static void usb_device_task(void *param) {
  (void) param;

  // init device stack on configured roothub port
  // This should be called after scheduler/kernel is started.
  // Otherwise it could cause kernel issue since USB IRQ handler does use RTOS queue API
  // RTOS forever loop
  usb_serial_init(USB_SERIAL_INST, USB_CLK_SOURCE_USE_DEFAULT, CONF_CPU_FREQUENCY);
  while (1) {
    usb_serial_poll_task();
  }
}

void blink(void* pvArg) {
  GPIO_SET_PIN_MODE(HB_LED, GPIO_MODE_OUTPUT);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  while(1) {
    /* Fetch new characters / background task */
    usb_serial_write_string(USB_SERIAL_0, "Hello World!\n");
    vTaskDelay(100/portTICK_PERIOD_MS);
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
    xTaskCreate(usb_device_task, "usbd", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(blink, "blink task", 2048, NULL, 0, NULL);
          vTaskStartScheduler();


    while (1) {
    }
}
