//
// Created by victor on 7/19/23.
//

#ifndef ATMELSAMD21_BOARD_DEFINITIONS_H
#define ATMELSAMD21_BOARD_DEFINITIONS_H

#include <hal_spi_host.h>
#include <hal_i2c_host.h>

#define MAIN_CLOCK_SPEED 48000000
#define I2C_CLOCK_SPEED 1000000
const gpio_pin_t LedPin = {GPIO_PORT_A, 2};
const gpio_pin_t kBackBoneSDA = {GPIO_PORT_A, 12};
const gpio_pin_t kBackBoneSCL = {GPIO_PORT_A, 13};

const gpio_pin_t flash_miso = {GPIO_PORT_A, 14};
const gpio_pin_t flash_mosi = {GPIO_PORT_A, 8};
const gpio_pin_t flash_sck = {GPIO_PORT_A, 9};
const gpio_pin_t flash_ss = {GPIO_PORT_A, 7};

#define I2C_HOST_INST I2C_SERCOM4
static const i2c_host_conf_opt_t i2c_host_options = {
    .clock_sources = I2C_CLK_SOURCE_USE_DEFAULT,
    .periph_clk_freq = MAIN_CLOCK_SPEED,
    .baud_rate_freq = I2C_CLOCK_SPEED,
    .extra_configuration_options = I2C_EXTRA_OPT_NONE
};

const spi_periph_inst_t spi_periph = {
        .sercom_inst_num = SERCOM_NUM_2,
        .sercom_inst = SERCOM2,
        .clk_gen_fast = CLKGEN_0,
        .fast_clk_gen_frequency = MAIN_CLOCK_SPEED,
        .clk_gen_slow = CLKGEN_3,
        .operating_mode = SPI_OPERATING_MODE_MASTER,
        .dopo_pad = SERCOM_DOPO_PAD0,
        .dipo_pad = SERCOM_DIPO_PAD2
};


#endif //ATMELSAMD21_BOARD_DEFINITIONS_H
