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
#include <limits.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include <task.h>
#include <timers.h>

/* Universal hal includes */
#include <hal_usb_serial.h>
#include <hal_gpio.h>

/* Manikin_software_libraries includes */
#include "usb_service_protocol.hpp"
#include "MB85RS2MTA.h"

/* Project includes */
#include "clock_init.h"
#include "board_defines.h"
#include "system_status_types.h"
#include "sensor_measurement.h"
#include "usb_protocol.h"
#include "fram_helper.h"

static inline system_status_t get_default_system_state() {
  return (system_status_t){
    .hub_id = 0, 
    .hub_type = BOARD_TYPE, 
    .hub_status = DRIVERS_INITIALIZED, 
    .usb_status = USB_STATUS_INACTIVE, 
    .memory_status = MEMORY_SPACE_LEFT,
    .port_status = PORT_UNITIALIZED, 
    .device_type_a = static_cast<device_type_t>(DEFAULT_SENSOR_TYPE_PORT_A), 
    .device_type_b = static_cast<device_type_t>(DEFAULT_SENSOR_TYPE_PORT_B), 
    .sample_time_a = DEFAULT_SAMPLE_RATE, 
    .sample_time_b = DEFAULT_SAMPLE_RATE, 
    .frame_num = 0
  };
}

int8_t update_usb_status(system_status_t* local_version) {
  BaseType_t res;
  if (xSemaphoreTake(USBProtoStatusMutex, 10) == pdTRUE) {
    status_shared_w_usb = *local_version;
    res = xSemaphoreGive(USBProtoStatusMutex);
  }
  return res;
}

int8_t handle_system_event(uint32_t ev, system_status_t* local_system_status) {
  const uint8_t recv_event = GET_EV_BITS_FROM_EV_VAL(ev);
  switch (recv_event) {
    case EV_ID_CHANGE: {
      const uint32_t id = GET_ID_BITS_FROM_ID_EV(ev);
      local_system_status->hub_id = id;
      if (xTaskNotify(fram_manager_task_handle, ev, eSetValueWithOverwrite) == pdTRUE) {
        update_usb_status(local_system_status);
      }
      break;
    }
    case EV_START: {
      if (xTaskNotify(sensor_hypervisor_task_handle, ev, eSetValueWithOverwrite) == pdTRUE) {
        local_system_status->hub_status = CAPTURING;
        update_usb_status(local_system_status);
      }
      break;
    }
    case EV_STOP: {
      if (xTaskNotify(sensor_hypervisor_task_handle, ev, eSetValueWithOverwrite) == pdTRUE) {
        local_system_status->hub_status = DRIVERS_INITIALIZED;
        update_usb_status(local_system_status);
      }
      break;
    }
    case EV_SAMPLE_RATE_CHANGE: {
      const uint8_t sample_rate_port_a = GET_SR_PA_BITS_FROM_SR_EV(ev);
      const uint8_t sample_rate_port_b = GET_SR_PB_BITS_FROM_SR_EV(ev);
      if (local_system_status->sample_time_a != sample_rate_port_a ||
          local_system_status->sample_time_b != sample_rate_port_b) {
        if (xTaskNotify(sensor_hypervisor_task_handle, ev, eSetValueWithOverwrite) == pdTRUE) {
            update_usb_status(local_system_status);
        }
      }
    }
  }
  return 0;
}

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
    vTaskDelay(HB_BLINK_RATE);
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

static void system_monitor_task(void* pvArg) {
  system_status_t local_system_status;
  BaseType_t value_received;
  uint32_t recv_data;

  local_system_status = get_default_system_state();
  /* Wait for the FRAM driver to post the Hub's Unique ID */
  if (xTaskNotifyWaitIndexed(0, 0x00, ULONG_MAX, &recv_data, portMAX_DELAY) == pdTRUE) {
    local_system_status.hub_id = (recv_data & 0xFF);
    /* Set the shared status object to our local version */
    if (xSemaphoreTake(USBProtoStatusMutex, 10) == pdTRUE) {
      status_shared_w_usb = local_system_status;
      BaseType_t semaphore_released = xSemaphoreGive(USBProtoStatusMutex);
      configASSERT(semaphore_released);
    }
  }
  while (1) {
    value_received = xTaskNotifyWait(0, 0x00, &recv_data, portMAX_DELAY);
    if (value_received == pdPASS) {
      int8_t res = handle_system_event(recv_data, &local_system_status);
      configASSERT(res);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

int main(void) {
  /*
   * Set the main clock to 48MHz, instead of the default 8 MHz
   */
  Clock_Init();

  int res = init_usb_pins();

  if (res != UHAL_STATUS_OK) {
    while (1)
      ;
  }

  res = init_flash_pins();

  if (res != UHAL_STATUS_OK) {
    while (1)
      ;
  }

  res = init_i2c_pins();

  if (res != UHAL_STATUS_OK) {
    while (1)
      ;
  }

  USBProtoStatusMutex = xSemaphoreCreateMutexStatic(&USBProtoStatusMutexBuf);
  configASSERT(USBProtoStatusMutex);

  TaskHandle_t usb_device_task_handle =
      xTaskCreateStatic(usb_device_task, "usbd", USB_DEVICE_TASK_STACK_SIZE, NULL, configMAX_PRIORITIES - 1,
                        usb_device_stack, &usb_device_taskdef);
  configASSERT(usb_device_task_handle);

  TaskHandle_t blinky_task_handle =
      xTaskCreateStatic(blink, "blinky", configMINIMAL_STACK_SIZE, NULL, 1, blinky_stack, &blinky_taskdef);

  configASSERT(blinky_task_handle);

  TaskHandle_t usb_protocol_task_handle = xTaskCreateStatic(usb_proto_task, "usbprototask", configNORMAL_STACK_SIZE,
                                                            NULL, 2, usb_write_stack, &usb_write_taskdef);

  configASSERT(usb_protocol_task_handle);

  system_monitor_task_handle = xTaskCreateStatic(system_monitor_task, "systemmonitor", configNORMAL_STACK_SIZE, NULL, 2,
                                                 system_monitor_stack, &system_monitor_taskdef);

  configASSERT(system_monitor_task_handle);

  fram_manager_task_handle = xTaskCreateStatic(fram_manager_task, "fram_manager", configMINIMAL_SECURE_STACK_SIZE, NULL,
                                               2, fram_manager_stack, &fram_manager_taskdef);

  configASSERT(fram_manager_task_handle);

  sensor_hypervisor_task_handle = xTaskCreateStatic(Sensx_hypervisor, "SensorHypervisor", configNORMAL_STACK_SIZE, NULL,
                                                    2, sensor_hypervisor_stack, &sensor_hypervisor_taskdef);

  configASSERT(system_monitor_task_handle);

  vTaskStartScheduler();

  while (1) {}
  vTaskDelete(blinky_task_handle);
  vTaskDelete(usb_protocol_task_handle);
  vTaskDelete(usb_device_task_handle);
}
