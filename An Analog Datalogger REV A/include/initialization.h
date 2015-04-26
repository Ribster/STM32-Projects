/*
 * initialization.h
 *
 *  Created on: 13-apr.-2015
 *      Author: Robbe Van Assche
 */


#ifndef INITIALIZATION_PROJECT_H_
#define INITIALIZATION_PROJECT_H_

#include "stm32f4xx.h"

#include <string.h>

#include "delay.h"
#include "gpio.h"
#include "rcc.h"
#include "uart.h"
#include "leds.h"
#include "pushbuttons.h"
#include "projectconfig.h"
#include "encoders.h"
#include "rtc.h"
#include "ssd1306.h"
#include "rng.h"
#include "sdio.h"
#include "afe.h"
#include "usb.h"
#include "menustructure.h"
//#include "nRF24L01p.h"

typedef enum initialization_list_t{
	initialization_list_UART,
	initialization_list_LEDs,
	initialization_list_Pushbuttons,
	initialization_list_Encoders,
	initialization_list_Systick,
	initialization_list_RTC,
	initialization_list_AFE,
	initialization_list_RNG,
	initialization_list_SD,
	initialization_list_SSD1306,
	initialization_list_USB,
	initialization_list_nRF
}initialization_list_t;

#define initialization_list_SIZE ((uint32_t)initialization_list_nRF+1)

// prototype
void
initialization(void);

void
initialization_initStringList(void);

void
initialization_addString(initialization_list_t addVal, char* str);

void
initialization_printList(void);

void
initialization_prefix(initialization_list_t val);

void
initialization_suffix(initialization_list_t val);

#endif /* INITIALIZATION_H_ */
