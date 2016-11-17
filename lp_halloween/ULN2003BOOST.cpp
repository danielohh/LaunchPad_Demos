//ULN2003BOOST.cpp
/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * This product is designed as an aid for customers of Texas Instruments.  No warranties, either express
 * or implied, with respect to this software or its fitness for any particular purpose is claimed by Texas
 * Instruments or the author.  The software is licensed solely on an "as is" basis.  The entire risk as to its
 * quality and performance is with the customer.
 * --/COPYRIGHT--*/
//******************************************************************************
#include "ULN2003BOOST.h"
#include "Energia.h"

int _Period;
int SRCLK = 13; //SRCLK for SN74HC595 is set as GPIO 13 in the Lauchpad standard
int RCLK = 12;  //RCLK for SN74HC595 is set as GPIO 12 in the Lauchpad standard
int SER = 11;   //SER for SN74HC595 is set as GPIO 111 in the Lauchpad standard
char halfStep[8] = {B10001000,B11001100,B01000100,B01100110,B00100100,B00110011,B00010001,B10011001}; //Defines the step waveform for half-Step
char fullStep[4] = {B11001100, B01100110, B00110011, B10011001};                                      //Defines the step waveform for full-Step
char waveDrive[4] = {B10001000, B01000100, B00100010, B00010001};                                     //Defines the step waveform for wave-drive

ULN2003BOOST::ULN2003BOOST(int Period) {
  //Assign delay for communication through the on board SN74HC595
  pinMode(SRCLK, OUTPUT);      
  pinMode(RCLK, OUTPUT);      
  pinMode(SER, OUTPUT);      
  pinMode(8, INPUT);         //used to fix any issues with GPIO8 *to be removed*
  digitalWrite (SRCLK, LOW); //writting low just in case
  digitalWrite (RCLK, LOW);
  digitalWrite (SER, LOW);
  _Period = Period;
}

void ULN2003BOOST::start(){
  //Blink and clear outputs for board
  clearOutput();
}

void ULN2003BOOST::sendByte(char Output){
  //Send user defined byte output
  digitalWrite(RCLK, LOW);
  digitalWrite(SER, LOW);
  shiftOut(SER, SRCLK, MSBFIRST, Output);
  digitalWrite (RCLK, HIGH);
  delay(_Period);
}

void ULN2003BOOST::clearOutput(){
  //Sets outputs to low or '0'
  sendByte(0x00);
}

void ULN2003BOOST::cwStepHalf(){
  //Step clockwise using a full Half-step sequence
  for(int i = 0; i < 8; i++){ //move accross array
    sendByte(halfStep[i]);
  }
}

void ULN2003BOOST::ccwStepHalf(){
  //Step counter-clockwise using a full Half-step sequence
  for(int i = 7; i >= 0; i--){ //move accross array
    sendByte(halfStep[i]);
  }
}

void ULN2003BOOST::cwStepFull(){
  //Step clockwise using a full Full-step sequence
  for(int i = 0; i < 4; i++){ //move accross array
    sendByte(fullStep[i]);
  }
}

void ULN2003BOOST::ccwStepFull(){
  //Step counter-clockwise using a full Full-step sequence
  for(int i = 3; i >= 0; i--){ //move accross array
    sendByte(fullStep[i]);
  }
}

void ULN2003BOOST::cwStepWave(){
  //Step clockwise using a full wave-drive sequence
  for(int i = 0; i < 4; i++){ //move accross array
    sendByte(waveDrive[i]);
  }
}

void ULN2003BOOST::ccwStepWave(){
  //Step counter-clockwise using a full wave-drive sequence
  for(int i = 3; i >= 0; i--){ //move accross array
    sendByte(waveDrive[i]);
  }
}


