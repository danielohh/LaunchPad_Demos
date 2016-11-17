#include "ULN2003BOOST.h"                      // Attach needed library for board functions
#include <BLE.h>

int period = 5;                              // Time defined in milliseconds
ULN2003BOOST myBoard = ULN2003BOOST(period);   // Create instance of ULN2003BOOST board
String bleString = "state";

void setup(){
  Serial.begin(115200);// Set pins and clear outputs
  ble.setLogLevel(BLE_LOG_ALL);
  ble.begin();
  ble.serial();
  ble.setAdvertName("Energia Serial");
  ble.startAdvert();
}

void loop(){
  if (ble.available())
  {
    bleString = ble.readString();
    Serial.println(ble.readString());
    // Turn on the motor. Note: conflicts with BLE?
    myBoard.start(); 
    int i = 300;
    if (bleString == "boo!")
    {
      // Drop the spider!
      while(i>0)
      {
        myBoard.cwStepFull();
        i--;
      }
    }
    if (bleString == "up")
    {
      while(i>0)
      {
        myBoard.ccwStepFull();
        i--;
      }
    }
  }
}



