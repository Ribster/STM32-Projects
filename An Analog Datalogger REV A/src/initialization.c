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
	initialization_prefix("LEDs");
	initialize_LEDS();
	initialization_suffix();



	// do the initialization of the pushbuttons
	initialization_prefix("Pushbuttons");
	initialize_pushButtons();
	initialization_suffix();

	// do the initialization of the encoders
	initialization_prefix("Encoders");
	initialize_encoderOne();
	initialize_encoderTwo();
	initialization_suffix();


	// do the initialization of the systick timer
	initialization_prefix("Systick");
	delay_setup();
	initialization_suffix();

	// do the initialization of the RTC
	initialization_prefix("RTC");
	initialize_RTC();
	initialization_suffix();

	// do the intialization of the AFE
	initialization_prefix("AFE");
	initialize_AFE();
	initialization_suffix();

	// do the initialization of the Random Number Generator
	initialization_prefix("RNG");
	initialize_RNG();
	initialization_suffix();

	// do the initialization of the SD card
	initialization_prefix("SD");
	initialize_SDIO();
	initialization_suffix();

	// do the initialization of the OLED -- This needs to be done after the systick initialization!!
	initialization_prefix("SSD1306");
	initialize_SSD1306();
	initialization_suffix();




	// do the initialization of the USB
	//initialization_prefix("USB");
	//initialize_USB();
	//initialization_suffix();

	// do the initialization of the nRF24L01p
	//initialization_prefix("nRF");
	//initialize_nRF24L01p();
	//initialization_suffix();
}
void
initialization_prefix(char* str){
	printf("initializing %s...", str);
}

void
initialization_suffix(void){
	printf("... Done!\r\n");
}



