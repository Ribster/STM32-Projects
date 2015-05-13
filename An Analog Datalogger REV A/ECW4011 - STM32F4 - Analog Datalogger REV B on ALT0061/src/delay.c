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
	// increment total systemtime
	delay_totalBuffer++;

	// decrement buffer register
	if(delay_microBuffer != 0x00){
		delay_microBuffer--;
	}

	// set the orange led for SD activity
	if(sd_busy == 0x01){
		leds_setLed(ledList_Orange, ENABLE);

	} else {
		leds_setLed(ledList_Orange, DISABLE);
	}

	// division of 1000 -> 1s interval
	if(delay_totalBuffer%1000 == 0){

		// trigger afe read. TODO: move to RTC triggered routine
		if(afe_enableRecording != 0x00){
			afe_recordingTime++;
			afe_startReadout();
		}

		// print RTC time to the OLED
		rtc_setTimeToOLED();

		// heartbeat on green led
		leds_toggleLed(ledList_Green);


		// if the menu cannot be updated for some reason, try updating it 1s later
		if(ssd1306_updateLater_1s != 0x00){
			ssd1306_updateLater_1s--;
			menu_enable = 0x01;
		}
	}

	// division of 100 -> 100ms interval
	if(delay_totalBuffer%100 == 0){
		// if the menu cannot be updated for some reason, try updating it 100ms later
		if(ssd1306_updateLater_100ms != 0x00){
			ssd1306_updateLater_100ms--;
			menu_enable = 0x01;
		}
	}

}
uint32_t
delay_getMillis(void){
	// return the total buffer
	return delay_totalBuffer;
}
uint32_t
delay_getMilliDifference(uint32_t t0, uint32_t t1){
	// return the timing difference between 2 millis() values
	return t1-t0;
}
uint32_t
delay_getMilliDifferenceSimple(uint32_t t0){
	// simplify timing difference with current time, wrapper function
	return delay_getMilliDifference(t0, delay_totalBuffer);
}
