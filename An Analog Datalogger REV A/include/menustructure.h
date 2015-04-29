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
menustructure_attachMenuItem(menuItem_t* sourceItem, menuItem_t* attachItem);

void
menustructure_showMenu(menuItem_t* parentItem);

void
menustructure_printMenuItem(menuItem_t* menuStructure);

void
menustructure_printMenuItemRow(menuItem_t* menuStructure);

void
menustructure_render(void);

void
menustructure_stepMenuUp(void);

void
menustructure_stepMenuDown(void);

void
menustructure_stepMenuEnter(void);

void
menustructure_stepMenuBack(void);

void
menustructure_menuFunctionSDCardinfo(void);

void
menustructure_menuFunctionSDFilestructure(void);

void
menustructure_menuFunctionSDSdsettings(void);

void
menustructure_menuFunctionRTCRtcinfo(void);

void
menustructure_menuFunctionRTCAdjustrtc(void);

void
menustructure_menuFunctionRTCRtcsettings(void);

void
menustructure_menuFunctionUSBInfo(void);

void
menustructure_menuFunctionAFEInfo(void);

void
menustructure_menuFunctionAFERecord(void);



#endif /* MENUSTRUCTURE_H_ */
