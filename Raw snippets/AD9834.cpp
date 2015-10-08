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

#if ARDUINO >= 100
    #include "Arduino.h"   
#else
extern "C" {
    #include "WConstants.h"
}
#endif
#include "AD9834.h"
#include <SPI.h>

unsigned int AD9834_CONTROL = 0x0000;

int FSYNC_PIN = 11;
int PSEL_PIN = 3;
int FSEL_PIN = 2;

int PINSW = 0;
int FREG = 0;

void AD9834_Use_Pins(int value){
    if(value){
        AD9834_CONTROL |= 0x0100;
        PINSW = 1;
    }else{
        AD9834_CONTROL &= ~0x0100;
        PINSW = 0;
    }
    AD9834_SendWord(AD9834_CONTROL);
}

void AD9834_Sign_Bit_On(int value){
    if(value){
        AD9834_CONTROL |= 0x0028;
    }else{
        AD9834_CONTROL &= ~0x0028;
    }
    AD9834_SendWord(AD9834_CONTROL);
}

void AD9834_DAC_ON(int value){
    if(value){
        AD9834_CONTROL &= ~0x0040; // Clear the SLEEP12 bit.
    }else{
        AD9834_CONTROL |= 0x0040; // Set the SLEEP12 bit.
    }
    AD9834_SendWord(AD9834_CONTROL);
}

void AD9834_SelectFREG(int value){
    if(PINSW){
        if(value)   digitalWrite(FSEL_PIN,HIGH);
        else        digitalWrite(FSEL_PIN,LOW);
    }else{
        if(value)   AD9834_CONTROL |= 0x0800;
        else        AD9834_CONTROL &= ~0x0800;
        AD9834_SendWord(AD9834_CONTROL);
    }
}

void AD9834_SelectPREG(int value){
    if(PINSW){
        if(value)   digitalWrite(PSEL_PIN,HIGH);
        else        digitalWrite(PSEL_PIN,LOW);
    }else{
        if(value)   AD9834_CONTROL |= 0x0400;
        else        AD9834_CONTROL &= ~0x0400;
        AD9834_SendWord(AD9834_CONTROL);
    }
}

void AD9834_Reset(int value){
    if(PINSW){
        if(value)   digitalWrite(RESET_PIN,HIGH);
        else        digitalWrite(RESET_PIN,LOW);
    }else{
        if(value)   AD9834_CONTROL |= 0x0100;
        else        AD9834_CONTROL &= ~0x0100;
        AD9834_SendWord(AD9834_CONTROL);
    }
}

void AD9834_Sleep(int value){
    if(PINSW){
        if(value)   digitalWrite(SLEEP_PIN,HIGH);
        else        digitalWrite(SLEEP_PIN,LOW);
    }else{
        if(value)   AD9834_CONTROL |= 0x0080;
        else        AD9834_CONTROL &= ~0x0080;
        AD9834_SendWord(AD9834_CONTROL);
    }
}

void AD9834_Triangle_Output(int value){
    if(value)   AD9834_CONTROL |= 0x0002;
    else        AD9834_CONTROL &= ~0x0002;
    AD9834_SendWord(AD9834_CONTROL);
}

// Sets FREG0 to the value required to produce a specified frequency, in Hz.
unsigned long AD9834_SetFreq(int f_reg, unsigned long freq){
	unsigned int data;
	unsigned long temp;
	unsigned int f_LSB;
	unsigned int f_MSB;
	temp=((unsigned long)((float)freq*5.36871))&0x0FFFFFFF;
	if(f_reg==1){
	    f_LSB = (0x8000 | (unsigned int)(temp & 0x00003FFF));
		f_MSB = (0x8000 | (unsigned int)((temp>>14) & 0x3FFF));
	}else{
		f_LSB = (0x4000 | (unsigned int)(temp & 0x00003FFF));
		f_MSB = (0x4000 | ((unsigned int)(temp>>14) & 0x3FFF));
	}
    AD9834_SendWord(AD9834_CONTROL|0x2000);
    AD9834_SendWord(f_LSB);
    AD9834_SendWord(f_MSB);
    AD9834_SendWord(AD9834_CONTROL);
    return temp;
}

// Sets FREG0 to the value required to produce a specified frequency, in Hz.
// Also set an offset in units of 0.18626448439197 Hz
unsigned long AD9834_SetFreq(int f_reg, unsigned long freq, unsigned long offset){
	unsigned int data;
	unsigned long temp;
	unsigned int f_LSB;
	unsigned int f_MSB;
	temp=((unsigned long)((float)freq*5.36871))&0x0FFFFFFF;
	temp = temp + offset;
	if(f_reg==1){
	    f_LSB = (0x8000 | (unsigned int)(temp & 0x00003FFF));
		f_MSB = (0x8000 | (unsigned int)((temp>>14) & 0x3FFF));
	}else{
		f_LSB = (0x4000 | (unsigned int)(temp & 0x00003FFF));
		f_MSB = (0x4000 | ((unsigned int)(temp>>14) & 0x3FFF));
	}
    AD9834_SendWord(AD9834_CONTROL|0x2000);
    AD9834_SendWord(f_LSB);
    AD9834_SendWord(f_MSB);
    AD9834_SendWord(AD9834_CONTROL);
    return temp;
}

// Initialises the AD9834, clearing all the registers, and putting it into sleep mode.
void AD9834_Setup(int fsync, int psel, int fsel){
	FSYNC_PIN = fsync;
	PSEL_PIN = psel;
	FSEL_PIN = fsel;
	pinMode(FSYNC_PIN, OUTPUT);
	pinMode(PSEL_PIN, OUTPUT);
	pinMode(FSEL_PIN, OUTPUT);
	
	digitalWrite(FSYNC_PIN,HIGH); // Set FSYNC high - this goes low when writing to the registers.
	digitalWrite(PSEL_PIN,LOW);
	digitalWrite(FSEL_PIN,LOW); // Use FREG0 by default.
	
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE2);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	
	// Set everything to off, basically.
	AD9834_SendWord(AD9834_CONTROL);
	
	// Null the phase Registers.
	AD9834_SendWord(0xC000); // Phase register 0
	AD9834_SendWord(0xE000); // Phase register 1

}

void AD9834_SINE_ON(){
    AD9834_Sleep(0);
    AD9834_DAC_ON(1);
    AD9834_Reset(0);
}
    

// Send a 16-bit data word to the AD9834, MSB first.
// WARNING: I am using direct register writes in this function, and hence
// the FSEL pin is hardcoded to Port B pin 7. 
// To use this on any other PCB, this function will have to be modified.
void AD9834_SendWord(unsigned int data){
	PORTB &= ~_BV(7);	
	SPI.transfer((data>>8)&0xFF);
	SPI.transfer(data&0xFF);
	PORTB |= _BV(7);
}

// Set the phase of a register, in degrees (0-359). 
void AD9834_SetPREG(unsigned int preg, unsigned int phase){
	// check for an invalid phase.
	if(phase<0 || phase>359){
		return;
	}
	
	unsigned int phaseval = 0x0FFF*((float)phase/360.0);
	
	if(preg == 0){
		AD9834_SendWord((0x0FFF & phaseval) + 0xC000);
	}else{
		AD9834_SendWord((0x0FFF & phaseval) + 0xE000);
	}
}




