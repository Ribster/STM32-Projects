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
		// setup initialization
		initialization();

		// print the home screen
		ssd1306_setTextBlock(0,0,127,50,
				"An Analog Datalogger\n"
				"REV A.\n"
				"Under Construction\n",
				Font_System5x8, 0);
		delay_milli(3000);
		ssd1306_setTextBlock(0,0,127,50,
				"An Analog Datalogger         REV A.       "
				"                      "
				"Under Construction.",
				Font_System5x8, 0);


		// make counter variable
		uint32_t counter = 0;

		// final loop
			while(1){
				// increment the loopcounter
				counter++;

				// 2 Start interrupt possibilities
					// RTC interrupt every second

					// External interrupt

				// Upon interrupt, read out AFE

				// Write data from AFE to SD card

				// Write the loopcounter
					// make a loopcounter with RTC clock displayed
					#define printLength 11
					char nr[printLength];
					xypair_t tmp = ssd1306_getScreenDimensions();
					uint8_t stringHeigth = (Font_System5x8.u8Height);
					uint8_t stringWidth = (Font_System5x8.u8Width*(printLength-1))+(printLength-2);
					sprintf(nr, "%010ld",counter);
					ssd1306_clearArea(0,tmp.y-stringHeigth, stringWidth, tmp.y);
					ssd1306_setString(0,tmp.y-stringHeigth-1,nr,Font_System5x8);

					printf(" -- loop nr %ld \r\n", counter++);
					delay_milli(1000);
			}


		// testloop
			while(1){
				// loop
				#define TESTLOOP	5
				counter++;

				#if TESTLOOP==1
					// just testing the leds
					leds_ledTest();

				#elif TESTLOOP==2
					// test the encoders
					leds_ledEncoderExample();
					printf(" - ");
					rtc_printTimeTerminal();
					delay_milli(200);

				#elif TESTLOOP==3
					// test the SSD1306 functions
					ssd1306_pixbufferTest();
					ssd1306_lineTest();
					ssd1306_circleTest();
					ssd1306_areaTest();
					ssd1306_stringTest();

				#elif TESTLOOP==4

					// make a loopcounter with RTC clock displayed
					#define printLength 11
					char nr[printLength];
					xypair_t tmp = ssd1306_getScreenDimensions();
					uint8_t stringHeigth = (Font_System5x8.u8Height);
					uint8_t stringWidth = (Font_System5x8.u8Width*(printLength-1))+(printLength-2);
					sprintf(nr, "%010ld",counter);
					ssd1306_clearArea(0,tmp.y-stringHeigth, stringWidth, tmp.y);
					ssd1306_setString(0,tmp.y-stringHeigth-1,nr,Font_System5x8);

					printf(" -- loop nr %ld \r\n", counter++);
					delay_milli(1000);

				#elif TESTLOOP==5
					/* If there's data on the virtual serial port:
					 *  - Echo it back
					 */
					uint8_t theByte;
					if (VCP_get_char(&theByte))
					{
						VCP_put_char(theByte);
					}
				#endif
	//






			}
	}


//******************************************************************************
