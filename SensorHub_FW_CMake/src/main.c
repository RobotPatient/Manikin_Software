#include "sam.h"
#include <hal_i2c.h>
#include <hal_gpio.h>
#include <Clock_stuff.h>
#include <i2c_register_stuff.h>
#include <board_definitions.h>
#include <FreeRTOS.h>
#include <task.h>
#define STACK_SIZE 200

/* Structure that will hold the TCB of the task being created. */
StaticTask_t xTaskBuffer;

/* Buffer that the task being created will use as its stack.  Note this is
an array of StackType_t variables.  The size of StackType_t is dependent on
the RTOS port. */
StackType_t xStack[ STACK_SIZE ];


/* Function that implements the task being created. */
void vTaskCode( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
       pvParameters value in the call to xTaskCreateStatic(). */
    for( ;; )
    {
        ToggleGPIOPin(LedPin);
        vTaskDelay(50/portTICK_PERIOD_MS);
    }
}


void i2c_slave_data_send_irq(const void *const hw){
    ((Sercom*)hw)->I2CS.DATA.reg = 0;

}

void i2c_slave_data_recv_irq(const void *const hw){
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

void i2c_slave_stop_irq(const void *const hw) {
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
    SetGPIOPinDirection(LedPin, kGPIODirOutput);
    xTaskCreateStatic(
            vTaskCode,       /* Function that implements the task. */
            "CBTASK",          /* Text name for the task. */
            STACK_SIZE,      /* Number of indexes in the xStack array. */
            ( void * ) 1,    /* Parameter passed into the task. */
            tskIDLE_PRIORITY,/* Priority at which the task is created. */
            xStack,          /* Array to use as the task's stack. */
            &xTaskBuffer );  /* Variable to hold the task's data structure. */
    //i2c_init(&I2CPeriph, I2C_CLOCK_SPEED);
    SetGPIOPinFunction(kBackBoneSCL, kGPIOFunctionD);
    SetGPIOPinFunction(kBackBoneSDA, kGPIOFunctionD);
    vTaskStartScheduler();
	while (1) {
	}
}


