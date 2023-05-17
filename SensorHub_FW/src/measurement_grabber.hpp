#ifndef MEASUREMENT_GRABBER_HPP
#define MEASUREMENT_GRABBER_HPP
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <sensor_helper.hpp>

inline constexpr uint8_t kSensorPollingTaskStackSize =  150;

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
    if (Data->Sensor != NULL) {
      data = Data->Sensor->GetSensorData();
      if (xQueueSend(*(Data->queue), (void*)&data, (TickType_t)10) != pdPASS) {
        /* Failed to post the message, even after 10 ticks. */
      }
      vTaskDelay(Data->SampleTime / portTICK_PERIOD_MS);
    }
  }
}

/**
 * @brief MeasurementGrabberTask gives uniform interface for the sensor..
 *        This way parameters like sampletime, measurementqueue and sensortype can be set.
*/
class MeasurementGrabber {
 public:
 /**
  * @brief Initialize the polling task for reading the sensor with set sample time..
 */
  void SetupPollTask(UniversalSensor* Sensor, uint8_t SampleTime,
                     xQueueHandle* queue, xTaskHandle* Task) {
    Data_ = {Sensor, SampleTime, queue};
    SampleTime_ = SampleTime;
    *Task = xTaskCreateStatic(
        MeasurementGrabberTask,     /* Function that implements the task. */
        "Measurement grabber task", /* Text name for the task. */
        kSensorPollingTaskStackSize, /* Number of indexes in the xStack array. */
        (void*)&Data_,              /* Parameter passed into the task. */
        (2 | portPRIVILEGE_BIT),    /* Priority at which the task is created. */
        xStack,                     /* Array to use as the task's stack. */
        &xTaskBuffer); /* Variable to hold the task's data structure. */
    Task_ = Task;
  }
  /**
   * @brief Suspend the created polling task
  */
  void SuspendPollingTask() { vTaskSuspend(*Task_); }

  /**
   * @brief Resume the created polling task
  */
  void ResumePollingTask() { vTaskResume(*Task_); }

  /**
   * @brief Change the current sensor to a new given one
  */
  void ChangeSensor(UniversalSensor* Sensor) {
    SuspendPollingTask();
    Sensor->Uninitialize();
    Data_.Sensor = Sensor;
  }

  /**
   * @brief Get the currently set sampletime
  */
  uint8_t GetSampleTime() { return SampleTime_; }

  /**
   * @brief Set a new sampletime
  */
  void SetSampleTime(const uint8_t SampleTime) {
    SampleTime_ = SampleTime;
    SuspendPollingTask();
    Data_.SampleTime = SampleTime;
    ResumePollingTask();
  }

 private:
  /**
   * @note Keep local copy of SampleTime_ that isn't accessed by the task
  */
  uint8_t SampleTime_;
  MeasurementGrabberData Data_;
  xTaskHandle* Task_;
  StaticTask_t xTaskBuffer;
  StackType_t xStack[kSensorPollingTaskStackSize];
};

#endif