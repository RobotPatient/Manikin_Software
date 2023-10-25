#include "Slave_driver.hpp"
#include "FastCRC.h"
#include "sam.h"
#include <hal_i2c_slave.h>

#define NUM_OF_I2C_REGISTERS 8

#define I2C_SLAVE_REG_STATUS_SIZE 2
#define I2C_SLAVE_REG_I2CCON_SIZE 3
#define I2C_SLAVE_REG_PORTACONF_SIZE 2
#define I2C_SLAVE_REG_PORTADATA_SIZE 255
#define I2C_SLAVE_REG_PORTASAMPLERDY_SIZE 2
#define I2C_SLAVE_REG_PORTBCONF_SIZE 2
#define I2C_SLAVE_REG_PORTBDATA_SIZE 255
#define I2C_SLAVE_REG_PORTBSAMPLERDY_SIZE 2


#define I2C_PERMISSION_RW 0
#define I2C_PERMISSION_RO 1

#define I2C_REGS_MAX_ADDR 7

uint8_t i2c_registers[576];
uint8_t i2c_registers_copy[576];

i2c_slave_reg_t* reg = (i2c_slave_reg_t *)(i2c_registers);

FastCRC8 CRC_Gen;

typedef struct {
    uint8_t *buf;
    size_t size;
    uint8_t permission;
    uint8_t crc;
}i2c_registers_base_t;

i2c_registers_base_t registers[NUM_OF_I2C_REGISTERS] {
        {reg->STATUS, I2C_SLAVE_REG_STATUS_SIZE, I2C_PERMISSION_RO, 0},
        {reg->I2CCON, I2C_SLAVE_REG_I2CCON_SIZE, I2C_PERMISSION_RW, 0},
        {reg->PORTACONF, I2C_SLAVE_REG_PORTACONF_SIZE, I2C_PERMISSION_RW, 0},
        {reg->PORTADATA, I2C_SLAVE_REG_PORTADATA_SIZE, I2C_PERMISSION_RW, 0},
        {reg->PORTASAMPLERDY, I2C_SLAVE_REG_PORTASAMPLERDY_SIZE, I2C_PERMISSION_RW, 0},
        {reg->PORTBCONF, I2C_SLAVE_REG_PORTBCONF_SIZE, I2C_PERMISSION_RW, 0},
        {reg->PORTBDATA, I2C_SLAVE_REG_PORTBDATA_SIZE, I2C_PERMISSION_RW, 0},
        {reg->PORTBSAMPLERDY, I2C_SLAVE_REG_PORTBSAMPLERDY_SIZE, I2C_PERMISSION_RW, 0}
};


struct I2CTransaction {
    bool error_occured;
    i2c_registers_base_t *reg;
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
                CurrTransaction.error_occured = 1;
            }
            else {
                CurrTransaction.reg = &registers[data];
                CurrTransaction.byte_cnt++;
            }

        } else {
            if(CurrTransaction.reg != NULL) {
                if (CurrTransaction.byte_cnt < CurrTransaction.reg->size+1 ) {
                    if(CurrTransaction.reg->permission) {
                        CurrTransaction.reg->buf[CurrTransaction.byte_cnt++ - 1] = data;
                    }
                    else {
                        CurrTransaction.error_occured = 1;
                    }
                } else if (CurrTransaction.byte_cnt == CurrTransaction.reg->size+1) {
                    CurrTransaction.crc = data;
                    uint8_t tempcrc = CRC_Gen.maxim(CurrTransaction.reg->buf, CurrTransaction.reg->size);
                    if(CurrTransaction.crc != tempcrc){

                    }
                } else {
                    CurrTransaction.error_occured = 1;
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


void reassign_internal_register(i2c_slave_reg_t* new_reg) {
    registers[0].buf = new_reg->STATUS;
    registers[1].buf = new_reg->I2CCON;
    registers[2].buf = new_reg->PORTACONF;
    registers[3].buf = new_reg->PORTADATA;
    registers[4].buf = new_reg->PORTASAMPLERDY;
    registers[5].buf = new_reg->PORTBCONF;
    registers[6].buf = new_reg->PORTBDATA;
    registers[7].buf = new_reg->PORTBSAMPLERDY;
}


void I2CSlaveDriver::init(uint8_t slave_addr) {
    I2C_SLAVE_INIT(peripheral_, slave_addr, I2C_CLK_SOURCE_USE_DEFAULT, I2C_EXTRA_OPT_NONE);
}

void I2CSlaveDriver::set_register_buffer(i2c_slave_reg_t *reg) {
    reassign_internal_register(reg);
}

void I2CSlaveDriver::recalculate_crc() {
    for(uint8_t i = 0; i < NUM_OF_I2C_REGISTERS; i++){
        registers[i].crc = CRC_Gen.maxim(registers[i].buf, registers[i].size);
    }
}