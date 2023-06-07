#include <Arduino.h>
//#include "wiring_private.h"
//#include "DFRobotDFPlayerMini.h"
#include "dfPlayer.hpp"

//DFRobotDFPlayerMini myDFPlayer;
dfPlayer &myDfPlayer = dfPlayer::dfPlayerGetInstance();

void setup() {
  Serial.begin(9600);
  //while(!Serial);

  if(myDfPlayer.begin(Serial1) == dfPlayer::dfPlayereDetails_t::DF_PlayerIsAvailable) 
    Serial.println("dfPlayer is available");
  else 
    Serial.println("Serial error");

  myDfPlayer.dfPlayerSetVolume(4);

  

//  myDfPlayer.dfPlayerPlay(2);
  //delay(6000);
  //myDfPlayer.dfPlayerPlay(3);
  //delay(6000);
  //myDfPlayer.dfPlayerPlay(4);
 // myDfPlayer.dfPlayerSetVolume(0);

}

void loop() {

}