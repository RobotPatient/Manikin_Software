#include "MB85RS2MTA.h"
#include "hal_spi_host.h"
#include "i2c_helper.hpp"
#include "sam.h"
#include "sensor_compression.hpp"
#include <Clock_stuff.h>
#include <FreeRTOS.h>
#include <board_definitions.h>
#include <hal_gpio.h>
#include <task.h>
#include "Slave_driver.hpp"


#define STACK_SIZE 200


/* Structure that will hold the TCB of the task being created. */
StaticTask_t xTaskBuffer;

/* Buffer that the task being created will use as its stack.  Note this is
an array of StackType_t variables.  The size of StackType_t is dependent on
the RTOS port. */
StackType_t xStack[ STACK_SIZE ];

I2CDriver sensor_port_a = I2CDriver(I2C_HOST_INST_PORT_A, kI2cSpeed_100KHz);
I2CDriver sensor_port_b = I2CDriver(I2C_HOST_INST_PORT_B, kI2cSpeed_100KHz);
I2CSlaveDriver backbone_port = I2CSlaveDriver(I2C_SLAVE_INST_BACKBONE_PORT);


CompressionSensor compressionSensor;
SensorData_t sensorData;

/* Function that implements the task being created. */
void vTaskCode( void * pvParameters )
{


    for( ;; )
    {
        sensorData = compressionSensor.GetSensorData();
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

int main(void)
{
    /*
     * Set the main clock to 48MHz
     */
    Clock_Init();
    init_pins();
    init_fram();

    sensor_port_a.Init();
    sensor_port_b.Init();
    backbone_port.init(0x10);

    compressionSensor.Initialize(&sensor_port_a);

    xTaskCreateStatic(
            vTaskCode,       /* Function that implements the task. */
            "CBTASK",          /* Text name for the task. */
            STACK_SIZE,      /* Number of indexes in the xStack array. */
            ( void * ) 1,    /* Parameter passed into the task. */
            tskIDLE_PRIORITY,/* Priority at which the task is created. */
            xStack,          /* Array to use as the task's stack. */
            &xTaskBuffer );  /* Variable to hold the task's data structure. */


    vTaskStartScheduler();
	while (1) {
	}
}


