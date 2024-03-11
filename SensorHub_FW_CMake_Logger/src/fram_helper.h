/**
 * @file fram_helper.h
 * @author Victor Hogeweij (hogeweyv@gmail.com, vo.hogeweij@student.han.nl)
 * @brief This file contains everything related to driving the FRAM chip onboard of the Sensor/ActuatorHub
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

#ifndef FRAM_HELPER_H
#define FRAM_HELPER_H
TaskHandle_t fram_manager_task_handle;
fram_dev_t fram_dev = {.spi_bus = SPI_PERIPHERAL_2, .cs_pin = FLASH_CS};

// void fram_manager_task(void* PvArg) {
//   uint8_t val = 0;
//   spi_bus_opt_t bus_opt = static_cast<spi_bus_opt_t>((SPI_BUS_OPT_DIPO_PAD_2 | SPI_BUS_OPT_DOPO_PAD_0));
//   spi_clock_sources_t clock_opt =
//       static_cast<spi_clock_sources_t>(SPI_CLK_SOURCE_FAST_CLKGEN0 | SPI_CLK_SOURCE_SLOW_CLKGEN1);
//   spi_host_init(SPI_PERIPHERAL_2, clock_opt, CONF_CPU_FREQUENCY, (8e6), bus_opt);
//   fram_init(fram_dev);
//   fram_read_byte(fram_dev, 1, &val);
//   fram_wren(fram_dev);
//   fram_write_byte(fram_dev, 1, 10);
//   xTaskNotify(system_monitor_task_handle, val, eSetValueWithOverwrite);
//   while (1) {
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//   }
// }

void fram_manager_task(void* PvArg) {
  uint8_t val = 0;
  spi_bus_opt_t bus_opt = static_cast<spi_bus_opt_t>((SPI_BUS_OPT_DIPO_PAD_2 | SPI_BUS_OPT_DOPO_PAD_0));
  spi_clock_sources_t clock_opt =
      static_cast<spi_clock_sources_t>(SPI_CLK_SOURCE_FAST_CLKGEN0 | SPI_CLK_SOURCE_SLOW_CLKGEN1);
  taskENTER_CRITICAL();
  spi_host_init(SPI_PERIPHERAL_2, clock_opt, CONF_CPU_FREQUENCY, (8e6), bus_opt);
  fram_init(fram_dev);
  fram_read_byte(fram_dev, 1, &val);
  taskEXIT_CRITICAL();
  xTaskNotify(system_monitor_task_handle, val, eSetValueWithOverwrite);
  BaseType_t value_received;
  uint32_t recv_data;
  while (1) {
    value_received = xTaskNotifyWait(0, 0x00, &recv_data, portMAX_DELAY);
    if(value_received == pdPASS) {
      const uint8_t recv_event = GET_EV_BITS_FROM_EV_VAL(recv_data);
      switch(recv_event) {
        case EV_ID_CHANGE:
        {
          taskENTER_CRITICAL();
          const uint8_t id = GET_ID_BITS_FROM_ID_EV(recv_data);
          fram_wren(fram_dev);
          fram_write_byte(fram_dev, 1, id);
          taskEXIT_CRITICAL();
          break;
        }
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

#endif /* FRAM_HELPER_H */