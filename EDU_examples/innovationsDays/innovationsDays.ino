#include "SPI.h"
//#include "font.h"
#include "lcd8x8rgb.h"
#include <OneMsTaskTimer.h>

unsigned char h[] = "TI and TAMU working together for Innovation!";
boolean point=0;
//unsigned char badgeID[14];
String badgeID = "";


void setup()
{
  Serial.begin(115200); 
   //Set up SPI for RGB 8x8 LED
   SPI.setDataMode(SPI_MODE0);
   SPI.setClockDivider(SPI_CLOCK_DIV16);
   SPI.begin();
   pinMode(chipSelectPin, OUTPUT);
   digitalWrite(chipSelectPin, LOW);
}

void loop()
{ 
  scrollString(h, point);
}
