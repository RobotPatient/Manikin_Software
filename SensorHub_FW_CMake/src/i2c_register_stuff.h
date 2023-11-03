//
// Created by victor on 7/19/23.
//

#ifndef ATMELSAMD21_I2C_REGISTER_STUFF_H
#define ATMELSAMD21_I2C_REGISTER_STUFF_H
#include <stddef.h>
#include <stdbool.h>

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

void i2c_slave_data_send_irq(const void *const hw, volatile bustransaction_t *bustransaction){
    ((Sercom*)hw)->I2CS.DATA.reg = 0;

}

void i2c_slave_data_recv_irq(const void *const hw, volatile bustransaction_t *bustransaction){
    const uint8_t data = ((Sercom*)hw)->I2CS.DATA.reg;
    if(!CurrTransaction.error_occured) {
      if (CurrTransaction.byte_cnt == 0) {
        if (data > I2C_REGS_MAX_ADDR) {
          i2c_regs[I2C_REGS_I2C_CONN_INDEX].buffer[I2C_REGS_I2C_CONN_I2C_ERROR] = I2C_ERROR_CODE_INVALID_REGISTER;
          CurrTransaction.error_occured = 1;
        }
        else {
          CurrTransaction.reg = &i2c_regs[data];
          CurrTransaction.byte_cnt++;
        }

      } else {
        if(CurrTransaction.reg != NULL) {
          if (CurrTransaction.byte_cnt < CurrTransaction.reg->size+1 ) {
            if(CurrTransaction.reg->RWPermissions) {
              CurrTransaction.reg->buffer[CurrTransaction.byte_cnt++ - 1] = data;
            }
            else {
              CurrTransaction.error_occured = 1;
              i2c_regs[I2C_REGS_I2C_CONN_INDEX].buffer[I2C_REGS_I2C_CONN_I2C_ERROR] = I2C_ERROR_ACCESS_PERMISSION_VIOLATION;
            }
          } else if (CurrTransaction.byte_cnt == CurrTransaction.reg->size+1) {
            CurrTransaction.crc = data;
          } else {
            CurrTransaction.error_occured = 1;
            i2c_regs[I2C_REGS_I2C_CONN_INDEX].buffer[I2C_REGS_I2C_CONN_I2C_ERROR] = I2C_ERROR_OUT_OF_BOUNDS;
          }
        }
      }
    }

}

void i2c_slave_stop_irq(const void *const hw, volatile bustransaction_t *bustransaction) {
    CurrTransaction.error_occured = 0;
    CurrTransaction.reg = NULL;
    CurrTransaction.byte_cnt =0;
    CurrTransaction.crc = 0;
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_PREC;
}
#endif //ATMELSAMD21_I2C_REGISTER_STUFF_H
