#include <Arduino.h>
//#include "wiring_private.h"
//#include "DFRobotDFPlayerMini.h"
#include "dfPlayer.hpp"

//DFRobotDFPlayerMini myDFPlayer;
dfPlayer &myDfPlayer = dfPlayer::dfPlayerGetInstance();

void setup() {
  Serial.begin(9600);
  //while(!Serial);

  if(myDfPlayer.begin(Serial1)) 
    Serial.println("dfPlayer is available");
  else 
    Serial.println("Serial error");

  myDfPlayer.dfPlayerSetVolume(10);
  myDfPlayer.dfPlayerPlay(2);
  delay(6000);
  myDfPlayer.dfPlayerPlay(3);
  delay(6000);
  myDfPlayer.dfPlayerPlay(4);
  delay(6000);
  myDfPlayer.dfPlayerPause();
  delay(6000);
  myDfPlayer.dfPlayerSetVolume(23);
  myDfPlayer.dfPlayerResume();
  delay(6000);
  myDfPlayer.dfPlayerSetVolume(14);
  myDfPlayer.dfPlayerPlayNext();
  delay(6000);
  myDfPlayer.dfPlayerPlayPrevious();
  


}

void loop() {

}