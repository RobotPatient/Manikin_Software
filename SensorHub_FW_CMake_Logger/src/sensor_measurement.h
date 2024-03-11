#ifndef SENSOR_MEASUREMENT_H
#define SENSOR_MEASUREMENT_H
#include "hal_i2c_host.h"
#include "sensor_compression.hpp"
#include "sensor_ventilation.hpp"
#include "sensor_fingerposition.hpp"
#include "sensor_positioning.hpp"


CompressionSensor compressionSensor;
VentilationSensor ventilationSensor;
CompressionPositionSensor compressionPositionSensor;
PositioningSensor positioningSensor;
UniversalSensor* UniversalSensorPool[4] = {&compressionSensor, &ventilationSensor, &compressionPositionSensor, &positioningSensor};
StaticTimer_t SensTimerBuffers[2];

QueueHandle_t SensorValQueue;

/* The variable used to hold the queue's data structure. */
static StaticQueue_t StaticSensorQueue;

/* The array to use as the queue's storage area.  This must be at least
uxQueueLength * uxItemSize bytes. */
uint8_t ucQueueStorageArea[ 10*sizeof(SensorData_t) ];

void Sensx_timer( TimerHandle_t xTimer ) {
// UniversalSensor *Sensor = (UniversalSensor*) pvTimerGetTimerID(xTimer);
// SensorData_t Data = Sensor->GetSensorData();
// uint32_t res = xQueueSend(SensorValQueue, &Data, 1/portTICK_PERIOD_MS);
// }
}

void Sensx_hypervisor(void *pvArg) 
{
I2CDriver sensor_port_b = I2CDriver(I2C_HOST_INST_PORT_B, kI2cSpeed_100KHz);
I2CDriver sensor_port_a = I2CDriver(I2C_HOST_INST_PORT_A, kI2cSpeed_100KHz);

sensor_port_a.Init();
sensor_port_b.Init();

UniversalSensor* ConnectedSensor_PortA[MAX_SENSORS_PER_PORT] = {};
UniversalSensor* ConnectedSensor_PortB[MAX_SENSORS_PER_PORT] = {};

ConnectedSensor_PortA[0] = UniversalSensorPool[DEFAULT_SENSOR_TYPE_PORT_A];

ConnectedSensor_PortA[0]->Initialize(&sensor_port_a);

SensorValQueue = xQueueCreateStatic(10, sizeof(SensorData_t), ucQueueStorageArea, &StaticSensorQueue);


TimerHandle_t SensaTimer = xTimerCreateStatic("Sensa_timr", DEFAULT_SAMPLE_RATE/portTICK_PERIOD_MS, pdTRUE, (void*) ConnectedSensor_PortA, Sensx_timer, &SensTimerBuffers[0]);
configASSERT(SensaTimer);
TimerHandle_t SensbTimer = xTimerCreateStatic("Sensb_timr", DEFAULT_SAMPLE_RATE/portTICK_PERIOD_MS, pdTRUE, (void*) ConnectedSensor_PortB, Sensx_timer, &SensTimerBuffers[1]);
configASSERT(SensbTimer);
  BaseType_t value_received;
  uint32_t recv_data;
while(1) {
    value_received = xTaskNotifyWait(0, 0x00, &recv_data, portMAX_DELAY);
    if(value_received == pdPASS) {
        const uint32_t event_type = GET_EV_BITS_FROM_EV_VAL(recv_data);
        switch(event_type) {
            case EV_START:
            {
                xTimerStart(SensaTimer, 0);
                xTimerStart(SensbTimer, 0);
                break;
            }
            case EV_STOP:
            {
                xTimerStop(SensaTimer, 0);
                xTimerStop(SensbTimer, 0);
                break;
            }
        }
    }
    vTaskDelay(10/portTICK_PERIOD_MS);
}
}

#endif  /* SENSOR_MEASUREMENT_H */