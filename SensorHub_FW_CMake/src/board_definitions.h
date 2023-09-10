//
// Created by victor on 7/19/23.
//

#ifndef ATMELSAMD21_BOARD_DEFINITIONS_H
#define ATMELSAMD21_BOARD_DEFINITIONS_H

#define I2C_CLOCK_SPEED 1000000
const GPIOPin LedPin = {kGPIOPortA, 2};
const GPIOPin kBackBoneSDA = {kGPIOPortA, 12};
const GPIOPin kBackBoneSCL = {kGPIOPortA, 13};
const I2CInst I2CPeriph = {SERCOM_NUM_4, SERCOM4, CLKGEN_3, CLKGEN_0, I2COpModeMaster, 0x54};
#endif //ATMELSAMD21_BOARD_DEFINITIONS_H
