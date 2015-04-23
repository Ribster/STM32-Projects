/*
 * initialization.c
 *
 *  Created on: 13-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "initialization.h"

// implementation
void
initialization(void){

	// do the initialization of the UART
	initialize_UART();

	// do the initialization of the leds
	initialize_LEDS();

	// do the initialization of the pushbuttons
	initialize_pushButtons();

	// do the initialization of the encoders
	initialize_encoderOne();
	initialize_encoderTwo();

	// do the initialization of the systick timer
	delay_setup();

	// do the initialization of the RTC
	initialize_RTC();

	// do the initialization of the Random Number Generator
	initialize_RNG();

	// do the initialization of the SD card
	initialize_SDIO();

	// do the intialization of the AFE
	initialize_AFE();

	// do the initialization of the OLED -- This needs to be done after the systick initialization!!
	initialize_SSD1306();

	// do the initialization of the USB
	initialize_USB();

	// do the initialization of the nRF24L01p
	//initialize_nRF24L01p();
}




