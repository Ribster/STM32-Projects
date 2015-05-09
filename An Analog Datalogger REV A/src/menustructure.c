/*
 * menustructure.c
 *
 *  Created on: 26-apr.-2015
 *      Author: Robbe
 */
#include "menustructure.h"

static uint32_t lastMoved;
menuItem_t menu_MAIN;		// main structure that poin
fileStructure_t file_MAIN;
menuItem_t* menu_currentRef;
fileStructure_t* file_currentRef;
uint8_t menu_enable;	// enable menu to be updated
uint32_t menu_menuID;	// container for the unique menu ID's
uint32_t file_fileID;	// container for the unique file ID's
uint8_t file_Filestructure_Entered;

static inline void
menustructure_attachMenuItem(menuItem_t* sourceItem, menuItem_t* attachItem);

static inline void
menustructure_file_attachMenuItem(fileStructure_t* sourceFile, fileStructure_t* attachFile);

void
initialize_menuStructure(void){
	menu_menuID = 0x00;
	menu_MAIN.menuID = ++menu_menuID;
	menu_MAIN.selected = 0x01;
	menu_enable = 0x00;

	menuItem_t* menu_TERMINAL = menustructure_addItem(&menu_MAIN, "Terminal", 0x00);
	menuItem_t* menu_RTC = menustructure_addItem(&menu_MAIN, "RTC", 0x00);
	menuItem_t* menu_SD = menustructure_addItem(&menu_MAIN, "SD Card", 0x00);
	menuItem_t* menu_USB = menustructure_addItem(&menu_MAIN, "USB", 0x00);
	menuItem_t* menu_AFE = menustructure_addItem(&menu_MAIN, "AFE", 0x00);

	menustructure_addItem(menu_SD, "SD Card Info", menustructure_menuFunction_SD_Cardinfo);
	menustructure_addItem(menu_SD, "File Structure", menustructure_menuFunction_SD_Filestructure);
	menustructure_addItem(menu_SD, "SD Card test", menustructure_menuFunction_SD_CardTest);
	menustructure_addItem(menu_SD, "SD Card Settings", menustructure_menuFunction_SD_Sdsettings);

	menustructure_addItem(menu_RTC, "RTC Info", menustructure_menuFunction_RTC_Rtcinfo);
	menustructure_addItem(menu_RTC, "Adjust RTC", menustructure_menuFunction_RTC_Adjustrtc);
	menustructure_addItem(menu_RTC, "RTC Settings", menustructure_menuFunction_RTC_Rtcsettings);

	menustructure_addItem(menu_USB, "USB Info", menustructure_menuFunction_USB_Info);
	menustructure_addItem(menu_USB, "USB Test", menustructure_menuFunction_USB_Test);
	menustructure_addItem(menu_USB, "USB Settings", menustructure_menuFunction_USB_Settings);

	menustructure_addItem(menu_AFE, "AFE Info", menustructure_menuFunction_AFE_Info);
	menustructure_addItem(menu_AFE, "AFE Record", menustructure_menuFunction_AFE_Record);
	menustructure_addItem(menu_AFE, "AFE Settings", menustructure_menuFunction_AFE_Settings);

	menustructure_addItem(menu_TERMINAL, "Terminal Info", menustructure_menuFunction_TERMINAL_Info);
	menustructure_addItem(menu_TERMINAL, "Terminal Test", menustructure_menuFunction_TERMINAL_Test);
	menustructure_addItem(menu_TERMINAL, "Terminal Output", menustructure_menuFunction_TERMINAL_Output);
	menustructure_addItem(menu_TERMINAL, "Terminal Settings", menustructure_menuFunction_TERMINAL_Settings);

	menu_currentRef = &menu_MAIN;
}

menuItem_t*
menustructure_addItem(menuItem_t* upperMenuItem, char* label, void(*func)(void)){
	menuItem_t* tmp = calloc(1,sizeof(menuItem_t));
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

static inline void
menustructure_attachMenuItem(menuItem_t* sourceItem, menuItem_t* attachItem){
	// sourceitem is for example an item from lvl one.
	// check if lowermenuitem is not null
		// if it is null-> assign attachitem to it
		// it it's not null-> go down the lower item and search for the last item

	// reference the upper menu item
	attachItem->upperMenuItem = sourceItem;

	if (sourceItem->lowerMenuItem == 0x00){
		sourceItem->lowerMenuItem = attachItem;
		attachItem->selected = 0x01;
	} else {
		menuItem_t* tmp = sourceItem->lowerMenuItem;
		while(tmp->rightMenuItem!=0x00){
			tmp = tmp->rightMenuItem;
		}
		tmp->rightMenuItem = attachItem;
		attachItem->leftMenuItem = tmp;
	}
}

void
menustructure_showMenu(menuItem_t* parentItem){
	if(parentItem->lowerMenuItem != 0x00){
		// show all the lower menu items
		xypair_t startPos = {OLED_MENUWRITING_START};
		xypair_t endPos = {OLED_MENUWRITING_END};
		xypair_t curPos;
		struct FONT_DEF font = OLED_MENUWRITING_FONT;
		uint32_t minWidth = OLED_MENUWRITING_MINIMUMWIDTH_CHARSIZE*(font.u8Width+1);

		// calculate the lines possible to fill the height of the screen
		uint32_t lineCount = startPos.y / (font.u8Height);
		if( (lineCount*(font.u8Height) + (lineCount-1)) <= startPos.y){
			lineCount++;
		}

		// get total items, position of selected item, max stringlength
		menuItem_t* tmp = parentItem->lowerMenuItem;
		uint32_t totalCount = 0x00;
		uint32_t selectedPos = 0x00;
		uint32_t strLength = 0x00;
		while(tmp!=0x00){
			// search for the total itemcount
			totalCount++;

			if(tmp->selected == 0x01){
				// search for the position of the selected item in the total itemcount
				if(selectedPos!=0x00){
					tmp->selected = 0x00;
				} else {
					selectedPos = totalCount;
				}
			}

			if(totalCount<= lineCount){
				strLength = MAX(strLength, strlen(tmp->label));
			}
			tmp = tmp->rightMenuItem;
		}
			// define the actual max stringwidth
			strLength *= (font.u8Width+1);
			strLength = MAX(strLength, minWidth);

		// print every item out
			// clear the screen
			ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
			menuItem_t* startItem = parentItem->lowerMenuItem;
			curPos.x = startPos.x;
			curPos.y = startPos.y-2;
			while(startItem !=0x00){
				// take the previous position
				// check if it's possible to write the string
					if(curPos.y>=(endPos.y+font.u8Height-1)){
						// center the string
							if(startItem->selected != 0x00){
								// write the string if checked
								ssd1306_setArea(curPos.x,curPos.y+2, curPos.x+strLength, curPos.y-font.u8Height+1);
								ssd1306_setStringInvertedCentered(curPos.y-font.u8Height,
										startPos.x, endPos.x,
										startItem->label,
										font,
										0);
							} else {
								// write the string if not checked
								ssd1306_setStringCentered(curPos.y-font.u8Height, startPos.x, endPos.x, startItem->label,font);
							}
							// set the new position
							curPos.y -= (font.u8Height+2);
					}
					startItem = startItem->rightMenuItem;
			}
			menu_enable = 0x01;
	} else if (parentItem->fptr != 0x00){
		// go to function
		if(parentItem->fptr == menustructure_menuFunction_SD_Filestructure){
		}

		parentItem->fptr();
	}
}

void
menustructure_printMenuItem(menuItem_t* menuStructure){
	if(strlen(menuStructure->label)>8){
		printf("Label:%s\tLevel:%d\tSelected:%d\tID:%ld\tUP:0x%lx\tDOWN:0x%08lx\tLeft:0x%08lx\tRight:0x%08lx\tfptr:0x%08lx\r\n",
				menuStructure->label, menuStructure->menuLevel, menuStructure->selected, menuStructure->menuID, (uint32_t)menuStructure->upperMenuItem
				, (uint32_t)menuStructure->lowerMenuItem, (uint32_t)menuStructure->leftMenuItem, (uint32_t)menuStructure->rightMenuItem, (uint32_t)menuStructure->fptr);
	} else {
		printf("Label:%s\t\tLevel:%d\tSelected:%d\tID:%ld\tUP:0x%08lx\tDOWN:0x%08lx\tLeft:0x%08lx\tRight:0x%08lx\tfptr:0x%08lx\r\n",
				menuStructure->label, menuStructure->menuLevel, menuStructure->selected, menuStructure->menuID, (uint32_t)menuStructure->upperMenuItem
				, (uint32_t)menuStructure->lowerMenuItem, (uint32_t)menuStructure->leftMenuItem, (uint32_t)menuStructure->rightMenuItem, (uint32_t)menuStructure->fptr);
	}
}

void
menustructure_printMenuItemRow(menuItem_t* menuStructure){
	// recursive function
	menuItem_t* tmp = menuStructure;
	while(tmp!=0x00){
		menustructure_printMenuItem(tmp);
		menustructure_printMenuItemRow(tmp->lowerMenuItem);
		tmp = tmp->rightMenuItem;
	}
}

void
menustructure_render(void){
	if(menu_enable != 0x00 && initialization_list_STATES[initialization_list_SSD1306] != 0x00 && initialization_list_STATES[initialization_list_General] != 0x00){
		// show the menu items
		menustructure_showMenu(menu_currentRef);

		menu_enable = 0x00;
	}
}

void
menustructure_stepMenu(uint8_t direction){
	if(delay_getMilliDifferenceSimple(lastMoved)>PB_DEBOUNCE_DELAY_MS){
		if(direction == OLED_MENU_STEP_UP){
			// Stepping up (pointerwise : left)
			if(menu_currentRef->lowerMenuItem != 0x00){
				// there are items under the parent structure
				menuItem_t* tmp = menu_currentRef->lowerMenuItem;
				while(tmp->selected == 0x00){
					tmp = tmp->rightMenuItem;
				}
				if(tmp->leftMenuItem != 0x00){
					tmp->selected = 0x00;
					tmp->leftMenuItem->selected = 0x01;
				}
			} else if(menu_currentRef->fptr != 0x00){
				// check if the functionpointer is filled in
				if((menu_currentRef->fptr) == (menustructure_menuFunction_SD_Filestructure)){
					menustructure_file_stepMenu(direction);
				}
			}
#ifdef DBG
			printf("Stepping UP! \r\n");
#endif
			menu_enable = 0x01;
		} else if (direction == OLED_MENU_STEP_DOWN){
			// Stepping down (pointerwise: right)
			if(menu_currentRef->lowerMenuItem != 0x00){
				// there are items under the parent structure
				menuItem_t* tmp = menu_currentRef->lowerMenuItem;
				while(tmp->selected == 0x00){
					tmp = tmp->rightMenuItem;
				}
				if(tmp->rightMenuItem != 0x00){
					tmp->selected = 0x00;
					tmp->rightMenuItem->selected = 0x01;
				}
			} else if(menu_currentRef->fptr != 0x00){
				// check if the functionpointer is filled in
				if((menu_currentRef->fptr) == (menustructure_menuFunction_SD_Filestructure)){
					menustructure_file_stepMenu(direction);
				}

			}
#ifdef DBG
			printf("Stepping DOWN! \r\n");
#endif
			menu_enable = 0x01;
		} else if (direction == OLED_MENU_STEP_BACK){
			// Stepping back (pointerwise: up)
			menuItem_t* tmp = menu_currentRef;
			if(tmp !=0x00 && tmp->fptr==0x00){
				// look for the item that is selected
				// set that item as the parent menu item
				while(tmp->leftMenuItem!=0x00){
					tmp = tmp->leftMenuItem;
				}

				while(tmp->selected==0x00){
					tmp = tmp->rightMenuItem;
				}

				if(tmp->upperMenuItem!=0x00){
					menu_currentRef = tmp->upperMenuItem;
				}

				menu_enable = 0x01;
			} else if(menu_currentRef->fptr != 0x00){
				// check if the functionpointer is filled in
				if((menu_currentRef->fptr) == (menustructure_menuFunction_SD_Filestructure)){
					menustructure_file_stepMenu(direction);
				} else if ( ((menu_currentRef->fptr) == (menustructure_menuFunction_AFE_Record)) && afe_enableRecording!=0x00 ){
					// do nothing
				} else {
					// look for the item that is selected
					// set that item as the parent menu item
					while(tmp->leftMenuItem!=0x00){
						tmp = tmp->leftMenuItem;
					}

					while(tmp->selected==0x00){
						tmp = tmp->rightMenuItem;
					}

					if(tmp->upperMenuItem!=0x00){
						menu_currentRef = tmp->upperMenuItem;
					}

					menu_enable = 0x01;
				}
			}
#ifdef DBG
			printf("Stepping BACK! \r\n");
#endif

		} else if (direction == OLED_MENU_STEP_ENTER){
			// Stepping in (pointerwise: down)
			menuItem_t* tmp = menu_currentRef->lowerMenuItem;
			if(tmp !=0x00){
				// look for the item that is selected
				// set that item as the parent menu item
				while(tmp->selected==0x00){
					tmp = tmp->rightMenuItem;
				}
				menu_currentRef = tmp;

				if((tmp->fptr) == (menustructure_menuFunction_SD_Filestructure)){
					file_Filestructure_Entered = 0x01;
				}

				menu_enable = 0x01;
			} else if(menu_currentRef->fptr != 0x00){
				// check if the functionpointer is filled in
				if((menu_currentRef->fptr) == (menustructure_menuFunction_SD_Filestructure)){
					menustructure_file_stepMenu(direction);
				} else if((menu_currentRef->fptr) == (menustructure_menuFunction_AFE_Record)){
					if(afe_enableRecording!=0x00){
						afe_stopRecording();
					} else {
						afe_startRecording();
					}
				}

			}
#ifdef DBG
			printf("Stepping ENTER! \r\n");
#endif
		}
	}
	lastMoved = delay_getMillis();
}

void
menustructure_file_stepMenu(uint8_t direction){
fileStructure_t* tmp = file_currentRef;
	if(direction == OLED_MENU_STEP_UP){
		// stepping up
			// just move the current selection bit.
		if(tmp->lowerFileItem!=0x00){
			// go down hierarchically
			tmp = tmp->lowerFileItem;
			// loop to the selected item
			while(tmp->selected==0x00){
				tmp = tmp->rightFileItem;
			}
			// got the selected item
			if(tmp->leftFileItem!=0x00){ // check for the first item
				// set the left item to selected
				tmp->leftFileItem->selected = 0x01;
				// set the current selected item unselected
				tmp->selected = 0x00;
			}
		}
	} else if(direction == OLED_MENU_STEP_DOWN){
		// stepping down
			// just move the current selection bit.
		if(tmp->lowerFileItem!=0x00){
			// go down hierarchically
			tmp = tmp->lowerFileItem;
			// loop to the selected item
			while(tmp->selected==0x00){
				tmp = tmp->rightFileItem;
			}
			// got the selected item
			if(tmp->rightFileItem!=0x00){ // check for the last item
				// set the left item to selected
				tmp->rightFileItem->selected = 0x01;
				// set the current selected item unselected
				tmp->selected = 0x00;
			}
		}
	} else if(direction == OLED_MENU_STEP_BACK){
		// stepping back
			// leave the selection bits. Just move the viewing point a level up
		if(tmp->upperFileItem!=0x00){	// check if the upper item is not empty
			file_currentRef = tmp->upperFileItem;
		} else {
			// we are at the top of the hierarchy.
			// now if you want to go back, you need to let the menu structure take over.
			if(menu_currentRef->upperMenuItem!=0x00){
				menu_currentRef = menu_currentRef->upperMenuItem;
			}
		}
	} else if(direction == OLED_MENU_STEP_ENTER){
		// stepping in
			// leave the selection bits. Just move the viewing point a level down
		if(tmp->lowerFileItem!=0x00){	// check if the lower item is not empty
			// move the pointer a level down
			tmp = tmp->lowerFileItem;
			// find the selected item
			while(tmp->selected!=0x00){ // loop over every item until it is selected
				// assign the right item
				tmp = tmp->rightFileItem;
			}
			// assign this item to the top of the renders
			file_currentRef = tmp->upperFileItem;
		}
	}
	menu_enable = 0x01;
}

fileStructure_t*
menustructure_file_addItem(fileStructure_t* upperFileItem, char* label,
		uint32_t size, uint8_t folder, uint8_t attrib){
	fileStructure_t* tmp = calloc(1,sizeof(fileStructure_t));
	if(tmp!=0x00){
		//tmp->upperFileItem = upperFileItem;
		tmp->label = label;
		tmp->size = size;
		tmp->DIR = ((attrib & AM_DIR) ? 'D' : '-');
		tmp->RDO = ((attrib & AM_RDO) ? 'R' : '-');
		tmp->SYS = ((attrib & AM_SYS) ? 'S' : '-');
		tmp->HID = ((attrib & AM_HID) ? 'H' : '-');
		tmp->menuLevel = upperFileItem->menuLevel+1;
		tmp->fileID = ++file_fileID;
		// attach the current new item to the structure
		menustructure_file_attachMenuItem(upperFileItem, tmp);
	}
	return tmp;
}

void
menustructure_file_insertItem(void){

}

void
menustructure_file_showMenu(fileStructure_t* parentItem, xypair_t xy0, xypair_t xy1, struct FONT_DEF font){
	// showing menu
	xycorners_t corners = ssd1306_getAdjustedPoints(xy0.x, xy0.y, xy1.x, xy1.y);


	// now visualize the structure
		uint32_t totalLines = corners.topLeft.y - corners.bottomLeft.y + font.u8Height+1;
		totalLines /= (font.u8Height+1);
		fileStructure_t* tmp = parentItem;
		uint32_t scrollCounter = 1;
		uint32_t totalCounter;
		corners.topLeft.y -= OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS;
		if(tmp->lowerFileItem != 0x00){	// check if the item lower is not empty
			// move down to a level that you want to visualize
			tmp = tmp->lowerFileItem;

			// loop over the items until there is one selected
			while(tmp!=0x00 && tmp->selected==0x00){
				tmp = tmp->rightFileItem;
				scrollCounter++;
			}
			totalCounter = scrollCounter;
			while(tmp->rightFileItem!=0x00){
				tmp = tmp->rightFileItem;
				totalCounter++;
			}

			// reset to the top
			tmp = parentItem->lowerFileItem;

			// set the start of the menu writing thing
			uint32_t startItem = scrollCounter;
			if(startItem<=totalLines){
				startItem = 1;
			}else{
				startItem -=totalLines-1;
			}


			// set the correct starting point
			for(uint32_t i=1; i<startItem;i++){
				if(tmp->rightFileItem !=0x00){
					tmp = tmp->rightFileItem;
				}
			}

			// write the lines
			for(uint32_t i=0; i<(MIN(totalLines, totalCounter)); i++){
				corners.topLeft.y = menustructure_file_showMenuItem(tmp, corners.topLeft.y, corners.topLeft.x, corners.topRight.x, font);
				if(tmp->rightFileItem!=0x00){
					tmp = tmp->rightFileItem;
				}

			}

		} else {
			// no lower item.
			// it's possible to print out the item here!
		}

}

uint8_t
menustructure_file_showMenuItem(fileStructure_t* fileItem, uint8_t y, uint8_t x0, uint8_t x1, struct FONT_DEF font){
	xypair_t tmp;
	// set the correct y point
	//y -= font.u8Height-1;
	tmp.x = x0;
	// determine if the item is selected
	if(fileItem->selected == 0x00){
		// not selected
		char buff[5];
		memset(buff,0,sizeof(buff));
		buff[0] = fileItem->DIR;
		buff[1] = fileItem->RDO;
		buff[2] = fileItem->SYS;
		buff[3] = fileItem->HID;
		tmp.x += 3;
		tmp = ssd1306_setString(tmp.x, y, buff, font);
		tmp.x += 6;
		tmp = ssd1306_setString(tmp.x, y, fileItem->label, font);
		tmp.x += 3;
		tmp = ssd1306_setString(tmp.x, y, general_dec32(fileItem->size), font);
		y = tmp.y;
		y -= font.u8Height+1;
	} else {
		// selected
		char buff[5];
		memset(buff,0,sizeof(buff));
		buff[0] = fileItem->DIR;
		buff[1] = fileItem->RDO;
		buff[2] = fileItem->SYS;
		buff[3] = fileItem->HID;
		ssd1306_setArea(tmp.x,y+1,OLED_TEXTBLOCK_RIGHTUP_X,y+font.u8Height+2);
		tmp.x += 3;
		tmp = ssd1306_setStringInverted(tmp.x, y, buff, font, 0);
		tmp.x += 6;
		tmp = ssd1306_setStringInverted(tmp.x, y, fileItem->label, font, 0);
		tmp.x += 3;
		tmp = ssd1306_setStringInverted(tmp.x, y, general_dec32(fileItem->size), font, 0);
		y = tmp.y;
		y -= font.u8Height+1;
	}
	return y;
}

static inline void
menustructure_file_attachMenuItem(fileStructure_t* sourceFile, fileStructure_t* attachFile){
	// sourceitem is for example an item from lvl one.
	// check if lowermenuitem is not null
		// if it is null-> assign attachitem to it
		// it it's not null-> go down the lower item and search for the last item

	// reference the upper menu item
	attachFile->upperFileItem = sourceFile;


	if (sourceFile->lowerFileItem == 0x00){
		sourceFile->lowerFileItem = attachFile;
		attachFile->selected = 0x01;
	} else {
		fileStructure_t* tmp = sourceFile->lowerFileItem;
		while(tmp->rightFileItem!=0x00){
			tmp = tmp->rightFileItem;
		}
		tmp->rightFileItem = attachFile;
		attachFile->leftFileItem = tmp;
	}

}

void
menustructure_file_deleteMenu(fileStructure_t* parentItem){

}

void
menustructure_file_fillMenu(void){
	//menustructure_file_addItem(&file_MAIN, "FILE1", 200, 0, (AM_DIR)|(AM_RDO)|(AM_SYS)|(AM_HID));
	path = "";

  		res = f_opendir(&dir, path);


  		if (res != FR_OK){
#ifdef DBG
  			printf("res = %d f_opendir\r\n", res);
#endif
  			return;
  		}



  		if (res == FR_OK)
  		{
  			while(1)
  			{
  				char *fn;

  				res = f_readdir(&dir, &fno);


  				if (res != FR_OK){
#ifdef DBG
  					printf("res = %d f_readdir\r\n", res);
#endif
  					return;
  				}

  				if ((res != FR_OK) || (fno.fname[0] == 0))
  					break;

  #if _USE_LFN
  				fn = *fno.lfname ? fno.lfname : fno.fname;
  #else
  				fn = fno.fname;
  #endif

  #ifdef DBG
  				printf("%c%c%c%c ",
  					((fno.fattrib & AM_DIR) ? 'D' : '-'),
  					((fno.fattrib & AM_RDO) ? 'R' : '-'),
  					((fno.fattrib & AM_SYS) ? 'S' : '-'),
  					((fno.fattrib & AM_HID) ? 'H' : '-') );

  				printf("%10ld ", (unsigned long)fno.fsize);

  				printf("%s/%s\r\n", path, fn);
  #endif

				//add item to structure

				char* tmp = calloc(strlen(fn),sizeof(char));


				//res = f_write(&fil, str, strlen(str), &BytesWritten);
				if(tmp != 0x00){
					strcpy(tmp, fn);
					menustructure_file_addItem(&file_MAIN, tmp, fno.fsize, 0, fno.fattrib);
				}

          }

  			}
}

void
menustructure_file_updateMenu(void){
	// delete previous file menu
	fileStructure_t* tmp = file_MAIN.lowerFileItem;
	if(tmp!=0x00){
		while(tmp->rightFileItem!=0x00){
			tmp = tmp->rightFileItem;
		}
		while(tmp->leftFileItem!=0x00){
			tmp = tmp->leftFileItem;
			free(tmp->rightFileItem);
		}
		tmp = file_MAIN.lowerFileItem;
		free(tmp);
		file_MAIN.lowerFileItem = 0x00;
		menustructure_file_fillMenu();
	} else {
		menustructure_file_fillMenu();
	}

}

// SD CARD ROUTINES
void
menustructure_menuFunction_SD_CardTest(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);

	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"CARDTEST",
			OLED_SUBMENUWRITING_HEADER_FONT);
}

void
menustructure_menuFunction_SD_Cardinfo(void){
	xypair_t tmp;

	if(sd_busy != 0x00){
		return;
	}

	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);

	sd_busy = 0x01;

	// get information

	memset(&cardInfo, 0, sizeof(SD_CardInfo));

	SD_Error result = SD_InitializeCards();
	result = SD_GetCardInfo(&cardInfo);

	sd_busy = 0x00;

	sdio_printCardInfo(&cardInfo);

	// header
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"CARD INFO",
			OLED_SUBMENUWRITING_HEADER_FONT);

	if(result == SD_OK){
		// show sd card size
			// print the subitem
			tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
			tmp = ssd1306_setStringBelowPreviousDifferentFont(tmp,
					OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
					"CARD SIZE: ",
					OLED_SUBMENUWRITING_HEADER_FONT,
					OLED_SUBMENUWRITING_SUBITEMS_FONT);
			// print the size
			uint64_t cc = cardInfo.SD_csd.DeviceSize/(1024/cardInfo.CardBlockSize);
			uint8_t pow=1;
			while(cc>1000){
				cc = cc/1000;
				pow++;
			}
			uint32_t total = (uint32_t)cc;
			char mib[] = "MB";
			char gib[] = "GB";

			tmp = ssd1306_setString(tmp.x,tmp.y,general_dec32(total), OLED_SUBMENUWRITING_SUBITEMS_FONT);
			switch(pow){
			case 1: tmp = ssd1306_setString(tmp.x,tmp.y,mib, OLED_SUBMENUWRITING_SUBITEMS_FONT); break;
			case 2: tmp = ssd1306_setString(tmp.x,tmp.y,gib, OLED_SUBMENUWRITING_SUBITEMS_FONT); break;
			}

		// show sd card available size
			tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
			tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
					OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
					"BLOCK SIZE: ",
					OLED_SUBMENUWRITING_SUBITEMS_FONT);
			tmp = ssd1306_setString(tmp.x,tmp.y,general_dec32(cardInfo.CardBlockSize), OLED_SUBMENUWRITING_SUBITEMS_FONT);

		// show sd card mounted
		tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
		tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
				OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
				"CARD TYPE: ",
				OLED_SUBMENUWRITING_SUBITEMS_FONT);
		switch(cardInfo.CardType){
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:
			tmp = ssd1306_setString(tmp.x,tmp.y,"STD CARD V1.1", OLED_SUBMENUWRITING_SUBITEMS_FONT);
			break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:
			tmp = ssd1306_setString(tmp.x,tmp.y,"STD CARD V2.0", OLED_SUBMENUWRITING_SUBITEMS_FONT);
			break;
		case SDIO_HIGH_CAPACITY_SD_CARD:
			tmp = ssd1306_setString(tmp.x,tmp.y,"SDHC", OLED_SUBMENUWRITING_SUBITEMS_FONT);
			break;
		case SDIO_MULTIMEDIA_CARD:
			tmp = ssd1306_setString(tmp.x,tmp.y,"MMC", OLED_SUBMENUWRITING_SUBITEMS_FONT);
			break;
		case SDIO_SECURE_DIGITAL_IO_CARD:
			tmp = ssd1306_setString(tmp.x,tmp.y,"SDIO CARD", OLED_SUBMENUWRITING_SUBITEMS_FONT);
			break;
		case SDIO_HIGH_SPEED_MULTIMEDIA_CARD:
			tmp = ssd1306_setString(tmp.x,tmp.y,"HSMMC", OLED_SUBMENUWRITING_SUBITEMS_FONT);
			break;
		case SDIO_SECURE_DIGITAL_IO_COMBO_CARD:
			tmp = ssd1306_setString(tmp.x,tmp.y,"SDIO COMBO", OLED_SUBMENUWRITING_SUBITEMS_FONT);
			break;
		case SDIO_HIGH_CAPACITY_MMC_CARD:
			tmp = ssd1306_setString(tmp.x,tmp.y,"HCMMC", OLED_SUBMENUWRITING_SUBITEMS_FONT);
			break;

		}

		// show sd card drive number
		tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
		tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
				OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
				"SER. NR: ",
				OLED_SUBMENUWRITING_SUBITEMS_FONT);
		tmp = ssd1306_setString(tmp.x,tmp.y,general_dec32(cardInfo.SD_cid.ProdSN), OLED_SUBMENUWRITING_SUBITEMS_FONT);

		// show sd card drive number
		tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
		tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
				OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
				"MAX. FREQ: ",
				OLED_SUBMENUWRITING_SUBITEMS_FONT);
		tmp = ssd1306_setString(tmp.x,tmp.y,general_dec32(cardInfo.SD_csd.MaxBusClkFrec), OLED_SUBMENUWRITING_SUBITEMS_FONT);
		tmp = ssd1306_setString(tmp.x,tmp.y," MHZ", OLED_SUBMENUWRITING_SUBITEMS_FONT);



		// show sd card drive number
		tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X+66;
		tmp.y = OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1;
		tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
				OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
				"MAX. W. BLOCK: ",
				OLED_SUBMENUWRITING_SUBITEMS_FONT);
		tmp = ssd1306_setString(tmp.x,tmp.y,general_dec32(cardInfo.SD_csd.MaxWrBlockLen), OLED_SUBMENUWRITING_SUBITEMS_FONT);

		// show sd card drive number
		tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X+66;
		tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
				OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
				"MAX. R. BLOCK: ",
				OLED_SUBMENUWRITING_SUBITEMS_FONT);
		tmp = ssd1306_setString(tmp.x,tmp.y,general_dec32(cardInfo.SD_csd.RdBlockLen), OLED_SUBMENUWRITING_SUBITEMS_FONT);
	} else {
		// print error message
		tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
		tmp = ssd1306_setStringBelowPreviousDifferentFont(tmp,
				OLED_SUBMENUWRITING_SPACINGPIXELS_FROMHEADER,
				"ERROR CONNECTING SD CARD",
				OLED_SUBMENUWRITING_HEADER_FONT,
				OLED_SUBMENUWRITING_SUBITEMS_FONT);
	}
}

void
menustructure_menuFunction_SD_Filestructure(void){
	if(sd_busy != 0x00){
		ssd1306_updateLater_100ms = 0x01;
		return;
	}
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	sd_busy = 0x01;


	xypair_t leftup, rightdown;
	leftup = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"FILESTRUCTURE",
			OLED_SUBMENUWRITING_HEADER_FONT);
	leftup.x = OLED_TEXTBLOCK_LEFTDOWN_X;
	leftup.y -= OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1;

	rightdown.x = OLED_TEXTBLOCK_RIGHTUP_X;
	rightdown.y = OLED_TEXTBLOCK_LEFTDOWN_X;

	// check if there is an existing file structure and make it updated
		if(file_Filestructure_Entered == 0x01 || file_MAIN.lowerFileItem == 0x00){	// file structure is empty
			file_currentRef = &file_MAIN;
			menustructure_file_updateMenu();
			file_Filestructure_Entered = 0x00;
		}

	menustructure_file_showMenu(file_currentRef, leftup, rightdown, OLED_MENUWRITING_FONT);

	menu_enable = 0x01;
	sd_busy = 0x00;
}

void
menustructure_menuFunction_SD_Sdsettings(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"CARD SETTINGS",
			OLED_SUBMENUWRITING_HEADER_FONT);
}

// RTC ROUTINES

void
menustructure_menuFunction_RTC_Rtcinfo(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"RTC INFO",
			OLED_SUBMENUWRITING_HEADER_FONT);

	// subseconds
	// sync prediv
	// asynch prediv
	// alarms set
	// clocksource
}

void
menustructure_menuFunction_RTC_Adjustrtc(void){
	xypair_t tmp;
	char rtc_date[10+1];
	char rtc_time[8+1];
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);

	// print the header
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"ADJUST RTC",
			OLED_SUBMENUWRITING_HEADER_FONT);

	// current DATE
	tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
	tmp = ssd1306_setStringBelowPreviousDifferentFont(tmp,
			OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
			"CURRENT DATE: ",
			OLED_SUBMENUWRITING_HEADER_FONT,
			OLED_SUBMENUWRITING_SUBITEMS_FONT);


	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	sprintf(rtc_date,"%02d/%02d/%04d",RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);

	tmp = ssd1306_setString(tmp.x,
			tmp.y,
			rtc_date,
			OLED_SUBMENUWRITING_SUBITEMS_FONT);

	// current TIME
	tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
	tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
			OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
			"CURRENT TIME: ",
			OLED_SUBMENUWRITING_SUBITEMS_FONT);


	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	sprintf(rtc_time, "%02d:%02d:%02d",RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);

	tmp = ssd1306_setString(tmp.x,
			tmp.y,
			rtc_time,
			OLED_SUBMENUWRITING_SUBITEMS_FONT);

	// new DATE
	tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
	tmp = ssd1306_setStringBelowPreviousDifferentFont(tmp,
			OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
			"NEW DATE: ",
			OLED_SUBMENUWRITING_HEADER_FONT,
			OLED_SUBMENUWRITING_SUBITEMS_FONT);


	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure_new);
	sprintf(rtc_date,"%02d/%02d/%04d",RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);

	tmp = ssd1306_setString(tmp.x,
			tmp.y,
			rtc_date,
			OLED_SUBMENUWRITING_SUBITEMS_FONT);

	// new TIME
	tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
	tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
			OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
			"NEW TIME: ",
			OLED_SUBMENUWRITING_SUBITEMS_FONT);


	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure_new);
	sprintf(rtc_time, "%02d:%02d:%02d",RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);

	tmp = ssd1306_setString(tmp.x,
			tmp.y,
			rtc_time,
			OLED_SUBMENUWRITING_SUBITEMS_FONT);

}

void
menustructure_menuFunction_RTC_Rtcsettings(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"RTC SETTINGS",
			OLED_SUBMENUWRITING_HEADER_FONT);
}

// USB ROUTINES
void
menustructure_menuFunction_USB_Test(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"USB TEST",
			OLED_SUBMENUWRITING_HEADER_FONT);
}
void
menustructure_menuFunction_USB_Settings(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"USB SETTINGS",
			OLED_SUBMENUWRITING_HEADER_FONT);
}

void
menustructure_menuFunction_USB_Info(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"USB INFO",
			OLED_SUBMENUWRITING_HEADER_FONT);
}

// AFE ROUTINES
void
menustructure_menuFunction_AFE_Settings(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"AFE SETTINGS",
			OLED_SUBMENUWRITING_HEADER_FONT);
}

void
menustructure_menuFunction_AFE_Info(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"AFE INFO",
			OLED_SUBMENUWRITING_HEADER_FONT);
}

void
menustructure_menuFunction_AFE_Record(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);

	// write the header
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"AFE RECORDING",
			OLED_SUBMENUWRITING_HEADER_FONT);

	// change recording setting
	tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
	tmp.y -= 11;

	tmp = ssd1306_setStringWithBorder(17,
			tmp.y,
			(afe_enableRecording!=0x00)?("STOP RECORDING"):("START RECORDING"),
			OLED_MENUWRITING_FONT,
			2);
	tmp.y -= 3;

	// current status
	tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
	tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
			OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
			"CURRENT STATUS: ",
			OLED_SUBMENUWRITING_SUBITEMS_FONT);

	tmp = ssd1306_setString(tmp.x,
			tmp.y,
			(afe_enableRecording!=0x00)?("RECORDING"):((afe_enableRecording!=0x01)?("IDLE"):("FINISHED")),
			OLED_SUBMENUWRITING_SUBITEMS_FONT);

	// folder name
	tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
	tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
			OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
			"LAST FILENAME: ",
			OLED_SUBMENUWRITING_SUBITEMS_FONT);

	tmp = ssd1306_setString(tmp.x,
			tmp.y,
			(char*)afe_writingFileString,
			OLED_SUBMENUWRITING_SUBITEMS_FONT);
	if(strcmp((char*)afe_writingFileString, "") == 0){
		tmp.y -= OLED_SUBMENUWRITING_SUBITEMS_FONT.u8Height+2;
	}


	// captured packets
	tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
	tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
			OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
			"CAPTURED PACKETS: ",
			OLED_SUBMENUWRITING_SUBITEMS_FONT);

	tmp = ssd1306_setString(tmp.x,
			tmp.y,
			general_dec32(afe_recordingPacketCount),
			OLED_SUBMENUWRITING_SUBITEMS_FONT);

	// capture time
		tmp.x = OLED_TEXTBLOCK_LEFTDOWN_X;
		tmp = ssd1306_setStringBelowPreviousSameFont(tmp,
				OLED_SUBMENUWRITING_SPACINGPIXELS_FROMSUBITEMS,
				"CAPTURE TIME: ",
				OLED_SUBMENUWRITING_SUBITEMS_FONT);

		// empty the time structure and chop it up
		rtc_setTimestructure((timeRegistration_t*)&afe_recordedTime, afe_recordingTime);
		// concatenate the correct time ; be aware that this function creates space. needs to be freed
		char* tmp_holdingString = rtc_getTimestructureString((timeRegistration_t*)&afe_recordedTime);
		// set it to the display
		tmp = ssd1306_setString(tmp.x,
				tmp.y,
				tmp_holdingString,
				OLED_SUBMENUWRITING_SUBITEMS_FONT);
		// empty the buffer
		free(tmp_holdingString);
		// update the screen every second if it's in writing mode
		if(ssd1306_updateLater_1s == 0x00 && afe_enableRecording != 0x00){
			ssd1306_updateLater_1s = 0x01;
		}
}

// TERMINAL ROUTINES

void
menustructure_menuFunction_TERMINAL_Info(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"TERM. INFO",
			OLED_SUBMENUWRITING_HEADER_FONT);
}

void
menustructure_menuFunction_TERMINAL_Test(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"TERM TEST",
			OLED_SUBMENUWRITING_HEADER_FONT);
}

void
menustructure_menuFunction_TERMINAL_Output(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"TERM VIEW",
			OLED_SUBMENUWRITING_HEADER_FONT);
}

void
menustructure_menuFunction_TERMINAL_Settings(void){
	xypair_t tmp;
	// clear the screen
	ssd1306_clearArea(OLED_MENUWRITING_START, OLED_MENUWRITING_END);
	tmp = ssd1306_setStringCentered(OLED_TEXTBLOCK_RIGHTUP_Y-OLED_SUBMENUWRITING_HEADER_FONT.u8Height-1,
			OLED_TEXTBLOCK_LEFTDOWN_X,
			OLED_TEXTBLOCK_RIGHTUP_X,
			"TERM SETTINGS",
			OLED_SUBMENUWRITING_HEADER_FONT);
}
