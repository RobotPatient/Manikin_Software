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