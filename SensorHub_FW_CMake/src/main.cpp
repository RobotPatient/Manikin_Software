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

#define STACK_SIZE 200


/* Structure that will hold the TCB of the task being created. */
StaticTask_t xTaskBuffer;

/* Buffer that the task being created will use as its stack.  Note this is
an array of StackType_t variables.  The size of StackType_t is dependent on
the RTOS port. */
StackType_t xStack[ STACK_SIZE ];

I2CDriver SensorPortA = I2CDriver(I2C_HOST_INST, kI2cSpeed_100KHz);

CompressionSensor compressionSensor;
SensorData_t sensorData;

/* Function that implements the task being created. */
void vTaskCode( void * pvParameters )
{
    gpio_set_pin_mode(flash_miso, GPIO_MODE_C);
    gpio_set_pin_mode(flash_mosi, GPIO_MODE_D);
    gpio_set_pin_mode(flash_sck, GPIO_MODE_D);
    gpio_set_pin_mode(flash_ss, GPIO_MODE_OUTPUT);
    fram_dev_t fram = {
        .spi_bus = SPI_PERIPHERAL_2,
        .cs_pin = flash_ss
    };
    const spi_bus_opt_t extra_config_opt = static_cast<spi_bus_opt_t>(SPI_BUS_OPT_DOPO_PAD_0 | SPI_BUS_OPT_DIPO_PAD_2);

    SPI_HOST_INIT(SPI_PERIPHERAL_2, SPI_CLK_SOURCE_USE_DEFAULT, MAIN_CLOCK_SPEED, 10e6, extra_config_opt);
    vTaskDelay(1/portTICK_RATE_MS);

    fram_init(fram);

    GPIO_SET_PIN_MODE(kBackBoneSCL, GPIO_MODE_D);
    GPIO_SET_PIN_MODE(kBackBoneSDA, GPIO_MODE_D);
    SensorPortA.Init();

    compressionSensor.Initialize(&SensorPortA);

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
    /* Create the task without using any dynamic memory allocation. */
    gpio_set_pin_mode(LedPin, GPIO_MODE_OUTPUT);
    xTaskCreateStatic(
            vTaskCode,       /* Function that implements the task. */
            "CBTASK",          /* Text name for the task. */
            STACK_SIZE,      /* Number of indexes in the xStack array. */
            ( void * ) 1,    /* Parameter passed into the task. */
            tskIDLE_PRIORITY,/* Priority at which the task is created. */
            xStack,          /* Array to use as the task's stack. */
            &xTaskBuffer );  /* Variable to hold the task's data structure. */
//    I2C_HOST_INIT(I2C_HOST_INST, i2c_host_options);

    vTaskStartScheduler();
	while (1) {
	}
}


