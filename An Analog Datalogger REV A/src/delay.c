/*
 * delay.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "delay.h"

// implementation
void
delay_milli(volatile uint32_t waitVal){
	uint32_t tmp = delay_microBuffer;
	if(delay_microBuffer>waitVal){
		tmp -= waitVal;
		while(delay_microBuffer>tmp);
	} else {
		delay_microBuffer = waitVal;
		while(delay_microBuffer!=0x00);
	}
}

void
delay_setup(void){
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY_HZ);
}
void
delay_interruptHandler(void){
	delay_totalBuffer++;
	if(delay_microBuffer != 0x00){
		delay_microBuffer--;
	}
	// set time on OLED screen
	if(delay_totalBuffer%100 == 0){
		rtc_setTimeToOLED();
		leds_toggleLed(ledList_Green);
	}
}
uint32_t
delay_getMillis(void){
	return delay_totalBuffer;
}
uint32_t
delay_getMilliDifference(uint32_t t0, uint32_t t1){
	return t1-t0;
}
uint32_t
delay_getMilliDifferenceSimple(uint32_t t0){
	return delay_getMilliDifference(t0, delay_totalBuffer);
}
