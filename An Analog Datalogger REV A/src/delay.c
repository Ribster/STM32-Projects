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
	if(delay_microBuffer != 0x00){
		delay_microBuffer--;
	}
}

