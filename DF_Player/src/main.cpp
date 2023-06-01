#include <Arduino.h>
//#include "wiring_private.h"
//#include "DFRobotDFPlayerMini.h"
#include "dfPlayer.hpp"

//DFRobotDFPlayerMini myDFPlayer;
dfPlayer &myDfPlayer = dfPlayer::dfPlayerGetInstance();

void setup() {
  Serial.begin(9600);
  //while(!Serial);

  if(myDfPlayer.begin(Serial1) == dfPlayer::dfPlayereDetails_t::SerialEstablished) 
    Serial.println("Serial is established");
  else 
    Serial.println("Serial is not established");

  myDfPlayer.dfPlayerStop();
  myDfPlayer.dfPlayerReset();
  myDfPlayer.dfPlayerSetSource(2);

  myDfPlayer.dfPlayerSetVolume(29);
  myDfPlayer.dfPlayerPlay(6);

}

void loop() {

}