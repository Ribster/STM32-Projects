/*
 * SCREEN.h
 *
 *  Created on: 3-jan.-2015
 *      Author: Robbe
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "IO.h"
#include "IOPin.h"
#include "SPI.h"
#include "DELAY.h"
#include "UART.h"

#include "Resources/ASCII chars 8x5.h"

namespace stm32lib {

using namespace std;

typedef enum SCREEN_SSD1306_CONFIG_e{
	SCREEN_SSD1306_CONFIG_NOCONFIG,
	SCREEN_SSD1306_CONFIG_I2C,
	SCREEN_SSD1306_CONFIG_3SPI,
	SCREEN_SSD1306_CONFIG_4SPI,
	SCREEN_SSD1306_CONFIG_8Bit_6800,
	SCREEN_SSD1306_CONFIG_8Bit_8080
} SCREEN_SSD1306_CONFIG_t;

typedef struct SCREEN_SSD1306_s{
	SCREEN_SSD1306_CONFIG_t configuration;
	IO_t* pin_RST;
	IO_t* pin_DC;
	IO_t* pin_LED = 0;
	SPI_t* SPIStruct;
	IO_t* D0;
	IO_t* D1;
	IO_t* D2;
	IO_t* D3;
	IO_t* D4;
	IO_t* D5;
	IO_t* D6;
	IO_t* D7;
} SCREEN_SSD1306_t;

typedef enum SCREEN_SSD1306_WRITEMODE_e{
	SCREEN_SSD1306_WRITEMODE_DATA,
	SCREEN_SSD1306_WRITEMODE_REGISTER
} SCREEN_SSD1306_WRITEMODE_t;

typedef enum SCREEN_SSD1306_COMMANDS_e{
	// XXXX XXXX are masking bits
	// **** **** are don't care bits

	//	Fundamental Commands
		// 1st byte= Contrast command
		// 2nd byte= value XXXX XXXX
		SCREEN_SSD1306_COMMANDS_BASE_Contrast_1of2 = 0x81,
		// Output follows RAM content
		SCREEN_SSD1306_COMMANDS_BASE_DisplayOn_UseRam_1of1 = 0xA4,
		// Output ignores RAM content
		SCREEN_SSD1306_COMMANDS_BASE_DisplayOn_IgnoreRam_1of1 = 0xA5,
		// 0 in RAM, Off in Display
		SCREEN_SSD1306_COMMANDS_BASE_SetNormal_1of1 = 0xA6,
		// 1 in RAM, Off in Display
		SCREEN_SSD1306_COMMANDS_BASE_SetInverse_1of1 = 0xA7,
		SCREEN_SSD1306_COMMANDS_BASE_DisplayOff_1of1 = 0xAE,
		SCREEN_SSD1306_COMMANDS_BASE_DisplayOn_1of1 = 0xAF,

	//	Scrolling Commands
		// 1st byte= select right / left hor. scroll
		// 2nd byte= dummy byte 0x00
		// 3rd byte= define start page address ; **** *XXX
		// 4th byte= time between each scroll step; **** *XXX
		// 5th byte= end page address; **** *XXX
		// 6th byte= dummy byte; 0x00
		// 7th byte= dummy byte; 0xFF
			// Right Horizontal Scroll
			SCREEN_SSD1306_COMMANDS_SCROLL_RightHorizontalScroll_1of7 = 0x26,
			// Left Horizontal Scroll
			SCREEN_SSD1306_COMMANDS_SCROLL_LeftHorizontalScroll_1of7 = 0x27,
		// Continuous vertical & horizontal scroll setup
			SCREEN_SSD1306_COMMANDS_SCROLL_ContinuousVerticalAndRightScroll_1of6 = 0x29,
			SCREEN_SSD1306_COMMANDS_SCROLL_ContinuousVerticalAndLeftScroll_1of6 = 0x2A,
		// Deactivate Scroll
		SCREEN_SSD1306_COMMANDS_SCROLL_DeactivateScroll_1of1 = 0x2E,
		// Activate Scroll
		SCREEN_SSD1306_COMMANDS_SCROLL_ActivateScroll_1of1 = 0x2F,
		// Set Vertical Scroll Area
		SCREEN_SSD1306_COMMANDS_SCROLL_SetVerticalScrollArea_1of3 = 0xA3,

	//	Addressing Setting Commands
		// Set lower nibble of column start address for Page Addressing Mode; 0000 XXXX
		SCREEN_SSD1306_COMMANDS_ADDRESS_SetLowerColumnStartAddress_1of1 = 0x00,
		// Set higher nibble of column start address for Page Addressing Mode; 0001 XXXX
		SCREEN_SSD1306_COMMANDS_ADDRESS_SetHigherColumnStartAddress_1of1 = 0x10,
		// 1st byte: Set memory addressing mode
		// 2nd byte: **** **XX
			// 00: Horizontal
			// 01: Vertical
			// 10: Page Addressing Mode (RESET)
			// 11: Invalid
		SCREEN_SSD1306_COMMANDS_ADDRESS_SetMemoryAddressingMode_1of2 = 0x20,
		// 1st byte: Set column address
		// 2nd byte: Column start address; *XXX XXXX
		// 3rd byte: Column end address; *XXX XXXX
		SCREEN_SSD1306_COMMANDS_ADDRESS_SetColumnAddress_1of3 = 0x21,
		// 1st byte: Set page address
		// 2nd byte: Page start address; **** *XXX
		// 3rd byte: Page end address; **** *XXX
		SCREEN_SSD1306_COMMANDS_ADDRESS_SetPageAddress_1of3 = 0x22,
		// Set page start address for page addressing mode; 1011 0XXX
		SCREEN_SSD1306_COMMANDS_ADDRESS_SetPageStartAddress_1of1 = 0xB0,

	//	Hardware Configuration Commands	(Panel resolution && layout)
		// Set display start line; 01XX XXXX
		SCREEN_SSD1306_COMMANDS_HCONF_SetDisplayStartLine_1of1 = 0x40,
		// Set segment Re-map
		SCREEN_SSD1306_COMMANDS_HCONF_SetSegmentReMapCol0AsSeg0_1of1 = 0xA0,
		SCREEN_SSD1306_COMMANDS_HCONF_SetSegmentReMapCol127AsSeg0_1of1 = 0xA1,
		// 1st byte: Set Multiplex Ratio
		// 2nd byte: 16-64MUX
		SCREEN_SSD1306_COMMANDS_HCONF_SetMultiplexRatio_1of2 = 0xA8,
		// Set COM Output Scan Direction; 1100 *000
			// Normal Mode (RESET) scan from COM0 to COM[n-1]
			SCREEN_SSD1306_COMMANDS_HCONF_SetComDirectionNormal_1of1 = 0xC0,
			// Remapped mode. Rescan from COM[N-1] to COM0
			SCREEN_SSD1306_COMMANDS_HCONF_SetComDirectionInverted_1of1 = 0xC8,
		// 1st byte: Set Display Offset
		// 2nd byte: **XX XXXX vertical shift by COM from 0d~63d
		SCREEN_SSD1306_COMMANDS_HCONF_SetDisplayOffset_1of2 = 0xD3,
		// 1st byte: Set COM pins hardware config
		// 2nd byte: 00XX 0010
			//b4 == 0: sequential COM pin config
			//b4 == 1: alternative COM pin config
			//b5 == 0: disable COM left/right remap
			//b5 == 1: enable COM lef/right remap
		SCREEN_SSD1306_COMMANDS_HCONF_SetComPinsHardwareConfig_1of2 = 0xDA,

	//	Timing & Driving Scheme Settings
		// 1st byte: Set display clock divide ration / oscillator freq
		// 2nd byte: XXXX XXXX
			// [3:0] -> divide ratio -1
			// [7:4] -> Fosc
		SCREEN_SSD1306_COMMANDS_TDCONF_SetDisplayClockDivide_1of2 = 0xD5,
		// 1st byte: Set precharge period
		// 2nd byte: XXXX XXXX
			// [3:0] -> phase 1 period
			// [7:4] -> phase 2 period
		SCREEN_SSD1306_COMMANDS_TDCONF_SetPrechargePeriod_1of2 = 0xD9,
		// 1st byte: Set VCOMH deselect level
		// 2nd byte: 0XXX 0000
			// 000 -> 0.65 x VCC
			// 010 -> 0.77 x VCC (RESET)
			// 011 -> 0.83 x VCC
		SCREEN_SSD1306_COMMANDS_TDCONF_SetVCOMHDeselectLevel_1of2 = 0xDB,
		// 1st byte:
		SCREEN_SSD1306_COMMANDS_TDCONF_NOP_1of1 = 0xE3,

	//	Charge pump commands
		// Sequence:
			// 0x8D: charge pump setting
			// 0x14: enable charge pump
			// 0xAF: display ON
		// 1st byte: Charge pump setting
		// 2nd byte: **01 0X00
			// 0: disable charge pump (reset)
			// 1: enable charge pump during display on
		SCREEN_SSD1306_COMMANDS_CHRGPMP_SetChargepumpSetting_1of2 = 0x8D
} SCREEN_SSD1306_COMMANDS_t;




typedef enum SCREEN_TYPE_e{
	SCREEN_TYPE_NOSELECTION,
	SCREEN_TYPE_SSD1289,
	SCREEN_TYPE_SSD1306,
	SCREEN_TYPE_ILI9320,
	SCREEN_TYPE_ILI9341,
	SCREEN_TYPE_ST7735
} SCREEN_TYPE_t;

typedef enum SCREEN_MODE_e{
	SCREEN_MODE_INVERTED,
	SCREEN_MODE_NORMAL
} SCREEN_MODE_t;

typedef enum SCREEN_ALIGN_e{
	SCREEN_ALIGN_LEFTBOTTOM,
	SCREEN_ALIGN_LEFTMID,
	SCREEN_ALIGN_LEFTTOP,
	SCREEN_ALIGN_MIDBOTTOM,
	SCREEN_ALIGN_MIDMID,
	SCREEN_ALIGN_MIDTOP,
	SCREEN_ALIGN_RIGHTBOTTOM,
	SCREEN_ALIGN_RIGHTMID,
	SCREEN_ALIGN_RIGHTTOP
} SCREEN_ALIGN_t;

typedef enum SCREEN_POWERMODE_e{
	SCREEN_POWERMODE_AWAKE,
	SCREEN_POWERMODE_ASLEEP
} SCREEN_POWERMODE_t;

typedef enum SCREEN_ORIENTATION_e{
	SCREEN_ORIENTATION_LANDSCAPE,
	SCREEN_ORIENTATION_PORTRAIT
} SCREEN_ORIENTATION_t;

typedef enum SCREEN_COLORS_e{
	SCREEN_COLORS_RED,
	SCREEN_COLORS_GREEN,
	SCREEN_COLORS_BLUE,
	SCREEN_COLORS_YELLOW,
	SCREEN_COLORS_WHITE
} SCREEN_COLORS_t;

typedef struct SCREEN_s{
	SCREEN_TYPE_t ScreenType = SCREEN_TYPE_NOSELECTION;
	SCREEN_SSD1306_t* SSD1306Struct;
	SCREEN_MODE_t SCREENMode = SCREEN_MODE_NORMAL;
	SCREEN_POWERMODE_t SCREENPowerMode = SCREEN_POWERMODE_AWAKE;
	SCREEN_ORIENTATION_t SCREENOrientation = SCREEN_ORIENTATION_LANDSCAPE;
	uint16_t horpx = 0;
	uint16_t verpx = 0;
	uint8_t* pixBuffer;
	uint8_t basecolors = 1;
	uint8_t letterSpace = 0;
	bool externalVCC = false;
	pair<pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>> body;
	pair<pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>> header;
}SCREEN_t;

class SCREEN {
public:
  // ctor && destor
  SCREEN(IO* IOObject, DELAY* DELAYObject, SPI*, SCREEN_t* SCREENStructure, SCREEN_SSD1306_CONFIG_t, SPI_t*, IOPin* RST, IOPin* DC);
  SCREEN(IO* IOObject, DELAY* DELAYObject, SPI*, SCREEN_t* SCREENStructure, SCREEN_SSD1306_CONFIG_t, SPI_t*, IO_t* RST, IO_t* DC);
  SCREEN(IO* IOObject, DELAY* DELAYObject, SPI*, SCREEN_t* SCREENStructure, SCREEN_SSD1306_CONFIG_t, SPI_t*, IO_t* RST, IO_t* DC, IO_t* LED);
  virtual ~SCREEN();

  // initialization
  void initialize(void);

  // get internal struct variables
  SCREEN_MODE_t getMode(void);
  SCREEN_POWERMODE_t getPowerMode(void);
  SCREEN_ORIENTATION_t getOrientation(void);

  // set internal struct variables
  void setMode(SCREEN_MODE_t);
  void setPowerMode(SCREEN_POWERMODE_t);
  void setOrientation(SCREEN_ORIENTATION_t);

  // set internal buffer items
  void setPixel(pair<uint32_t, uint32_t>, bool);
  void setBuffer(bool);
    // characters
    pair<uint32_t,uint32_t> setCharacter(uint8_t ch, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t, uint8_t scaling);
    pair<uint32_t,uint32_t> setCharacter(uint8_t ch, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t, uint8_t scaling, SCREEN_MODE_t inverted);
    // strings
    pair<uint32_t,uint32_t> setString(string str, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t, uint8_t scaling);
    pair<uint32_t,uint32_t> setString(string str, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t, uint8_t scaling, SCREEN_MODE_t inverted);
    pair<uint32_t,uint32_t> setString(const char* str, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t, uint8_t scaling);
    pair<uint32_t,uint32_t> setString(const char* str, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t, uint8_t scaling, SCREEN_MODE_t inverted);
    // rectangles
    pair<uint32_t,uint32_t> setRectangle(pair<uint32_t, uint32_t> point, uint32_t dimx, uint32_t dimy, SCREEN_ALIGN_t);
    pair<uint32_t,uint32_t> setRectangle(pair<uint32_t, uint32_t> point, uint32_t dimx, uint32_t dimy, SCREEN_ALIGN_t, SCREEN_MODE_t inverted);
    pair<uint32_t,uint32_t> setRectangle(pair<uint32_t, uint32_t> begin, pair<uint32_t, uint32_t> end, SCREEN_ALIGN_t);
    pair<uint32_t,uint32_t> setRectangle(pair<uint32_t, uint32_t> begin, pair<uint32_t, uint32_t> end, SCREEN_ALIGN_t, SCREEN_MODE_t inverted);
    pair<uint32_t,uint32_t> setRectangle(pair<pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>> range, SCREEN_ALIGN_t);
    pair<uint32_t,uint32_t> setRectangle(pair<pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>> range, SCREEN_ALIGN_t, SCREEN_MODE_t inverted);
      // screens
      void setHeader(bool);
      void setBody(bool);
      void setScreen(bool);

  // get object information
  bool getInRange(pair<uint32_t, uint32_t>, pair<pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>>);
  bool getInScreen(pair<uint32_t, uint32_t>);
  pair<pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>> getScreenRange(void);
  bool getInHeader(pair<uint32_t, uint32_t>);
  pair<pair<uint32_t, uint32_t>, pair<uint32_t,uint32_t>> getHeaderRange(void);
  bool getInBody(pair<uint32_t, uint32_t>);
  pair<pair<uint32_t, uint32_t>, pair<uint32_t,uint32_t>> getBodyRange(void);
  pair<uint8_t,uint8_t> getCharSize(uint8_t scaling);
  pair<uint32_t,uint32_t> getReturnPointFromLeftBottomAlign(pair<uint32_t,uint32_t> pointLeftBottom, pair<uint8_t,uint8_t> size, SCREEN_ALIGN_t);
  pair<uint32_t,uint32_t> getReturnPointToLeftBottomAlign(pair<uint32_t,uint32_t> point, pair<uint8_t,uint8_t> size, SCREEN_ALIGN_t);
  pair<uint32_t,uint32_t> getCharacterPointWithShift(pair<uint32_t,uint32_t> point, pair<uint8_t,uint8_t> size);
  // write functions
  void writeBuffer(void);

  void writeClearScreen(SCREEN_MODE_t);
  void writeClearScreen(void);

  void writeHeader(bool);
  void writeBody(bool);

private:
  // fysical pin manipulations
  void setRST(bool);
  void setDC(bool);
  void setSS(bool);
  void setLED(bool);

  // debugging
  void printPrefix(void);

  // private pointers
  SCREEN_t* SCREENStruct_ = NULL;
  IO* IOObj_ = NULL;
  SPI* SPIObj_ = NULL;
  DELAY* DELAYObj_ = NULL;

  // internal struct functions
  uint32_t getSizePixelBuffer(void);

  // lower level transmission functions
  void transmitByte(SCREEN_SSD1306_COMMANDS_t, bool);
  void transmitByte(SCREEN_SSD1306_COMMANDS_t);
  void transmitByte(uint8_t data, bool);
  void transmitCommand(uint8_t data);
};

} /* namespace stm32lib */

#endif /* SCREEN_H_ */
