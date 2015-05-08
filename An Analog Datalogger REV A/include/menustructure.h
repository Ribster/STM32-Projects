/*
 * menustructure.h
 *
 *  Created on: 26-apr.-2015
 *      Author: Robbe
 */

#ifndef MENUSTRUCTURE_H_
#define MENUSTRUCTURE_H_




#include "stm32f4xx.h"

#include <string.h>

#include "projectconfig.h"
#include "general.h"

#include "ssd1306.h"
#include "initialization.h"
#include "encoders.h"
#include "gpio.h"
#include "delay.h"
#include "sdio.h"
#include "fonts/smallfonts.h"

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

typedef struct fileStructure_t{
	uint8_t menuLevel;
	struct fileStructure_t* upperFileItem; // hierarchical higher
	struct fileStructure_t* lowerFileItem; // hierarchical lower
	struct fileStructure_t* leftFileItem;  // hierarchical same level, left side
	struct fileStructure_t* rightFileItem; // hierarchical same level, right side
	char* label;
	uint8_t selected;
	uint32_t fileID;
	char DIR;
	char RDO;
	char SYS;
	char HID;
	uint32_t size;
	char* dirName;
}fileStructure_t;

void
initialize_menuStructure(void);

menuItem_t*
menustructure_addItem(menuItem_t* upperMenuItem, char* label, void(*func)(void));

// NORMAL MENU STRUCTURE

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

// SD FILESYSTEM

void
menustructure_file_stepMenu(uint8_t direction);

fileStructure_t*
menustructure_file_addItem(fileStructure_t* upperFileItem, char* label, uint32_t size, uint8_t folder, uint8_t attrib);

void
menustructure_file_insertItem(void);

void
menustructure_file_showMenu(fileStructure_t* parentItem, xypair_t xy0, xypair_t xy1, struct FONT_DEF font);

uint8_t
menustructure_file_showMenuItem(fileStructure_t* fileItem, uint8_t y, uint8_t x0, uint8_t x1, struct FONT_DEF font);

void
menustructure_file_deleteMenu(fileStructure_t* parentItem);

void
menustructure_file_fillMenu(void);

void
menustructure_file_updateMenu(void);



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
