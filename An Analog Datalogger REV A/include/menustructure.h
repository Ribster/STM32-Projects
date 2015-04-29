/*
 * menustructure.h
 *
 *  Created on: 26-apr.-2015
 *      Author: Robbe
 */

#ifndef MENUSTRUCTURE_H_
#define MENUSTRUCTURE_H_

#include "stm32f4xx.h"

#include "projectconfig.h"

#include "initialization.h"

#include "encoders.h"
#include "ssd1306.h"
#include "gpio.h"
#include "delay.h"
#include "sdio.h"

// fatfs library
#include "diskio.h"
#include "ff.h"

extern uint8_t menu_enable;

typedef struct menuItem_t{
	uint8_t menuLevel;
	struct menuItem_t* upperMenuItem; // hierarchical higher
	struct menuItem_t* lowerMenuItem; // hierarchical lower
	struct menuItem_t* leftMenuItem;  // hierarchical same level, left side
	struct menuItem_t* rightMenuItem; // hierarchical same level, right side
	char* label;
	uint8_t selected;
	uint32_t menuID;
	void (*fptr)(void);
}menuItem_t;

void
initialize_menuStructure(void);

menuItem_t*
menustructure_addItem(menuItem_t* upperMenuItem, char* label, void(*func)(void));



void
menustructure_showMenu(menuItem_t* parentItem);

void
menustructure_printMenuItem(menuItem_t* menuStructure);

void
menustructure_printMenuItemRow(menuItem_t* menuStructure);

void
menustructure_render(void);

void
menustructure_stepMenu(uint8_t direction);

// SD CARD ROUTINES
void
menustructure_menuFunction_SD_Cardinfo(void);

void
menustructure_menuFunction_SD_Filestructure(void);

void
menustructure_menuFunction_SD_CardTest(void);

void
menustructure_menuFunction_SD_Sdsettings(void);

// RTC ROUTINES
void
menustructure_menuFunction_RTC_Rtcinfo(void);

void
menustructure_menuFunction_RTC_Adjustrtc(void);

void
menustructure_menuFunction_RTC_Rtcsettings(void);

// USB ROUTINES
void
menustructure_menuFunction_USB_Info(void);
void
menustructure_menuFunction_USB_Test(void);
void
menustructure_menuFunction_USB_Settings(void);


// AFE ROUTINES
void
menustructure_menuFunction_AFE_Info(void);

void
menustructure_menuFunction_AFE_Record(void);

void
menustructure_menuFunction_AFE_Settings(void);

// TERMINAL ROUTINES

void
menustructure_menuFunction_TERMINAL_Info(void);

void
menustructure_menuFunction_TERMINAL_Test(void);

void
menustructure_menuFunction_TERMINAL_Output(void);

void
menustructure_menuFunction_TERMINAL_Settings(void);


#endif /* MENUSTRUCTURE_H_ */
