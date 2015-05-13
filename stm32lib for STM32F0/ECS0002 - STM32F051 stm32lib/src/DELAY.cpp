/*
 * DELAY.cpp
 *
 *  Created on: 4-dec.-2014
 *      Author: Robbe
 */

#include "DELAY.h"

namespace stm32lib {

DELAY::DELAY(volatile uint32_t* storage): delayStorage(storage) {
	// TODO Auto-generated constructor stub

}

DELAY::~DELAY() {
	// TODO Auto-generated destructor stub
}

void DELAY::begin(delaySelection selection){
	switch (selection) {
		case DELAY_Milliseconds:
			SysTick_Config( (SystemCoreClock) / 1000);
			milliSeconds = true;
			break;
	}
	timingStorage = 0;
}

void DELAY::interruptHandler(void){
	if(*delayStorage != 0u){
		--*delayStorage;
	}
	timingStorage++;
}

void DELAY::startMilli(uint32_t newVal){
		*delayStorage = newVal;
}
void DELAY::stop(void){
	while(*delayStorage != 0u){
	}
}
void DELAY::milli(uint32_t newVal){
	startMilli(newVal);
	stop();
}
void DELAY::delayMicro(uint32_t newVal){
	waitDelayMicro(getMicros(), newVal);
}
void DELAY::delayMilli(uint32_t newVal){
	waitDelayMilli(getMillis(), newVal);
}

void DELAY::waitDelayMicro(uint32_t startTime, uint32_t delayVal){
	uint32_t difference;
	do{
		difference = getMicros()-startTime;
	}while( (difference) < delayVal);
}

void DELAY::waitDelayMilli(uint32_t startTime, uint32_t delayVal){
	uint32_t difference;
	do{
		difference = getMillis()-startTime;
	}while( (difference) < delayVal);
}

uint32_t DELAY::getMillis(void){
	return timingStorage;
}
uint32_t DELAY::getMicros(void){
	return timingStorage*1000 + (1000- (SysTick->VAL/48) );
}

void DELAY::setMillis(uint32_t newVal){
	timingStorage = newVal;
}


} /* namespace stm32lib */
