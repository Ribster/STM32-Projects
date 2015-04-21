/*
 * main.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

// std lib
	#include "stm32f4xx.h"

// arm-none-eabi
	#include <stdio.h>
// project files
	#include "projectconfig.h"
	#include "initialization.h"
	#include "leds.h"
	#include "pushbuttons.h"
	#include "delay.h"
	#include "dma.h"
	#include "gpio.h"
	#include "rcc.h"
	#include "rtc.h"
	#include "sdio.h"
	#include "spi.h"
	#include "ssd1306.h"
	#include "uart.h"
	#include "usb.h"
	#include "encoders.h"
	#include "sdio.h"
// fatfs lib
	#include "diskio.h"
	#include "ff.h"

// global variables
char TERMINAL_receiverbuffer[TERMINAL_IT_RX_MAXSTRINGLENGTH];


// main routine
	int main(void){
		// setup
		initialization();

		uint32_t counter = 0;

		while(1){
		// loop

//			leds_ledTest();
			delay_milli(2000);
			//leds_ledEncoderExample();
//			printf(" - ");
//			rtc_printTimeTerminal();
//			delay_milli(200);
//			ssd1306_pixbufferTest();
//			ssd1306_lineTest();
//			ssd1306_circleTest();
//			ssd1306_areaTest();
//			ssd1306_stringTest();



			printf(" -- loop nr %ld \r\n", counter++);
		}
	}


//******************************************************************************
