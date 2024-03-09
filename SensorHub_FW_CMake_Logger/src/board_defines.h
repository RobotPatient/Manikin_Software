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

#define USB_DEVICE_TASK_STACK_SIZE 2048


StackType_t usb_device_stack[USB_DEVICE_TASK_STACK_SIZE];
StaticTask_t usb_device_taskdef;

StackType_t blinky_stack[configMINIMAL_STACK_SIZE];
StaticTask_t blinky_taskdef;

StackType_t usb_write_stack[configNORMAL_STACK_SIZE];
StaticTask_t usb_write_taskdef;

#endif /* BOARD_DEFINES_H */