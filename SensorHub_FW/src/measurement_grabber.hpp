#ifndef MEASUREMENT_GRABBER_HPP
#define MEASUREMENT_GRABBER_HPP
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <sensor_helper.hpp>

#define STACK_SIZE 250

/**
 * @brief Struct type used internally to pass arguments 
 *        between class and MeasurementGrabberTask
*/
typedef struct {
  UniversalSensor* Sensor;
  uint8_t SampleTime;
  xQueueHandle* queue;
} MeasurementGrabberData;

/**
 * @brief MeasurementGrabberTask which reads the sensor measurements
 *        and sends the results to the queue assigned in the MeasurementGrabberData 
 *        in the arguments (void *PvParameter).
*/
void MeasurementGrabberTask(void* PvParameter) {
  MeasurementGrabberData* Data = (MeasurementGrabberData*)PvParameter;
  SensorData data;
  while (1) {
    data = Data->Sensor->GetSensorData();
    if (xQueueSend(*(Data->queue), (void*)&data, (TickType_t)10) != pdPASS) {
      /* Failed to post the message, even after 10 ticks. */
    }
    vTaskDelay(Data->SampleTime / portTICK_PERIOD_MS);
  }
}

/**
 * @brief MeasurementGrabberTask gives uniform interface for the sensor..
 *        This way parameters like sampletime, measurementqueue and sensortype can be set.
*/
class MeasurementGrabber {
 public:
  void SetupPollTask(UniversalSensor* Sensor, uint8_t SampleTime,
                     xQueueHandle* queue, xTaskHandle* Task) {
    Data_ = {Sensor, SampleTime, queue};
    SampleTime_ = SampleTime;
    *Task = xTaskCreateStatic(
        MeasurementGrabberTask,     /* Function that implements the task. */
        "Measurement grabber task", /* Text name for the task. */
        STACK_SIZE,                 /* Number of indexes in the xStack array. */
        (void*)&Data_,              /* Parameter passed into the task. */
        (2 | portPRIVILEGE_BIT),    /* Priority at which the task is created. */
        xStack,                     /* Array to use as the task's stack. */
        &xTaskBuffer); /* Variable to hold the task's data structure. */
    Task_ = Task;
  }

  void SuspendPollingTask() { vTaskSuspend(*Task_); }

  void ResumePollingTask() { vTaskResume(*Task_); }

  void ChangeSensor(UniversalSensor* Sensor) {
    SuspendPollingTask();
    Sensor->Uninitialize();
    Data_.Sensor = Sensor;
  }

  uint8_t GetSampleTime() { return SampleTime_; }

  void SetSampleTime(const uint8_t SampleTime) {
    SampleTime_ = SampleTime;
    SuspendPollingTask();
    Data_.SampleTime = SampleTime;
    ResumePollingTask();
  }

 private:
  uint8_t SampleTime_;
  MeasurementGrabberData Data_;
  xTaskHandle* Task_;
  /* Structure that will hold the TCB of the task being created. */
  StaticTask_t xTaskBuffer;
  /* Buffer that the task being created will use as its stack.  Note this is
    an array of StackType_t variables.  The size of StackType_t is dependent on
    the RTOS port. */
  StackType_t xStack[STACK_SIZE];
};

#endif