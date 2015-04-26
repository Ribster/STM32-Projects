/*
 * initialization.c
 *
 *  Created on: 13-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "initialization.h"

char* initialization_list_BUFFER[initialization_list_SIZE];
uint8_t initialization_list_STATES[initialization_list_SIZE];

// implementation
void
initialization(void){

	// initialize stringList
	initialization_initStringList();

	// do the initialization of the UART
	initialization_prefix(initialization_list_UART);
		initialize_UART();
	initialization_suffix(initialization_list_UART);

	// do the initialization of the leds
	initialization_prefix(initialization_list_LEDs);
		initialize_LEDS();
	initialization_suffix(initialization_list_LEDs);

	// do the initialization of the pushbuttons
	initialization_prefix(initialization_list_Pushbuttons);
		initialize_pushButtons();
	initialization_suffix(initialization_list_Pushbuttons);

	// do the initialization of the encoders
	initialization_prefix(initialization_list_Encoders);
		initialize_encoderOne();
		initialize_encoderTwo();
	initialization_suffix(initialization_list_Encoders);

	// do the initialization of the systick timer
	initialization_prefix(initialization_list_Systick);
		delay_setup();
	initialization_suffix(initialization_list_Systick);

	// do the initialization of the RTC
	initialization_prefix(initialization_list_RTC);
		initialize_RTC();
	initialization_suffix(initialization_list_RTC);

	// do the intialization of the AFE
	initialization_prefix(initialization_list_AFE);
		initialize_AFE();
	initialization_suffix(initialization_list_AFE);

	// do the initialization of the Random Number Generator
	initialization_prefix(initialization_list_RNG);
		initialize_RNG();
	initialization_suffix(initialization_list_RNG);

	// do the initialization of the SD card
	initialization_prefix(initialization_list_SD);
		initialize_SDIO();
	initialization_suffix(initialization_list_SD);

	// do the initialization of the OLED -- This needs to be done after the systick initialization!!
	initialization_prefix(initialization_list_SSD1306);
		initialize_SSD1306();
	initialization_suffix(initialization_list_SSD1306);

	// do the initialization of the USB
	//initialization_prefix(initialization_list_USB);
		//initialize_USB();
	//initialization_suffix(initialization_list_USB);

	// do the initialization of the nRF24L01p
	//initialization_prefix(initialization_list_nRF);
		//initialize_nRF24L01p();
	//initialization_suffix(initialization_list_nRF);

	// initialize Homescreen

	ssd1306_setLine(0, 55, 127, 55);

	char* verString = "VER:";
	char* startString = " An Analog Datalogger\nREV A.\t";
	char* endString = "\n by Robbe Van Assche";
	char* newString = malloc(strlen(startString) + strlen(verString) + strlen(PROJVER) + strlen(endString));
	sprintf(newString, "%s%s%s%s", startString, verString, PROJVER, endString);
	ssd1306_setTextBlock(0,0,127,53,
			newString,
			Font_System5x8, 0);

	// print initializations
	delay_milli(2500);
	initialization_printList();
}
void
initialization_initStringList(void){
	initialization_addString(initialization_list_UART, "UART");
	initialization_addString(initialization_list_LEDs, "LEDs");
	initialization_addString(initialization_list_Pushbuttons, "PushBttns");
	initialization_addString(initialization_list_Encoders, "Encoders");
	initialization_addString(initialization_list_Systick, "Systick");
	initialization_addString(initialization_list_RTC, "RTC");
	initialization_addString(initialization_list_AFE, "AFE");
	initialization_addString(initialization_list_RNG, "RNG");
	initialization_addString(initialization_list_SD, "SD");
	initialization_addString(initialization_list_SSD1306, "SSD1306");
	initialization_addString(initialization_list_USB, "USB");
	initialization_addString(initialization_list_nRF, "nRF24L01+");
}

void
initialization_addString(initialization_list_t addVal, char* str){
	//initialization_list_BUFFER[addVal] = strdup(str);
	initialization_list_BUFFER[addVal] = str;
	initialization_list_STATES[addVal] = 0x00;
}

void
initialization_printList(void){
	// print an initialization list on the SSD1306

}


void
initialization_prefix(initialization_list_t val){
	printf("initializing %s...", initialization_list_BUFFER[val]);
}

void
initialization_suffix(initialization_list_t val){
	initialization_list_STATES[val] = 0x01;
	printf("...Done!\r\n");
}



