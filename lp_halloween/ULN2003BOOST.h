//ULN2003BOOST.h
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

extern char halfStep[8];
extern char fullStep[4];
extern char waveDrive[4];

class ULN2003BOOST {
  public: 
  ULN2003BOOST(int Period); //Initiate board with Pin assignments for serial data mode
  void start(); //Initiate board, clear and blink LEDs to signify ready to transmit
  void sendByte(char Output); //serialize out byte information entered by user or readout
  void clearOutput(); //set all outputs to low or '0'
  void cwStepHalf(); //completes a Half-Step sequence clockwise
  void cwStepFull(); //completes a Full-Step sequence clockwise
  void cwStepWave(); //completes a Wave-Drive sequence clockwise
  void ccwStepHalf(); //completes a Half-Step sequence counter-clockwise
  void ccwStepFull(); //completes a Full-Step sequence counter-clockwise
  void ccwStepWave(); //completes a Half-Step sequence counter-clockwise
};

