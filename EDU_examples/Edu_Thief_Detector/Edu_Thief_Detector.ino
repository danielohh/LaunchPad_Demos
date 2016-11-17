#include "SPI.h"
#include "Screen_HX8353E.h"


// The number of times to trigger the action if the condition is met
// We limit this so you won't use all of your Temboo calls while testing
int maxCalls = 10;

// The number of times this Choreo has been run so far in this sketch
int calls = 0;


Screen_HX8353E myScreen;

#define ACC_X 23
#define ACC_Y 24
#define ACC_Z 25
#define BUZZ 40
#define REDLED 39

int ACC_X_READ = 0;
int ACC_Y_READ = 0;
int ACC_Z_READ = 0;


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  Serial.println("Magic 8 Ball!");
  Serial.println("Shake to Begin!");
  
  
  
  myScreen.begin();
  myScreen.clear();
  myScreen.setFontSize(0);
  myScreen.gText(4, 4, "Magic 8 Ball", whiteColour);
  myScreen.gText(4, 14, "Shake to Begin", whiteColour);
  pinMode(BUZZ, OUTPUT);
  delay(1000);
}

void loop()
{
  // put your main code here, to run repeatedly:
  ACC_X_READ = (analogRead(ACC_X)-512)*100/194;
  ACC_Y_READ = (analogRead(ACC_Y)-512)*100/194;
  ACC_Z_READ = (analogRead(ACC_Z)-512)*100/194;
  int TOTAL_ACC = ACC_X_READ*ACC_X_READ + ACC_Y_READ*ACC_Y_READ + ACC_Z_READ*ACC_Z_READ;
  TOTAL_ACC = sqrt(TOTAL_ACC);
  
  myScreen.gText(4, 4, "Magic 8 Ball", whiteColour);
  myScreen.gText(4, 14, "Shake to Begin", whiteColour);
  
  //Serial.print(ACC_X_READ); Serial.print("\t");
  //Serial.print(ACC_Y_READ); Serial.print("\t");
  //Serial.print(ACC_Z_READ); Serial.print("\t");
  Serial.print("ACC: ");
  Serial.print(TOTAL_ACC);Serial.println();  
  
  if((TOTAL_ACC > 150) && (TOTAL_ACC!=452)){
    myScreen.clear();
    myScreen.gText(4, 4, "Magic 8 Ball", whiteColour);
  
    int value = random(0,4);
    Serial.println(value);
    
    switch(value){
    case 1:
      myScreen.gText(14, 24, "Yes!", greenColour);
      break;
    case 2:
      myScreen.gText(14, 24, "No!", redColour);
      break;
    case 3:
      myScreen.gText(14, 24, "Result hazy", yellowColour);
      myScreen.gText(14, 34, "Try again", yellowColour);
      break;
    case 4:
      myScreen.gText(14, 24, "Unlikely!", redColour);
      break;
    default:
      myScreen.gText(14, 24, "Probably!", greenColour);
      break;
    }
    delay(1000);
    

  }
  delay(100);
}


