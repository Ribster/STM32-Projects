/*
 * SCREEN.cpp
 *
 *  Created on: 3-jan.-2015
 *      Author: Robbe
 */

#include "SCREEN.h"

namespace stm32lib {
 /*** PUBLIC FUNCTIONS ***/

// ctor && destor
  SCREEN::SCREEN(	IO* IOObject, DELAY* DELAYObject, SPI* SPIObject, SCREEN_t* SCREENStructure ,
				SCREEN_SSD1306_CONFIG_t SCREENConfig, SPI_t* SPIStructure, IOPin* RST, IOPin* DC) {
	if( !(SCREENConfig == SCREEN_SSD1306_CONFIG_3SPI || SCREENConfig == SCREEN_SSD1306_CONFIG_4SPI) ){
		exit(0);
	}

	// Fill in internal pointers
	IOObj_ = IOObject;
	DELAYObj_ = DELAYObject;
	SPIObj_ = SPIObject;
	SCREENStruct_ = SCREENStructure;
	// Set internal type
	SCREENStruct_->ScreenType = SCREEN_TYPE_SSD1306;
	// Make room for struct
	SCREENStruct_->SSD1306Struct = new SCREEN_SSD1306_t;
	// Error handling
	if(SCREENStruct_->SSD1306Struct == NULL){
		exit(4);
	}
	// Set data in struct
	SCREENStruct_->SSD1306Struct->configuration = SCREENConfig;
	SCREENStruct_->SSD1306Struct->SPIStruct = SPIStructure;
	SCREENStruct_->SSD1306Struct->pin_DC = DC->PIN;
	SCREENStruct_->SSD1306Struct->pin_RST = RST->PIN;
	SCREENStruct_->horpx = 128;
	SCREENStruct_->verpx = 64;

	SCREENStruct_->header.first.first = 0;
	SCREENStruct_->header.first.second = 48;
	SCREENStruct_->header.second.first = SCREENStruct_->horpx-1;
	SCREENStruct_->header.second.second = SCREENStruct_->verpx-1;

	SCREENStruct_->body.first.first = 0;
	SCREENStruct_->body.first.second = 0;
	SCREENStruct_->body.second.first = SCREENStruct_->horpx-1;
	SCREENStruct_->body.second.second = SCREENStruct_->header.first.second-1;

	SCREENStruct_->basecolors = 1;
		// Make pixelbuffer
	if(SCREENStruct_->pixBuffer != 0){
		free(SCREENStruct_->pixBuffer);
	}
	SCREENStruct_->pixBuffer = new uint8_t[getSizePixelBuffer()];
	if(SCREENStruct_->pixBuffer == 0){
		exit(1);
	}
  }


  SCREEN::SCREEN(IO* IOObject, DELAY* DELAYObject, SPI* SPIObject, SCREEN_t* SCREENStructure, SCREEN_SSD1306_CONFIG_t SCREENConfig, SPI_t* SPIStructure, IO_t* RST, IO_t* DC){
	if( !(SCREENConfig == SCREEN_SSD1306_CONFIG_3SPI || SCREENConfig == SCREEN_SSD1306_CONFIG_4SPI) ){
		exit(0);
	}

	// Fill in internal pointers
	IOObj_ = IOObject;
	DELAYObj_ = DELAYObject;
	SPIObj_ = SPIObject;
	SCREENStruct_ = SCREENStructure;
	// Set internal type
	SCREENStruct_->ScreenType = SCREEN_TYPE_SSD1306;
	// Make room for struct
	SCREENStruct_->SSD1306Struct = new SCREEN_SSD1306_t;
	// Error handling
	if(SCREENStruct_->SSD1306Struct == NULL){
		exit(4);
	}
	// Set data in struct
	SCREENStruct_->SSD1306Struct->configuration = SCREENConfig;
	SCREENStruct_->SSD1306Struct->SPIStruct = SPIStructure;
	SCREENStruct_->SSD1306Struct->pin_DC = DC;
	SCREENStruct_->SSD1306Struct->pin_RST = RST;
	SCREENStruct_->horpx = 128;
	SCREENStruct_->verpx = 64;

	SCREENStruct_->header.first.first = 0;
	SCREENStruct_->header.first.second = 48;
	SCREENStruct_->header.second.first = SCREENStruct_->horpx-1;
	SCREENStruct_->header.second.second = SCREENStruct_->verpx-1;

	SCREENStruct_->body.first.first = 0;
	SCREENStruct_->body.first.second = 0;
	SCREENStruct_->body.second.first = SCREENStruct_->horpx-1;
	SCREENStruct_->body.second.second = SCREENStruct_->header.first.second-1;

	SCREENStruct_->basecolors = 1;
		// Make pixelbuffer
	if(SCREENStruct_->pixBuffer != 0){
		free(SCREENStruct_->pixBuffer);
	}
	SCREENStruct_->pixBuffer = new uint8_t[getSizePixelBuffer()];
	if(SCREENStruct_->pixBuffer == 0){
		exit(1);
	}
  }

  SCREEN::SCREEN(IO* IOObject, DELAY* DELAYObject, SPI* SPIObject, SCREEN_t* SCREENStructure, SCREEN_SSD1306_CONFIG_t SCREENConfig, SPI_t* SPIStructure, IO_t* RST, IO_t* DC, IO_t* LED){

	if( !(SCREENConfig == SCREEN_SSD1306_CONFIG_3SPI || SCREENConfig == SCREEN_SSD1306_CONFIG_4SPI) ){
		exit(0);
	}

	// Fill in internal pointers
	IOObj_ = IOObject;
	DELAYObj_ = DELAYObject;
	SPIObj_ = SPIObject;
	SCREENStruct_ = SCREENStructure;
	// Set internal type
	SCREENStruct_->ScreenType = SCREEN_TYPE_SSD1306;
	// Make room for struct
	SCREENStruct_->SSD1306Struct = new SCREEN_SSD1306_t;
	// Error handling
	if(SCREENStruct_->SSD1306Struct == NULL){
		exit(4);
	}
	// Set data in struct
	SCREENStruct_->SSD1306Struct->configuration = SCREENConfig;
	SCREENStruct_->SSD1306Struct->SPIStruct = SPIStructure;
	SCREENStruct_->SSD1306Struct->pin_DC = DC;
	SCREENStruct_->SSD1306Struct->pin_RST = RST;
	SCREENStruct_->SSD1306Struct->pin_LED = LED;
	SCREENStruct_->horpx = 128;
	SCREENStruct_->verpx = 64;

	SCREENStruct_->header.first.first = 0;
	SCREENStruct_->header.first.second = 48;
	SCREENStruct_->header.second.first = SCREENStruct_->horpx-1;
	SCREENStruct_->header.second.second = SCREENStruct_->verpx-1;

	SCREENStruct_->body.first.first = 0;
	SCREENStruct_->body.first.second = 0;
	SCREENStruct_->body.second.first = SCREENStruct_->horpx-1;
	SCREENStruct_->body.second.second = SCREENStruct_->header.first.second-1;

	SCREENStruct_->basecolors = 1;
		// Make pixelbuffer
	if(SCREENStruct_->pixBuffer != 0){
		free(SCREENStruct_->pixBuffer);
	}
	SCREENStruct_->pixBuffer = new uint8_t[getSizePixelBuffer()];
	if(SCREENStruct_->pixBuffer == 0){
		exit(1);
	}
  }

  SCREEN::~SCREEN() {
	  // Clear generated Structs
	  if(SCREENStruct_->SSD1306Struct != 0){
		  free(SCREENStruct_->SSD1306Struct);
	  }
  }

// initialization
  void SCREEN::initialize(void){
  	SCREEN_TYPE_t type = SCREENStruct_->ScreenType;
  	if(type == SCREEN_TYPE_SSD1306){
  		if(SCREENStruct_->SSD1306Struct->SPIStruct->initDone == false){
  			printPrefix();
  			printf("init SPI not done correctly\r\n");
  			exit(1);
  		}
  		if(SCREENStruct_->SSD1306Struct->pin_RST->init == false){
  			printPrefix();
  			printf("init Reset pin not done correctly\r\n");
  			exit(2);
  		}
  		if(SCREENStruct_->SSD1306Struct->pin_DC->init == false){
  			printPrefix();
  			printf("init DC pin not done correctly\r\n");
  			exit(3);
  		}

  		setRST(true);
  		DELAYObj_->delayMilli(1);
  		setRST(false);
  		DELAYObj_->delayMilli(10);
  		setRST(true);
  		setDC(true);
  		setSS(true);

  		// display off
  		transmitCommand(SCREEN_SSD1306_COMMANDS_BASE_DisplayOff_1of1);
  		// clock divide set
  		transmitCommand(SCREEN_SSD1306_COMMANDS_TDCONF_SetDisplayClockDivide_1of2);
  		transmitCommand(0x80); // suggested ratio

  		// line #0
  		transmitCommand(SCREEN_SSD1306_COMMANDS_HCONF_SetDisplayStartLine_1of1 | 0x0);
  		// set chargepump
  		transmitCommand(SCREEN_SSD1306_COMMANDS_CHRGPMP_SetChargepumpSetting_1of2);
  		if(SCREENStruct_->externalVCC){
  			transmitCommand(0x10);
  		} else {
  			transmitCommand(0x14);
  		}
  		// set memory mode
  		transmitCommand(SCREEN_SSD1306_COMMANDS_ADDRESS_SetMemoryAddressingMode_1of2);
  		transmitCommand(0x00);	// swap horizontal vertical
  		/*
  		// set column address
  		transmitCommand(SCREEN_SSD1306_COMMANDS_ADDRESS_SetColumnAddress_1of3);
  		transmitCommand(0x00);
  		transmitCommand(0x7F);

  		// set page address
  		transmitCommand(SCREEN_SSD1306_COMMANDS_ADDRESS_SetPageAddress_1of3);
  		transmitCommand(0x00);
  		transmitCommand(0x7F);
  		*/
  		// segment remap
  		transmitCommand(SCREEN_SSD1306_COMMANDS_HCONF_SetSegmentReMapCol0AsSeg0_1of1 | 0x1);	// swap left / right
  		// Com output from high to SEG0
  		transmitCommand(SCREEN_SSD1306_COMMANDS_HCONF_SetComDirectionNormal_1of1);	// swap top bottom
  		// set com pins
  		transmitCommand(SCREEN_SSD1306_COMMANDS_HCONF_SetComPinsHardwareConfig_1of2);
  		transmitCommand(0x12);
  		// set multiplex ration
  		transmitCommand(SCREEN_SSD1306_COMMANDS_HCONF_SetMultiplexRatio_1of2);
  		transmitCommand(0x3F);
  		// set display offset
  		transmitCommand(SCREEN_SSD1306_COMMANDS_HCONF_SetDisplayOffset_1of2);
  		transmitCommand(0x00);
  		// set contrast
  		transmitCommand(SCREEN_SSD1306_COMMANDS_BASE_Contrast_1of2);
  		if(SCREENStruct_->externalVCC){
  			transmitCommand(0x9F);
  		} else {
  			transmitCommand(0xCF);
  		}
  		// set precharge
  		transmitCommand(SCREEN_SSD1306_COMMANDS_TDCONF_SetPrechargePeriod_1of2);
  		if(SCREENStruct_->externalVCC){
  			transmitCommand(0x22);
  		} else {
  			transmitCommand(0xF1);
  		}
  		// set VCOMH deselect level
  		transmitCommand(SCREEN_SSD1306_COMMANDS_TDCONF_SetVCOMHDeselectLevel_1of2);
  		transmitCommand(0x40);
  		// set on and use data in RAM
  		transmitCommand(SCREEN_SSD1306_COMMANDS_BASE_DisplayOn_UseRam_1of1);
  		// set normal state
  		transmitCommand(SCREEN_SSD1306_COMMANDS_BASE_SetNormal_1of1);



  		transmitCommand(SCREEN_SSD1306_COMMANDS_BASE_DisplayOn_1of1);

  		//transmitCommand(SCREEN_SSD1306_COMMANDS_BASE_DisplayOn_IgnoreRam_1of1);

  		setBuffer(false);
  		writeBuffer();
  	}
  }

// get internal struct variables
  SCREEN_MODE_t SCREEN::getMode(void){
  	return SCREENStruct_->SCREENMode;
  }
  SCREEN_POWERMODE_t SCREEN::getPowerMode(void){
  	return SCREENStruct_->SCREENPowerMode;
  }
  SCREEN_ORIENTATION_t SCREEN::getOrientation(void){
  	return SCREENStruct_->SCREENOrientation;
  }


// set internal struct variables
  void SCREEN::setMode(SCREEN_MODE_t newVal){
  	SCREENStruct_->SCREENMode = newVal;
  	if(newVal == SCREEN_MODE_INVERTED){
  		transmitCommand(SCREEN_SSD1306_COMMANDS_BASE_SetInverse_1of1);
  	} else {
  		transmitCommand(SCREEN_SSD1306_COMMANDS_BASE_SetNormal_1of1);
  	}
  }
  void SCREEN::setPowerMode(SCREEN_POWERMODE_t newVal){
  	SCREENStruct_->SCREENPowerMode = newVal;
  	if(newVal == SCREEN_POWERMODE_ASLEEP){
  		transmitCommand(SCREEN_SSD1306_COMMANDS_BASE_DisplayOff_1of1);
  		transmitCommand(SCREEN_SSD1306_COMMANDS_CHRGPMP_SetChargepumpSetting_1of2);
  		transmitCommand(0x10);
  	}else{
  		transmitCommand(SCREEN_SSD1306_COMMANDS_CHRGPMP_SetChargepumpSetting_1of2);
  		transmitCommand(0x14);
  		transmitCommand(SCREEN_SSD1306_COMMANDS_BASE_DisplayOn_1of1);
  	}
  }
  void SCREEN::setOrientation(SCREEN_ORIENTATION_t newVal){
  	SCREENStruct_->SCREENOrientation = newVal;
  }

// set internal buffer items
  void SCREEN::setPixel(pair<uint32_t, uint32_t> point, bool newVal){
      if(!getInScreen(point)){
	      return;
      }
      uint8_t* ptr = SCREENStruct_->pixBuffer;
      ptr += (point.first) + ( (point.second/8) * 128);

      if(newVal){
	      *(ptr) |= 1<<( point.second%8);
      } else {
	      *(ptr) &= ~(1<<( point.second%8));
      }
  }
  void SCREEN::setBuffer(bool newVal){
      memset(SCREENStruct_->pixBuffer, (newVal==true)?(0xFF):(0x00), getSizePixelBuffer());
  }
    // characters
      pair<uint32_t,uint32_t> SCREEN::setCharacter(uint8_t ch, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t alignment, uint8_t scaling){
	    return setCharacter(ch, point, alignment, scaling, SCREENStruct_->SCREENMode);
      }
      pair<uint32_t,uint32_t> SCREEN::setCharacter(uint8_t ch, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t alignment, uint8_t scaling, SCREEN_MODE_t inverted){
	    uint8_t xCount, yCount;
	    uint8_t cChar;
	    uint16_t arraypoint = (ch-0x20)*5;
	    pair<uint8_t,uint8_t> tmp = getCharSize(scaling);
	    pair<uint32_t,uint32_t> transformPoint = getReturnPointToLeftBottomAlign(point, tmp, alignment);
	    pair<uint32_t,uint32_t> returnPoint;

	    if(!getInScreen(make_pair(transformPoint.first+tmp.first, transformPoint.second+tmp.second))){
		return point;
	    }

	    for(xCount = 0; xCount<tmp.first; xCount++){
		    cChar = ASCII8x5[arraypoint+(xCount/(scaling+1))];
		    for(yCount=0; yCount<tmp.second; yCount++){
			    bool temp = (cChar & (1<<(yCount/(scaling+1))));
			    if(inverted == SCREEN_MODE_NORMAL){
				if(temp){
				    setPixel(make_pair(transformPoint.first+xCount, transformPoint.second+tmp.second-yCount-1), true );
				}
			    } else {
				if(temp){
				    setPixel(make_pair(transformPoint.first+xCount, transformPoint.second+tmp.second-yCount-1), false );
				}
			    }

		    }
	    }
	    // retransform to input alignment
	    returnPoint = getReturnPointFromLeftBottomAlign(transformPoint, tmp, alignment);
	    return getCharacterPointWithShift(returnPoint, tmp);
      }
    // strings
      pair<uint32_t,uint32_t> SCREEN::setString(string str, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t alignment, uint8_t scaling){
	return setString(str, point, alignment, scaling, SCREENStruct_->SCREENMode);
      }
      pair<uint32_t,uint32_t> SCREEN::setString(string str, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t alignment, uint8_t scaling, SCREEN_MODE_t inverted){
	return setString(str.c_str(), point, alignment, scaling, inverted);
      }
      pair<uint32_t,uint32_t> SCREEN::setString(const char* str, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t alignment, uint8_t scaling){
	return setString(str, point, alignment, scaling, SCREENStruct_->SCREENMode);
      }
      pair<uint32_t,uint32_t> SCREEN::setString(const char* str, pair<uint32_t, uint32_t> point, SCREEN_ALIGN_t alignment, uint8_t scaling, SCREEN_MODE_t inverted){
	pair<uint32_t,uint32_t> tmp = point;
      	for(uint16_t i=0; i<strlen(str); i++){
      	    tmp = setCharacter(*(str+i), tmp, alignment, scaling, inverted);
      	}
      	// remove extra spacing
      	tmp.first -= SCREENStruct_->letterSpace;
      	return tmp;
      }
    // rectangles
      pair<uint32_t,uint32_t> SCREEN::setRectangle(pair<uint32_t, uint32_t> point, uint32_t dimx, uint32_t dimy, SCREEN_ALIGN_t alignment){
	  return setRectangle(point, dimx, dimy, alignment, SCREENStruct_->SCREENMode);
      }
      pair<uint32_t,uint32_t> SCREEN::setRectangle(pair<uint32_t, uint32_t> point, uint32_t dimx, uint32_t dimy, SCREEN_ALIGN_t alignment, SCREEN_MODE_t inverted){
	// gets rectangle with point aligned to a certain alignment.
	// transformation happens so that the point is transformed to leftbottom
	// rectangle gets set
	// returns point of rectangle to the right.

	pair<uint32_t, uint32_t> parsePoint = getReturnPointToLeftBottomAlign(point, make_pair(dimx,dimy), alignment);
	uint32_t i,j;
	// check this part
	if(!getInScreen(parsePoint)){
	    pair<uint32_t,uint32_t> tmp = parsePoint;
	    tmp.first += dimx;
	    tmp.second += dimy;
	    if(!getInScreen(tmp)){
		return point;
	    }
	}

	for(i=point.first;i<point.first+dimx;i++){
	    for(j=point.second;j<point.second+dimy;j++){
		setPixel(make_pair(i,j), inverted);
	    }
	}
	return getReturnPointToLeftBottomAlign(point, make_pair(dimx,dimy), alignment);

      }
      pair<uint32_t,uint32_t> SCREEN::setRectangle(pair<uint32_t, uint32_t> begin, pair<uint32_t, uint32_t> end, SCREEN_ALIGN_t alignment){
	  return setRectangle(begin, end, alignment, SCREENStruct_->SCREENMode);
      }
      pair<uint32_t,uint32_t> SCREEN::setRectangle(pair<uint32_t, uint32_t> begin, pair<uint32_t, uint32_t> end, SCREEN_ALIGN_t alignment, SCREEN_MODE_t inverted){
	  return setRectangle(make_pair(begin.first, begin.second), end.first-begin.first, end.second-begin.second, alignment, inverted);
      }
      pair<uint32_t,uint32_t> SCREEN::setRectangle(pair<pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>> range, SCREEN_ALIGN_t alignment){
	return setRectangle(range, alignment, SCREENStruct_->SCREENMode);
      }
      pair<uint32_t,uint32_t> SCREEN::setRectangle(pair<pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>> range, SCREEN_ALIGN_t alignment, SCREEN_MODE_t inverted){
	return setRectangle(range.first, range.second, alignment, inverted);
      }
      // screens
      void SCREEN::setHeader(bool newVal){
	setRectangle(getHeaderRange(), SCREEN_ALIGN_LEFTBOTTOM, (newVal==true)?(SCREEN_MODE_NORMAL):(SCREEN_MODE_INVERTED) );
      }
      void SCREEN::setBody(bool newVal){
	setRectangle(getBodyRange(), SCREEN_ALIGN_LEFTBOTTOM, (newVal==true)?(SCREEN_MODE_NORMAL):(SCREEN_MODE_INVERTED) );
      }
      void SCREEN::setScreen(bool newVal){
	setRectangle(getScreenRange(), SCREEN_ALIGN_LEFTBOTTOM, (newVal==true)?(SCREEN_MODE_NORMAL):(SCREEN_MODE_INVERTED) );
      }

  // get object information
  bool SCREEN::getInRange(pair<uint32_t, uint32_t> point, pair<pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>> range){
    uint32_t xmatch = point.first;
    uint32_t xlow = range.first.first;
    uint32_t xhigh = range.second.first;

    uint32_t ymatch = point.second;
    uint32_t ylow = range.first.second;
    uint32_t yhigh = range.second.second;

    if ( (xmatch >= xlow && xmatch <xhigh) || (xmatch >= xhigh && xmatch <xlow) ){
    } else {
	return false;
    }




    if ( (ymatch >= ylow && ymatch <yhigh) || (ymatch >= yhigh && ymatch <ylow) ){
	return true;
    }

    return false;
  }
  bool SCREEN::getInScreen(pair<uint32_t, uint32_t> point){
    return getInRange(point, getScreenRange());
  }
  pair< pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>> SCREEN::getScreenRange(void){
    return make_pair(make_pair(0, 0), make_pair(SCREENStruct_->horpx-1, SCREENStruct_->verpx-1));
  }
  bool SCREEN::getInHeader(pair<uint32_t, uint32_t> point){
    return getInRange(point, getHeaderRange());
  }
  pair<pair<uint32_t, uint32_t>, pair<uint32_t,uint32_t>> SCREEN::getHeaderRange(void){
    return SCREENStruct_->header;
  }
  bool SCREEN::getInBody(pair<uint32_t, uint32_t> point){
    return getInRange(point, getBodyRange());
  }
  pair<pair<uint32_t, uint32_t>, pair<uint32_t,uint32_t>> SCREEN::getBodyRange(void){
    return SCREENStruct_->body;
  }
  pair<uint32_t,uint32_t> SCREEN::getReturnPointFromLeftBottomAlign(pair<uint32_t,uint32_t> pointLeftBottom, pair<uint8_t,uint8_t> size, SCREEN_ALIGN_t alignment){
    switch(alignment){
      case (SCREEN_ALIGN_LEFTBOTTOM):
	  // do nothing
	  break;
      case (SCREEN_ALIGN_LEFTMID):
	  pointLeftBottom.second += size.second/2;
	  break;
      case (SCREEN_ALIGN_LEFTTOP):
	  pointLeftBottom.second += size.second;
	  break;
      case (SCREEN_ALIGN_MIDBOTTOM):
	  pointLeftBottom.first += size.first/2;
	  break;
      case (SCREEN_ALIGN_MIDMID):
	  pointLeftBottom.first += size.first/2;
	  pointLeftBottom.second += size.second/2;
	  break;
      case (SCREEN_ALIGN_MIDTOP):
	  pointLeftBottom.first += size.first/2;
	  pointLeftBottom.second += size.second;
	  break;
      case (SCREEN_ALIGN_RIGHTBOTTOM):
	  pointLeftBottom.first += size.first;
	  break;
      case (SCREEN_ALIGN_RIGHTMID):
	  pointLeftBottom.first += size.first;
	  pointLeftBottom.second += size.second/2;
	  break;
      case (SCREEN_ALIGN_RIGHTTOP):
	  pointLeftBottom.first += size.first;
	  pointLeftBottom.second += size.second;
	  break;
    }
    return pointLeftBottom;
  }
  pair<uint32_t,uint32_t> SCREEN::getReturnPointToLeftBottomAlign(pair<uint32_t,uint32_t> point, pair<uint8_t,uint8_t> size, SCREEN_ALIGN_t alignment){
    switch(alignment){
      case(SCREEN_ALIGN_LEFTBOTTOM):
	  // nothing
	  break;
      case(SCREEN_ALIGN_LEFTMID):
	  if(point.second>size.second/2)
	    point.second -= size.second/2;
	  break;
      case(SCREEN_ALIGN_LEFTTOP):
	  if(point.second>size.second)
	  point.second -= size.second;
	  break;
      case(SCREEN_ALIGN_MIDBOTTOM):
	  if(point.first>size.first)
	  point.first -= size.first/2;
	  break;
    case(SCREEN_ALIGN_MIDMID):
	  if(point.first>size.first/2)
	  point.first -= size.first/2;
          if(point.second>size.second/2)
	  point.second -= size.second/2;
	  break;
      case(SCREEN_ALIGN_MIDTOP):
          if(point.first>size.first/2)
	  point.first -= size.first/2;
          if(point.second>size.second)
	  point.second -= size.second;
	  break;
      case(SCREEN_ALIGN_RIGHTBOTTOM):
          if(point.first>size.first)
	  point.first -= size.first;
	  break;
      case(SCREEN_ALIGN_RIGHTMID):
          if(point.first>size.first)
	  point.first -= size.first;
          if(point.second>size.second/2)
	  point.second -= size.second/2;
	  break;
      case(SCREEN_ALIGN_RIGHTTOP):
          if(point.first>size.first)
	  point.first -= size.first;
          if(point.second>size.second)
	  point.second -= size.second;
	  break;
    }
    return point;
  }
  pair<uint32_t,uint32_t> SCREEN::getCharacterPointWithShift(pair<uint32_t,uint32_t> point, pair<uint8_t,uint8_t> size){
    point.first += size.first;
    point.first += SCREENStruct_->letterSpace;
    return point;
  }
  pair<uint8_t,uint8_t> SCREEN::getCharSize(uint8_t scaling){
    uint8_t x, y;

    const uint8_t xBase=5, yBase=8;

    x = (scaling+1)*xBase;
    y = (scaling+1)*yBase;

    return make_pair(x, y);
  }

// write functions
  void SCREEN::writeBuffer(void){
    uint16_t count;
    setLED(true);
    setSS(false);
    for(count=0; count<getSizePixelBuffer(); count++){
	    transmitByte(SCREENStruct_->pixBuffer[count],false);
    }
    setSS(true);
    setLED(false);
  }
  void SCREEN::writeClearScreen(SCREEN_MODE_t mode){
    setBuffer((mode==SCREEN_MODE_NORMAL)?(false):(true));
    writeBuffer();
  }
  void SCREEN::writeClearScreen(void){
    writeClearScreen(SCREENStruct_->SCREENMode);
  }
  void SCREEN::writeHeader(bool newVal){
    setHeader(newVal);
    writeBuffer();
  }
  void SCREEN::writeBody(bool newVal){
    setBody(newVal);
    writeBuffer();
  }

/*** PRIVATE FUNCTIONS ***/

// fysical pin manipulations
  void SCREEN::setRST(bool newVal){
    IOObj_->setValue(SCREENStruct_->SSD1306Struct->pin_RST, newVal);
  }
  void SCREEN::setDC(bool newVal){
    IOObj_->setValue(SCREENStruct_->SSD1306Struct->pin_DC, newVal);
  }
  void SCREEN::setSS(bool newVal){
    IOObj_->setValue(SCREENStruct_->SSD1306Struct->SPIStruct->pin_SS, newVal);
  }
  void SCREEN::setLED(bool newVal){
    if(SCREENStruct_->SSD1306Struct->pin_LED != 0){
	IOObj_->setValue(SCREENStruct_->SSD1306Struct->pin_LED, newVal);
    }
  }

// debugging
  void SCREEN::printPrefix(void){
  	printf("[SCREEN] ");
  }

// internal struct functions
  uint32_t SCREEN::getSizePixelBuffer(void){
    return (((SCREENStruct_->horpx * SCREENStruct_->verpx)/8 )*SCREENStruct_->basecolors);
  }

// lower level transmission functions
  void SCREEN::transmitByte(SCREEN_SSD1306_COMMANDS_t cmd, bool en){
    setLED(true);
    setDC(false);
    if(en){
	    setSS(false);
    }
    SPIObj_->printnoSS(cmd);
    if(en){
	    setSS(true);
    }
    setLED(false);
  }
  void SCREEN::transmitByte(SCREEN_SSD1306_COMMANDS_t cmd){
    transmitByte(cmd, true);
  }
  void SCREEN::transmitByte(uint8_t data, bool en){
    setLED(true);
    setDC(true);
    if(en){
	    setSS(false);
    }
    SPIObj_->printnoSS(data);
    if(en){
	    setSS(true);
    }
    setLED(false);
  }
  void SCREEN::transmitCommand(uint8_t data){
    setLED(true);
    setSS(true);
    setDC(false);
    setSS(false);
    SPIObj_->printnoSS(data);
    setSS(true);
    setLED(false);
  }


} /* namespace stm32lib */
