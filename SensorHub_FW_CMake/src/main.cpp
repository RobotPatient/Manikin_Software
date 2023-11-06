#include "MB85RS2MTA.h"
#include "hal_spi_host.h"
#include "i2c_helper.hpp"
#include "sam.h"
#include <Clock_stuff.h>
#include <FreeRTOS.h>
#include <board_definitions.h>
#include <hal_gpio.h>
#include <task.h>
#include "Slave_driver.hpp"
#include "evsys_imp.hpp"

#include "sensor_compression.hpp"
#include "sensor_ventilation.hpp"
#include "sensor_fingerposition.hpp"

#define STACK_SIZE 200

/* Note: manual see SensorHub_communication_protocol.pdf */

/* Buffer structure that will hold the system information and/or sensor data. */
i2c_slave_reg_t public_reg;

/* Structure that will hold the TCB of the task being created. */
StaticTask_t xTaskBuffer;

/* Buffer that the task being created will use as its stack.  Note this is
an array of StackType_t variables.  The size of StackType_t is dependent on
the RTOS port. */
StackType_t xStack[ STACK_SIZE ];

I2CDriver sensor_port_b = I2CDriver(I2C_HOST_INST_PORT_A, kI2cSpeed_100KHz);
I2CDriver sensor_port_a = I2CDriver(I2C_HOST_INST_PORT_B, kI2cSpeed_100KHz);
I2CSlaveDriver backbone_port = I2CSlaveDriver(I2C_SLAVE_INST_BACKBONE_PORT);

CompressionSensor compressionSensor;
VentilationSensor ventilationSensor;
CompressionPositionSensor compressionPositionSensor;

SensorData_t sensorData;

/* Possible sensors */
UniversalSensor* universalSensorPool[3] = { &compressionSensor, &ventilationSensor, &compressionPositionSensor };
// ToDo JK: Implement additional sensors.

/* Actual sensors */
UniversalSensor* ConnectedSensor_PortA[2] = {};
UniversalSensor* ConnectedSensor_PortB[2] = {};

/* Function that implements the task being created. */
void vTaskCode( void * pvParameters )
{
    for( ;; )
    {
        if (compressionSensor.Available()) {
          sensorData = compressionSensor.GetSensorData();
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

/**
 * @brief Method to initialize the sensors that are connected to the SensorHub.
 *
 * More detailed description of the function.
 *
 * @param none.
 * @return void
 */



/**
 * @brief Method to initialize the sensors that are connected to the SensorHub.
 *
 * More detailed description of the function.
 *
 * @param none.
 * @return void
 */

/*
void initSensors() {
    // updates the registers
    // ToDo alle sensoren aflopen
    // Proberen of sensor init goed gaat
    // Dit heeft op low level wel een
    // 
}*/

/** Same documentation for both members. Details can be added here. */

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

    compressionSensor.Initialize(&sensor_port_b);
    backbone_port.set_external_register_buffer(&public_reg);

    setup_evsys_handler();

    backbone_port.force_update_internal_buffer(public_reg.STATUS, 2);

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

