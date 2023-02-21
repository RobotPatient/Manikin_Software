#ifdef __arm__
#include "wiring_private.h"
#include "Wire.h"

// i2c system bus
#define W0_SCL 27 // PA22 
#define W0_SDA 26 // PA23 

#define W1_SCL 39 // PA13  
#define W1_SDA 28 // PA12  

#define W2_SCL 13 // PA17 
#define W2_SDA 11 // PA16

TwoWire WireBackbone(&sercom3, W0_SDA, W0_SCL); // Main
TwoWire WireSensorA(&sercom1, W1_SDA, W1_SCL);  // Sensor A
TwoWire WireSensorB(&sercom4, W2_SDA, W2_SCL);  // Sensor B


void InitI2CPins(){
  pinPeripheral(W0_SDA, PIO_SERCOM);
  pinPeripheral(W0_SCL, PIO_SERCOM);

  pinPeripheral(W2_SDA, PIO_SERCOM);
  pinPeripheral(W2_SCL, PIO_SERCOM);
  
  pinPeripheral(W1_SDA, PIO_SERCOM_ALT);
  pinPeripheral(W1_SCL, PIO_SERCOM_ALT);
}
#endif