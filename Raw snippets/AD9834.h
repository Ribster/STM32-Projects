/*
	AD9834.cpp - AD9834 Direct Digital Synthesis IC Library
  
	Copyright (C) 2012 Mark Jessop <mark.jessop@adelaide.edu.au>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a full copy of the GNU General Public License, 
    see <http://www.gnu.org/licenses/>.
*/

#ifndef AD9834_h
#define AD9834_h

#include "Arduino.h" 

// Pin Definitions - Change these as needed.
#define RESET_PIN   9
#define SLEEP_PIN   8



// Function Definitions
void AD9834_SendWord(unsigned int data);
void AD9834_Use_Pins(int value);
void AD9834_Sign_Bit_On(int value);
void AD9834_DAC_ON(int value);
void AD9834_SelectFREG(int value);
void AD9834_SelectPREG(int value);
void AD9834_Reset(int value);
void AD9834_Sleep(int value);
void AD9834_Triangle_Output(int value);
unsigned long AD9834_SetFreq(int f_reg, unsigned long freq);
unsigned long AD9834_SetFreq(int f_reg, unsigned long freq, unsigned long offset);
void AD9834_Setup(int fsync, int psel, int fsel);
void AD9834_SINE_ON();
void AD9834_SetPREG(unsigned int preg, unsigned int phase);



#endif
