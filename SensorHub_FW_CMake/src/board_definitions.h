//
// Created by victor on 7/19/23.
//

#ifndef ATMELSAMD21_BOARD_DEFINITIONS_H
#define ATMELSAMD21_BOARD_DEFINITIONS_H

#include <hal_spi_host.h>
#include <hal_i2c_host.h>

#define MAIN_CLOCK_SPEED 48000000
#define I2C_CLOCK_SPEED 1000000

const gpio_pin_t led_pin = GPIO_PIN_PA2;
const gpio_pin_t port_a_sda_pin = GPIO_PIN_PA16;
const gpio_pin_t port_a_scl_pin = GPIO_PIN_PA17;
const gpio_pin_t port_b_sda_pin = GPIO_PIN_PA12;
const gpio_pin_t port_b_scl_pin = GPIO_PIN_PA13;
const gpio_pin_t port_backbone_sda_pin = GPIO_PIN_PA22;
const gpio_pin_t port_backbone_scl_pin = GPIO_PIN_PA23;

const gpio_pin_t flash_miso = GPIO_PIN_PA14;
const gpio_pin_t flash_mosi = GPIO_PIN_PA8;
const gpio_pin_t flash_sck = GPIO_PIN_PA9;
const gpio_pin_t flash_ss = GPIO_PIN_PA7;

#define I2C_HOST_INST_PORT_A I2C_PERIPHERAL_4
#define I2C_HOST_INST_PORT_B I2C_PERIPHERAL_1
#define I2C_SLAVE_INST_BACKBONE_PORT I2C_PERIPHERAL_3

#define MAX_SENSORS_PER_PORT 6 // For now, we allow up to six sensors per Sensor Port (PORT_A and PORT_B). This allows the use of up to two extenders per port.
#define DEFAULT_SLAVE_ADDR 0x10
#define ALT_DEFAULT_SLAVE_ADDR 0x11

const fram_dev_t fram = {
        .spi_bus = SPI_PERIPHERAL_2,
        .cs_pin = flash_ss
};


void init_pins() {
    /* HeartBeat led pin*/
    GPIO_SET_PIN_MODE(led_pin, GPIO_MODE_OUTPUT);

    /* I2C Pins */
    GPIO_SET_PIN_MODE(port_a_scl_pin, GPIO_MODE_C);
    GPIO_SET_PIN_MODE(port_a_sda_pin, GPIO_MODE_C);
    GPIO_SET_PIN_MODE(port_b_scl_pin, GPIO_MODE_D);
    GPIO_SET_PIN_MODE(port_b_sda_pin, GPIO_MODE_D);
    GPIO_SET_PIN_MODE(port_backbone_scl_pin, GPIO_MODE_C);
    GPIO_SET_PIN_MODE(port_backbone_sda_pin, GPIO_MODE_C);

    /* SPI Pins */
    GPIO_SET_PIN_MODE(flash_miso, GPIO_MODE_C);
    GPIO_SET_PIN_MODE(flash_mosi, GPIO_MODE_D);
    GPIO_SET_PIN_MODE(flash_sck, GPIO_MODE_D);
    GPIO_SET_PIN_MODE(flash_ss, GPIO_MODE_OUTPUT);
}

void init_fram() {
    const spi_bus_opt_t extra_config_opt = static_cast<spi_bus_opt_t>(SPI_BUS_OPT_DOPO_PAD_0 | SPI_BUS_OPT_DIPO_PAD_2);

    SPI_HOST_INIT(SPI_PERIPHERAL_2, SPI_CLK_SOURCE_USE_DEFAULT, MAIN_CLOCK_SPEED, 10e6, extra_config_opt);

    fram_init(fram);
}


#endif //ATMELSAMD21_BOARD_DEFINITIONS_H
