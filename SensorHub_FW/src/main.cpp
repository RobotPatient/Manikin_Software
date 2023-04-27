#ifdef __arm__
#include <Arduino.h>

#include <sensorhub_settings.hpp>
#include <sensor_helper.hpp>
#include <hal_exception.hpp>

using namespace hal::exception;
using namespace hal::log;
/* Create handle for sensor connector PORT_A and PORT_B.
 * These will be used in setup() and loop()*/
UniversalSensor *connector_port_a;
UniversalSensor *connector_port_b;

void printResults(String Prefix, SensorData data){
  for(int i =0; i< data.num_of_bytes/2; i++) {
    Serial.print(Prefix+"R:");
    Serial.print(data.buffer[i]);
  }
  if(data.num_of_bytes == 1){
    Serial.print(data.buffer[0]);
  }
  Serial.print("\n");

}

static Logger* log_inst;
static LoggerSettings Log_module_settings;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial)
    continue;
  // wireBackbone.begin();
  // InitI2CPins();
  // i2c_handle_port_a.Init();
  // i2c_handle_port_b.Init();
  // connector_port_a = new FingerPositionSensor(&i2c_handle_port_a);
  // connector_port_a->Initialize();
  // connector_port_b = new DifferentialPressureSensor(&i2c_handle_port_b);
  // connector_port_b->Initialize();
  hal::exception::Init();
  InitSerialLogger(log_inst, &Log_module_settings, &Serial);
  hal::exception::attachLogger(log_inst);
}

void loop() {
  // put your main code here, to run repeatedly:
  // SensorData ResultSensA = connector_port_a->GetSensorData();
  // printResults("A_",ResultSensA);
  // SensorData ResultSensB = connector_port_b->GetSensorData();
  // printResults("B_",ResultSensB);
  // delay(100);;
  THROW_RESET("TEST!", UNINITIALIZED);
  //const uint8_t i = 0;
  //ASSERT_WARN(i != 0);
  delay(1000);
}

#else
int main() {
  while (1)
    continue;
}
#endif
