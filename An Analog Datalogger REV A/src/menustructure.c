/*
 * menustructure.c
 *
 *  Created on: 26-apr.-2015
 *      Author: Robbe
 */
#include "menustructure.h"

static uint32_t lastMoved;
menuItem_t menu_MAIN;
menuItem_t* menu_currentRef;
uint8_t menu_enable;
uint32_t menu_menuID;

inline static void
menustructure_stepMenu(uint8_t direction);

void
initialize_menuStructure(void){
	menu_menuID = 0;
	menu_MAIN.menuID = ++menu_menuID;
	menu_enable = 0x00;

	menuItem_t* menu_RTC = menustructure_addItem(&menu_MAIN, "RTC", 0);
	menuItem_t* menu_SD = menustructure_addItem(&menu_MAIN, "SD Card", 0);
	menuItem_t* menu_USB = menustructure_addItem(&menu_MAIN, "USB", 0);
	menuItem_t* menu_AFE = menustructure_addItem(&menu_MAIN, "AFE", 0);

	menustructure_addItem(menu_SD, "SD Card Info", menustructure_menuFunctionSDCardinfo);
	menustructure_addItem(menu_SD, "File Structure", menustructure_menuFunctionSDFilestructure);
	menustructure_addItem(menu_SD, "SD Settings", menustructure_menuFunctionSDSdsettings);

	menustructure_addItem(menu_RTC, "RTC Info", menustructure_menuFunctionRTCRtcinfo);
	menustructure_addItem(menu_RTC, "Adjust RTC", menustructure_menuFunctionRTCAdjustrtc);
	menustructure_addItem(menu_RTC, "RTC Settings", menustructure_menuFunctionRTCRtcsettings);

	menustructure_addItem(menu_USB, "USB Info", menustructure_menuFunctionUSBInfo);

	menustructure_addItem(menu_AFE, "AFE Info", menustructure_menuFunctionAFEInfo);
	menustructure_addItem(menu_AFE, "AFE Record", menustructure_menuFunctionAFERecord);

	menu_currentRef = menu_MAIN.lowerMenuItem;

#ifdef DGBIO
	menustructure_printAllMenuItems(&menu_MAIN);
#endif
}

menuItem_t*
menustructure_addItem(menuItem_t* upperMenuItem, char* label, void(*func)(void)){
	menuItem_t* tmp = malloc(sizeof(menuItem_t));
	memset(tmp, 0, sizeof(menuItem_t));
	if(tmp!=0){
		tmp->upperMenuItem = upperMenuItem;
		tmp->label = label;
		tmp->fptr = func;
		tmp->menuLevel = upperMenuItem->menuLevel+1;
		tmp->menuID = ++menu_menuID;
		// attach the current new item to the structure
		menustructure_attachMenuItem(upperMenuItem, tmp);
	}
	return tmp;
}

void
menustructure_attachMenuItem(menuItem_t* sourceItem, menuItem_t* attachItem){
	// sourceitem is for example an item from lvl one.
	// check if lowermenuitem is not null
		// if it is null-> assign attachitem to it
		// it it's not null-> go down the lower item and search for the last item

	// reference the upper menu item
	attachItem->upperMenuItem = sourceItem;

	if (sourceItem->lowerMenuItem == 0){
		sourceItem->lowerMenuItem = attachItem;
		attachItem->selected = 0x01;
	} else {
		menuItem_t* tmp = sourceItem->lowerMenuItem;
		while(tmp->nextMenuRef!=0){
			tmp = tmp->nextMenuRef;
		}
		tmp->nextMenuRef = attachItem;
		attachItem->previousMenuRef = tmp;
	}
}

void
menustructure_printAllMenuItems(menuItem_t* menuStructure){
	menustructure_printMenuItemRow(menuStructure->lowerMenuItem);
}

void
menustructure_printMenuItem(menuItem_t* menuStructure){
	if(strlen(menuStructure->label)>8){
		printf("Label:%s\tLevel:%d\tSelected:%d\tID:%ld\tUP:0x%lx\tDOWN:0x%08lx\tLeft:0x%08lx\tRight:0x%08lx\tCurrent:0x%08lx\tfptr:0x%08lx\r\n",
				menuStructure->label, menuStructure->menuLevel, menuStructure->selected, menuStructure->menuID, (uint32_t)menuStructure->upperMenuItem
				, (uint32_t)menuStructure->lowerMenuItem, (uint32_t)menuStructure->previousMenuRef, (uint32_t)menuStructure->nextMenuRef, (uint32_t)menuStructure->currentMenuRef, (uint32_t)menuStructure->fptr);
	} else {
		printf("Label:%s\t\tLevel:%d\tSelected:%d\tID:%ld\tUP:0x%08lx\tDOWN:0x%08lx\tLeft:0x%08lx\tRight:0x%08lx\tCurrent:0x%08lx\tfptr:0x%08lx\r\n",
				menuStructure->label, menuStructure->menuLevel, menuStructure->selected, menuStructure->menuID, (uint32_t)menuStructure->upperMenuItem
				, (uint32_t)menuStructure->lowerMenuItem, (uint32_t)menuStructure->previousMenuRef, (uint32_t)menuStructure->nextMenuRef, (uint32_t)menuStructure->currentMenuRef, (uint32_t)menuStructure->fptr);
	}
}

void
menustructure_printMenuItemRow(menuItem_t* menuStructure){
	// recursive function
	menuItem_t* tmp = menuStructure;
	while(tmp!=0x00){
		menustructure_printMenuItem(tmp);
		menustructure_printMenuItemRow(tmp->lowerMenuItem);
		tmp = tmp->nextMenuRef;
	}
}



void
menustructure_render(void){
	if(menu_enable == 0) return;

	//ssd1306_clearArea(OLED_TEXTBLOCK_DIMENSIONS);
}

inline static void
menustructure_stepMenu(uint8_t direction){
	if(delay_getMilliDifferenceSimple(lastMoved)>50){
		if(direction == 0){
			// Stepping up
			printf("Stepping UP! \r\n");
		} else if (direction == 1){
			// Stepping down
			printf("Stepping DOWN! \r\n");
		}
	}
	lastMoved = delay_getMillis();
}

void
menustructure_stepMenuUp(void){
	menustructure_stepMenu(0);
}

void
menustructure_stepMenuDown(void){
	menustructure_stepMenu(1);
}




void
menustructure_menuFunctionSDCardinfo(void){

}

void
menustructure_menuFunctionSDFilestructure(void){

}

void
menustructure_menuFunctionSDSdsettings(void){

}

void
menustructure_menuFunctionRTCRtcinfo(void){

}

void
menustructure_menuFunctionRTCAdjustrtc(void){

}

void
menustructure_menuFunctionRTCRtcsettings(void){

}

void
menustructure_menuFunctionUSBInfo(void){

}

void
menustructure_menuFunctionAFEInfo(void){

}

void
menustructure_menuFunctionAFERecord(void){

}
