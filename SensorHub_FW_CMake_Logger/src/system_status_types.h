#ifndef SYSTEM_STATUS_TYPES_H
#define SYSTEM_STATUS_TYPES_H

#define GET_EV_BITS_FROM_EV_VAL(EV_VAL) (EV_VAL & 0xF)
#define GET_ID_BITS_FROM_ID_EV(EV_VAL) ((EV_VAL >> 4) & 0xFF)
#define GET_SR_PA_BITS_FROM_SR_EV(EV_VAL) ((EV_VAL >> 4) & 0xFF)
#define GET_SR_PB_BITS_FROM_SR_EV(EV_VAL) ((EV_VAL >> 12) & 0xFF)

#define SET_EV_WITH_DATA(EV, DATA) ((EV & 0xF) | ((DATA & 0xFFFFFF) << 4))

SemaphoreHandle_t USBProtoStatusMutex = NULL;
StaticSemaphore_t USBProtoStatusMutexBuf;

typedef enum { USB_STATUS_INACTIVE = 0, USB_STATUS_ACTIVE } usb_status_t;

typedef enum { MEMORY_SPACE_LEFT = 0, NO_MEMORY_SPACE_LEFT } memory_status_t;

typedef enum { UNKNOWN_TYPE = 0, SENSORHUB, ACTUATORHUB } hub_type_t;

typedef enum { PORT_UNITIALIZED = 0, PORT_INITIALIZED, DEVICE_ASSIGNED, DEVICE_INITIALIZED} port_status_t;

typedef enum { DEVICE_TYPE_NONE = 0, DEVICE_TYPE_UNKNOWN = 1, DEVICE_TYPE_TOF, DEVICE_TYPE_DP, DEVICE_TYPE_TOUCH, DEVICE_TYPE_GYRO, DEVICE_TYPE_PUMP = 0x21} device_type_t;

typedef enum { DRIVERS_UNINITIALIZED = 0, DRIVERS_INITIALIZED = 1, CAPTURING = 2} hub_status_t;

typedef struct {
  uint8_t hub_id;
  hub_type_t hub_type;
  hub_status_t hub_status;
  usb_status_t usb_status;
  memory_status_t memory_status;
  port_status_t port_status;
  device_type_t device_type_a;
  device_type_t device_type_b;
  uint16_t sample_time_a;
  uint16_t sample_time_b;
  uint16_t frame_num;
} system_status_t;


typedef enum {
  EV_ID_CHANGE = 0,
  EV_START = 1,
  EV_STOP = 2,
  EV_SAMPLE_RATE_CHANGE = 3,
  EV_SAMPLE_INCOMING = 4,
  EV_GET_SAMPLE_RANGE_FROM_BEGIN = 5,
  EV_GET_SAMPLE_RANGE_FROM_END = 6,
  EV_SENSOR_CHANGE = 7,
} system_events_t;


system_status_t status_shared_w_usb;


#endif /* SYSTEM_STATUS_TYPES_H */