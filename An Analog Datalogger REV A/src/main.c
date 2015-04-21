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
		//ssd1306_setOrientation(SSD1306_ORIENTATION_LANDSCAPE_UpsideDown);
		for(uint8_t i=0;i<40; i++){
			ssd1306_setTextBlock(0,0,127,50,
					"Sed tristique pharetra mi, faucibus mollis ex. "
					"Proin consectetur ipsum tincidunt, faucibus libero eget, "
					"feugiat risus. In dignissim, metus eu pellentesque pharetra, "
					"purus enim fringilla felis, in lobortis magna nisl sit amet arcu. "
					"Sed ex lacus, vulputate ut orci ac, tincidunt mattis ipsum. "
					"Curabitur nulla odio, sagittis non accumsan ultrices, cursus ac metus. "
					"Suspendisse et lacinia ipsum, molestie condimentum nulla. "
					"Pellentesque habitant morbi tristique senectus et netus et malesuada "
					"fames ac turpis egestas. Aliquam erat volutpat. Vestibulum vel "
					"vulputate nisi. Integer euismod nibh facilisis, tempor ipsum eget, "
					"tincidunt justo. Cras ultrices interdum metus, in efficitur lectus "
					"imperdiet eu. Integer et felis et velit semper bibendum in nec purus.",
					Font_System5x8, i);
			char nr[4];
			xypair_t tmp = ssd1306_getScreenDimensions();
			uint8_t stringHeigth = (Font_System3x6.u8Height);
			uint8_t stringWidth = (Font_System3x6.u8Width*3)+(1);
			sprintf(nr, "%03d",i);
			ssd1306_clearArea(0,tmp.y-stringHeigth-1, stringWidth, tmp.y-1);
			ssd1306_setString(0,tmp.y-stringHeigth-1,nr,Font_System3x6);
			delay_milli(100);
		}

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
