//
// Created by victor on 7/19/23.
//

#ifndef ATMELSAMD21_I2C_REGISTER_STUFF_H
#define ATMELSAMD21_I2C_REGISTER_STUFF_H
#include <stddef.h>

#define I2C_REGS_MAX_ADDR 5
struct Register {
    uint8_t* buffer;
    uint8_t size;
    uint8_t RWPermissions;
};

#define PERMISSIONS_RO 0
#define PERMISSIONS_RW 1

#define I2C_REGS_I2C_CONN_INDEX 1
#define I2C_REGS_I2C_CONN_I2C_ERROR 2

enum I2C_ERROR_CODES {
    I2C_ERROR_CODE_INVALID_REGISTER,
    I2C_ERROR_ACCESS_PERMISSION_VIOLATION,
    I2C_ERROR_DATA_CORRUPTION,
    I2C_ERROR_OUT_OF_BOUNDS,
};

uint8_t STATUS[2];
uint8_t I2CCON[3];
uint8_t PORTACONF[2];
uint8_t PORTADATA[2][256];
uint8_t PORTBCONF[2];
uint8_t PORTBDATA[2][256];


struct Register i2c_regs[6] ={{STATUS, 2, PERMISSIONS_RO},
                              {I2CCON, 3, PERMISSIONS_RW},
                              {PORTACONF, 2, PERMISSIONS_RW},
                              {PORTADATA[0], 255, PERMISSIONS_RO},
                              {PORTBCONF, 2, PERMISSIONS_RW},
                              {PORTBDATA[0], 255, PERMISSIONS_RO}
};
struct I2CTransaction {
    bool error_occured;
    struct Register* reg;
    uint8_t  byte_cnt;
    uint8_t crc;
};

struct I2CTransaction CurrTransaction = {false, NULL, 0, 0};

#endif //ATMELSAMD21_I2C_REGISTER_STUFF_H
