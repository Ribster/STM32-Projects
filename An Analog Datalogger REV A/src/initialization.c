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

	// do the initialization of the systick timer
	delay_setup();
}




