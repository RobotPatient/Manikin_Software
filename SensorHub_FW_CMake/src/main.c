#include "sam.h"
#include <hal_i2c.h>
#include <hal_gpio.h>
#include <Clock_stuff.h>
#include <i2c_register_stuff.h>
#include <board_definitions.h>
#include <FreeRTOS.h>
#include <task.h>
#include "hal_spi.h"
#define STACK_SIZE 200

/* Structure that will hold the TCB of the task being created. */
StaticTask_t xTaskBuffer;

/* Buffer that the task being created will use as its stack.  Note this is
an array of StackType_t variables.  The size of StackType_t is dependent on
the RTOS port. */
StackType_t xStack[ STACK_SIZE ];

const unsigned char testbuf[30][3] ={{0x02, 0x07,0x01},
                                     {0x02, 0x08,0x01},
                                     {0x00, 0x96,0x00},
                                     {0x00, 0x97, 0xfd},
                                     {0x00, 0xe3, 0x00},
                                     {0x00, 0xe4, 0x04},
                                     {0x00, 0xe5, 0x02},
                                     {0x00, 0xe6, 0x01},
                                     {0x00, 0xe7, 0x03},
                                     {0x00, 0xf5, 0x02},
                                     {0x00, 0xd9, 0x05},
                                     {0x00, 0xdb, 0xce},
                                     {0x00, 0xdc, 0x03},
                                     {0x00, 0xdd, 0xf8},
                                     {0x00, 0x9f, 0x00},
                                     {0x00, 0xa3, 0x3c},
                                     {0x00, 0xb7, 0x00},
                                     {0x00, 0xbb, 0x3c},
                                     {0x00, 0xb2, 0x09},
                                     {0x00, 0xca, 0x09},
                                     {0x01, 0x98, 0x01},
                                     {0x01, 0xb0, 0x17},
                                     {0x01, 0xad, 0x00},
                                     {0x00, 0xff, 0x05},
                                     {0x01, 0x00, 0x05},
                                     {0x01, 0x99, 0x05},
                                     {0x01, 0xa6, 0x1b},
                                     {0x01, 0xac, 0x3e},
                                     {0x01, 0xa7, 0x1f},
                                     {0x00, 0x30, 0x00}};

//const unsigned char resetreg[3] = {0x00, 0x16, 0xB4};
const unsigned char StartMeasurement[3] = {0x00, 0x18, 0x01};
const unsigned char InterruptClear[3] = {0x00, 0x15, 0x07};
const unsigned char ReadRange[2] = {0x00, 0x62};
//static const unsigned char idreg[3] = {0x00, 0x00, 0xB4};
static uint8_t data;
/* Function that implements the task being created. */
void vTaskCode( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
       pvParameters value in the call to xTaskCreateStatic(). */
//    for (uint8_t i =0; i < 30; i++) {
//        i2c_write_blocking(&I2CPeriph, 0x29, testbuf[i], 3, 0);
//    }
//    i2c_write_blocking(&I2CPeriph, 0x29, resetreg, 2, 1);
    /*
     * @todo note that pins for sercom peripherals needs configuration first.
     */
    set_gpio_pin_mode(flash_miso, GPIO_MODE_C);
    set_gpio_pin_mode(flash_mosi, GPIO_MODE_D);
    set_gpio_pin_mode(flash_sck, GPIO_MODE_D);
    set_gpio_pin_mode(flash_ss, GPIO_MODE_OUTPUT);
    spi_dev_t flash_chip = {
            .spi_peripheral = &spi_periph,
            .character_size = SPI_8_BIT_CHARACTER_SIZE,
            .data_order = SPI_DATA_ORDER_MSB_FIRST,
            .clock_polarity = SPI_CLOCK_POLARITY_SCK_LOW_IDLE,
            .cs_pin = flash_ss
    };
    vTaskDelay(10/portTICK_RATE_MS);
//    for(uint8_t i=0; i< 30; i++) {
//        i2c_write_blocking(&I2CPeriph, 0x29, testbuf[i], 3, 1);
//        vTaskDelay(1/portTICK_RATE_MS);
 //       i2c_read_blocking(&I2CPeriph, 0x29, data, 1);
        //vTaskDelay(1/portTICK_RATE_MS);
    //}


    for( ;; )
    {
        spi_init(&flash_chip, 1000000);
        spi_start_transaction(&flash_chip);
        spi_write_blocking(&flash_chip, &data, 1);
        spi_end_transaction(&flash_chip);

//        i2c_write_blocking(&I2CPeriph, 0x29, StartMeasurement, 3, 1);
//        toggle_gpio_pin_output(LedPin);
//        vTaskDelay(10/portTICK_PERIOD_MS);
//        i2c_write_blocking(&I2CPeriph, 0x29, InterruptClear, 3, 1);
//        i2c_write_blocking(&I2CPeriph, 0x29, ReadRange, 2, 1);
//        i2c_read_blocking(&I2CPeriph, 0x29, &data, 1);
    }
}


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


int main(void)
{
    /*
     * Set the main clock to 48MHz
     */

    Clock_Init();
    /* Create the task without using any dynamic memory allocation. */
    set_gpio_pin_mode(LedPin, GPIO_MODE_OUTPUT);
    xTaskCreateStatic(
            vTaskCode,       /* Function that implements the task. */
            "CBTASK",          /* Text name for the task. */
            STACK_SIZE,      /* Number of indexes in the xStack array. */
            ( void * ) 1,    /* Parameter passed into the task. */
            tskIDLE_PRIORITY,/* Priority at which the task is created. */
            xStack,          /* Array to use as the task's stack. */
            &xTaskBuffer );  /* Variable to hold the task's data structure. */
    i2c_init(&I2CPeriph, I2C_CLOCK_SPEED);
    set_gpio_pin_mode(kBackBoneSCL, GPIO_MODE_D);
    set_gpio_pin_mode(kBackBoneSDA, GPIO_MODE_D);
    vTaskStartScheduler();
	while (1) {
	}
}


