/*
  Educational BoosterPack MK II
  http://boosterpackdepot.info/wiki/index.php?title=Educational_BoosterPack_MK_II 
  
  Microphone input, LED output, serial output 
  based on Analog input, analog output, serial output
  
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.
 
 The circuit:
 * Microphone (analog) attached to  pin 6.
   Neutral reading is at the center of the analog scale ~2048
   Higher sound input results in higher dynamic range at both
   ends of the analog scale [0,4096]
 * LED connected from pin 39 to ground
 
 created 29 Dec. 2008
 modified 30 Aug 2011
 by Tom Igoe
 modified 03 Dec 2013
 by Dung Dang for Educational BoosterPack MK II
 
 This example code is in the public domain.
 
 */

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = 6;  // Analog input pin that the potentiometer is attached to
const int digitalOutPin = 23; 
const int analogOutPin = 39;

int state = 0;
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int threshold = 2200;       // set this based on noise in the room

void setup() {
  // By default MSP432 has analogRead() set to 10 bits. 
  // This Sketch assumes 12 bits. Uncomment to line below to set analogRead()
  // to 12 bit resolution for MSP432.
  //analogReadResolution(12);

  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  pinMode(digitalOutPin, OUTPUT);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  
  outputValue = map(sensorValue, 0, 4096, 0, 255);  
  // change the analog out value:
  analogWrite(analogOutPin, outputValue); 
  
  
  Serial.println(sensorValue);
  if (sensorValue > threshold)
    {
      if(state == 0)
      {
        digitalWrite(digitalOutPin, HIGH);
        Serial.println("State High!");
        state = 1;
        delay(500); //"Debounce"
      }
      else
      {
        digitalWrite(digitalOutPin, LOW);
        Serial.println("State Low!");
        state = 0;   
        delay(500); //"Debounce"
      }
    }  
    delay(10);

                 
}
