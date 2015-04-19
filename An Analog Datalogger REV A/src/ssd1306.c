/*
 * ssd1306.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "ssd1306.h"

// local variables
static SSD1306_OLED_t ssd1306_parameters;

// local prototypes
static void
ssd1306_firstInit(void);
inline static void
setRST(BitAction newVal);
inline static void
setDC(BitAction newVal);
inline static void
setSS(BitAction newVal);
inline static void
transmitCommand(uint8_t val);
inline static void
transmitByte(uint8_t val);
inline static void
ssd1306_initializeDMA(void);
inline static void
ssd1306_alterPixel(uint8_t x, uint8_t y, BitAction newVal);
inline static void
ssd1306_alterLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, BitAction newval);
inline static void
ssd1306_alterArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, BitAction newval);
inline static xypair_t
ssd1306_adjustCoordinate(uint8_t x, uint8_t y);

// initialization
void
initialize_SSD1306(void){
	// set the parameters of the OLED screen
	ssd1306_parameters.pixels_height = OLED_HEIGHT;
	ssd1306_parameters.pixels_width = OLED_WIDTH;
	ssd1306_parameters.bufferSize = OLED_BufferSize;
	ssd1306_parameters.viewMode = SSD1306_VIEWMODE_NORMAL;
	ssd1306_parameters.viewOrientation = OLED_ORIENTATION;
	ssd1306_parameters.externalVCC = OLED_EXTERNALVCC;
	// make the framebuffer
	ssd1306_parameters.pixBuffer = malloc(ssd1306_parameters.bufferSize);
	if(ssd1306_parameters.pixBuffer == 0x00){
		free(ssd1306_parameters.pixBuffer);
		printf("ERROR: INTIALIZING OLED SSD1306");
		exit(1);
		return;
	}

	// initialize the pins
		// DC PIN
			gpio_initStandard(
					OLED_DC_PORT,
					OLED_DC_MODE,
					OLED_DC_OTYPE,
					OLED_DC_PIN,
					OLED_DC_PULL,
					OLED_DC_SPEED);
			GPIO_WriteBit(OLED_DC_PORT, (1<<OLED_DC_PIN), OLED_DC_INITSTATE);
		// RESET PIN
			gpio_initStandard(
					OLED_RST_PORT,
					OLED_RST_MODE,
					OLED_RST_OTYPE,
					OLED_RST_PIN,
					OLED_RST_PULL,
					OLED_RST_SPEED);
			GPIO_WriteBit(OLED_RST_PORT, (1<<OLED_RST_PIN), OLED_RST_INITSTATE);
		// CHIP SELECT PIN
			gpio_initStandard(
					OLED_SS_PORT,
					OLED_SS_MODE,
					OLED_SS_OTYPE,
					OLED_SS_PIN,
					OLED_SS_PULL,
					OLED_SS_SPEED);
			GPIO_WriteBit(OLED_SS_PORT, (1<<OLED_SS_PIN), OLED_SS_INITSTATE);
		// MOSI PIN
			gpio_initAF(
					OLED_MOSI_PORT,
					OLED_MOSI_MODE,
					OLED_MOSI_OTYPE,
					OLED_MOSI_PIN,
					OLED_MOSI_PULL,
					OLED_MOSI_SPEED,
					OLED_MOSI_AF);
		// SCK PIN
			gpio_initAF(
					OLED_SCK_PORT,
					OLED_SCK_MODE,
					OLED_SCK_OTYPE,
					OLED_SCK_PIN,
					OLED_SCK_PULL,
					OLED_SCK_SPEED,
					OLED_SCK_AF);
	// initialize the SPI driver
			rcc_setSPIClock(OLED_SPI, ENABLE);
		initialize_SPI(
				OLED_SPI,
				OLED_SPI_Direction,
				OLED_SPI_Mode,
				OLED_SPI_DataSize,
				OLED_SPI_CPOL,
				OLED_SPI_CPHA,
				OLED_SPI_NSS,
				OLED_SPI_BaudRatePrescaler,
				OLED_SPI_FirstBit,
				OLED_SPI_CRCPolynomial);
		SPI_Cmd(OLED_SPI, ENABLE);

	// initialize routine
	ssd1306_firstInit();
}
// pixel buffer
void
ssd1306_transmitPixelBuffer(void){
	setDC(Bit_SET);
	setSS(Bit_RESET);
	SPI_Cmd(OLED_SPI, DISABLE);
	ssd1306_initializeDMA();
	dma_enableSPIRequest(OLED_TX_DMAStream, OLED_SPI, OLED_TX_DMARequest);
	SPI_Cmd(OLED_SPI, ENABLE);
	while (DMA_GetFlagStatus(OLED_TX_DMAStream, OLED_TX_TransferCompleteFlag)==RESET);
	dma_disableSPIRequest(OLED_TX_DMAStream, OLED_SPI, OLED_TX_DMARequest);
	DMA_Cmd(OLED_TX_DMAStream, DISABLE);
	setSS(Bit_SET);
}
void
ssd1306_shiftPixelBuffer(void){

}
// interrupt
void
ssd1306_interruptHandler(void){
	if(TIM_GetITStatus(OLED_TIMER, OLED_TIMER_ITFlag)){
		TIM_ClearITPendingBit(OLED_TIMER, OLED_TIMER_ITFlag);
		leds_setLed(ledList_White1, ENABLE);
		ssd1306_transmitPixelBuffer();
		leds_setLed(ledList_White1, DISABLE);
	}
}
// testfunctions
void
ssd1306_pixbufferTest(void){
//	// test on direct buffer acces
//		leds_setLed(ledList_Blue, ENABLE);
//		memset(ssd1306_parameters.pixBuffer, 0, ssd1306_parameters.bufferSize);
//		for(uint32_t i=0; i<ssd1306_parameters.bufferSize; i++){
//			for(uint8_t j=0; j<8; j++){
//				*(ssd1306_parameters.pixBuffer+i) |= (1<<j);
//				delay_milli(2);
//			}delay_milli(1);
//		}
//		delay_milli(500);
//		for(uint32_t i=0; i<ssd1306_parameters.bufferSize; i++){
//			for(uint8_t j=0; j<8; j++){
//				*(ssd1306_parameters.pixBuffer+i) &= ~(1<<j);
//				delay_milli(1);
//			}delay_milli(1);
//		}
//		delay_milli(500);
//		leds_setLed(ledList_Blue, DISABLE);

	// test on pixel routine
	if( (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_LANDSCAPE_Normal) || (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_LANDSCAPE_UpsideDown) ){
		leds_setLed(ledList_Green, ENABLE);
		for(uint8_t i=0; i<ssd1306_parameters.pixels_height; i++){
			for(uint8_t j=0; j<ssd1306_parameters.pixels_width; j++){
				ssd1306_setPixel(j,i);
				delay_milli(1);
			}delay_milli(1);
		}
		delay_milli(500);
		for(uint8_t i=0; i<ssd1306_parameters.pixels_height; i++){
			for(uint8_t j=0; j<ssd1306_parameters.pixels_width; j++){
				ssd1306_clearPixel(j,i);
				delay_milli(1);
			}delay_milli(1);
		}
		delay_milli(500);
		leds_setLed(ledList_Green, DISABLE);
	} else if ( (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_PORTRAIT_CW) || (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_PORTRAIT_CCW) ){
		leds_setLed(ledList_Green, ENABLE);

			for(uint8_t j=0; j<ssd1306_parameters.pixels_width; j++){
				for(uint8_t i=0; i<ssd1306_parameters.pixels_height; i++){
				ssd1306_setPixel(i,j);
				delay_milli(1);
				}
			}delay_milli(1);
		delay_milli(500);

			for(uint8_t j=0; j<ssd1306_parameters.pixels_width; j++){
				for(uint8_t i=0; i<ssd1306_parameters.pixels_height; i++){
				ssd1306_clearPixel(i,j);
				delay_milli(1);
				}
			}delay_milli(1);
		delay_milli(500);

		leds_setLed(ledList_Green, DISABLE);
	}


}
// general settings
void
ssd1306_setOn(void){
	transmitCommand(SSD1306_COMMANDS_BASE_DisplayOn_1of1);
	TIM_ITConfig(OLED_TIMER, OLED_TIMER_ITFlag, ENABLE);
	NVIC_EnableIRQ(OLED_TIMER_IRQn);
	TIM_Cmd(OLED_TIMER, ENABLE);
}
void
ssd1306_setOff(void){
	TIM_Cmd(OLED_TIMER, DISABLE);
	TIM_ITConfig(OLED_TIMER, OLED_TIMER_ITFlag, DISABLE);
	NVIC_DisableIRQ(OLED_TIMER_IRQn);
	transmitCommand(SSD1306_COMMANDS_BASE_DisplayOff_1of1);
}
void
ssd1306_setOrientation(SSD1306_ORIENTATION_t newVal){
	if( newVal == SSD1306_ORIENTATION_PORTRAIT_CW ){
		transmitCommand(SSD1306_COMMANDS_HCONF_SetSegmentReMapCol0AsSeg0_1of1);
		transmitCommand(SSD1306_COMMANDS_HCONF_SetComDirectionNormal_1of1);
		transmitCommand(SSD1306_COMMANDS_ADDRESS_SetMemoryAddressingMode_1of2);
		transmitCommand(0x01);
	} else if ( newVal == SSD1306_ORIENTATION_PORTRAIT_CCW ){
		transmitCommand(SSD1306_COMMANDS_HCONF_SetSegmentReMapCol127AsSeg0_1of1);
		transmitCommand(SSD1306_COMMANDS_HCONF_SetComDirectionInverted_1of1);
		transmitCommand(SSD1306_COMMANDS_ADDRESS_SetMemoryAddressingMode_1of2);
		transmitCommand(0x01);
	} else if ( newVal == SSD1306_ORIENTATION_LANDSCAPE_Normal ){
		transmitCommand(SSD1306_COMMANDS_HCONF_SetSegmentReMapCol127AsSeg0_1of1);
		transmitCommand(SSD1306_COMMANDS_HCONF_SetComDirectionNormal_1of1);
		transmitCommand(SSD1306_COMMANDS_ADDRESS_SetMemoryAddressingMode_1of2);
		transmitCommand(0x00);
	} else if ( newVal == SSD1306_ORIENTATION_LANDSCAPE_UpsideDown ){
		transmitCommand(SSD1306_COMMANDS_HCONF_SetSegmentReMapCol0AsSeg0_1of1);
		transmitCommand(SSD1306_COMMANDS_HCONF_SetComDirectionInverted_1of1);
		transmitCommand(SSD1306_COMMANDS_ADDRESS_SetMemoryAddressingMode_1of2);
		transmitCommand(0x00);
	}

}
void
ssd1306_setViewMode(SSD1306_VIEWMODE_t newVal){

}
// pixels
uint8_t
ssd1306_getPixel(uint8_t x, uint8_t y){
	return 0;
}
void
ssd1306_setPixel(uint8_t x, uint8_t y){
	ssd1306_alterPixel(x, y, Bit_SET);
}
void
ssd1306_clearPixel(uint8_t x, uint8_t y){
	ssd1306_alterPixel(x, y, Bit_RESET);
}
inline static void
ssd1306_alterPixel(uint8_t x, uint8_t y, BitAction newVal){
	if( (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_LANDSCAPE_Normal) || (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_LANDSCAPE_UpsideDown) ){
		if( (x>=ssd1306_parameters.pixels_width) || (y>=ssd1306_parameters.pixels_height) ){
			return;
		} else {
			// From 0,0 to 0,7; 1,0 to 1,7 .... From 127,0 to 127,7
			// From 0,8 to 0,15; 1,8 to 1,15 .... From 127,8 to 127,15
			// ...
			// From 0,55 to 0,63; 1,55 to 1,63 .... From 127,55 to 127,63
			if(newVal){
				ssd1306_parameters.pixBuffer[(x) + ( (y/8) * ssd1306_parameters.pixels_width)] |= (1<< (y%8) );
				printf("SET LANDSCAPE x: %d, y: %d ; byte: %d ; bit: %d \r\n", x, y, (x) + ( (y/8) * ssd1306_parameters.pixels_width), y%8);
				delay_milli(50);
			} else {
				ssd1306_parameters.pixBuffer[(x) + ( (y/8) * ssd1306_parameters.pixels_width)] &= ~(1<< (y%8) );
				printf("RESET LANDSCAPE x: %d, y: %d ; byte: %d ; bit: %d \r\n", x, y, (x) + ( (y/8) * ssd1306_parameters.pixels_width), y%8);
				delay_milli(50);
			}
		}

	} else if ( (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_PORTRAIT_CW) || (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_PORTRAIT_CCW) ){
		if( (x>=ssd1306_parameters.pixels_height) || (y>=ssd1306_parameters.pixels_width) ){
			return;
		} else {
			// From 0,0 to 63,0
			// From 0,1 to 63,1
			// ...
			// From 0,127 to 63,127
			if(newVal){
				ssd1306_parameters.pixBuffer[(x/8) + y*ssd1306_parameters.pixels_height/8] |= (1<< (x%8) );
				//printf("SET PORTRAIT x: %d, y: %d ; byte: %d ; bit: %d \r\n", x, y, (x/8) + y*ssd1306_parameters.pixels_height/8, x%8);
				//delay_milli(50);
			} else {
				ssd1306_parameters.pixBuffer[(x/8) + y*ssd1306_parameters.pixels_height/8] &= ~(1<< (x%8) );
				//printf("RESET PORTRAIT x: %d, y: %d ; byte: %d ; bit: %d \r\n", x, y, (x/8) + y*ssd1306_parameters.pixels_height/8, x%8);
				//delay_milli(50);
			}
		}
	}
}
// whole screen
void
ssd1306_setScreen(void){
	memset(ssd1306_parameters.pixBuffer, 0xFF, ssd1306_parameters.bufferSize);
}
void
ssd1306_clearScreen(void){
	memset(ssd1306_parameters.pixBuffer, 0x00, ssd1306_parameters.bufferSize);
}
// characters
void
ssd1306_setCharacter(uint8_t x, uint8_t y){

}
void
ssd1306_clearCharacter(uint8_t x, uint8_t y){

}
void
ssd1306_setString(uint8_t x, uint8_t y){

}
void
ssd1306_clearString(uint8_t x, uint8_t y){

}
// lines
void
ssd1306_setLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
	ssd1306_alterLine(x0,y0,x1,y1,Bit_SET);
}
void
ssd1306_clearLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
	ssd1306_alterLine(x0,y0,x1,y1,Bit_RESET);
}
inline static void
ssd1306_alterLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, BitAction newval){
	xypair_t xy0, xy1; // input normalized parameters

	// normalize values
	xy0 = ssd1306_adjustCoordinate(x0, y0);
	xy1 = ssd1306_adjustCoordinate(x1, y1);
	// get the difference

	/* Bresenham's line algorithm */
		// Reference: http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm
		// Altered with the struct
		int dx = abs(xy1.x-xy0.x), sx = xy0.x<xy1.x ? 1 : -1;
		int dy = abs(xy1.y-xy0.y), sy = xy0.y<xy1.y ? 1 : -1;
		int err = (dx>dy ? dx : -dy)/2, e2;

		for(;;){
		ssd1306_alterPixel(xy0.x,xy0.y, newval);
		if (xy0.x==xy1.x && xy0.y==xy1.y) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; xy0.x += sx; }
		if (e2 < dy) { err += dx; xy0.y += sy; }
		}
	/* Bresenham's line algorithm */

}

// circles
void
ssd1306_setCircle(uint8_t x, uint8_t y){

}
void
ssd1306_clearCircle(uint8_t x, uint8_t y){

}
// area's
void
ssd1306_setArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
	ssd1306_alterArea(x0, y0, x1, y1, Bit_SET);
}
void
ssd1306_clearArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
	ssd1306_alterArea(x0, y0, x1, y1, Bit_RESET);
}
inline static void
ssd1306_alterArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, BitAction newval){
	uint8_t xMin = MIN(x0,x1), xMax = MAX(x0,x1);
	uint8_t yMin = MIN(y0,y1), yMax = MAX(y0,y1);
	uint8_t i;
	for(i=0; i<yMax-yMin; i++){
		ssd1306_alterLine(xMin, yMin+i, xMax, yMin+i, newval);
	}
}

// local functions
static void
ssd1306_firstInit(void){
		setRST(Bit_RESET);
		delay_milli(10);
		setRST(Bit_SET);
		delay_milli(10);
		setDC(Bit_SET);
		setSS(Bit_SET);

		// Steps are taken from UG-2864HMBEG01 OLED screen.
		// Reference: http://www.sunrom.com/media/files/p/648/4199_Datasheet.pdf
		// Altered the steps to squeeze in my orientation function.

		/* STEP 1 */
			// display off
			transmitCommand(SSD1306_COMMANDS_BASE_DisplayOff_1of1);

		/* STEP 2 */
			// clock divide set
			transmitCommand(SSD1306_COMMANDS_TDCONF_SetDisplayClockDivide_1of2);
			transmitCommand(0x80); // suggested ratio

		/* STEP 3 */
			// set multiplex ratio
			transmitCommand(SSD1306_COMMANDS_HCONF_SetMultiplexRatio_1of2);
			transmitCommand(0x3F);

		/* STEP 4 */
			// set display offset
			transmitCommand(SSD1306_COMMANDS_HCONF_SetDisplayOffset_1of2);
			transmitCommand(0x00);

		/* STEP 5 */
			// Set display start line
			transmitCommand(SSD1306_COMMANDS_HCONF_SetDisplayStartLine_1of1);

		/* STEP 6 */
			// set chargepump
			transmitCommand(SSD1306_COMMANDS_CHRGPMP_SetChargepumpSetting_1of2);
			if(ssd1306_parameters.externalVCC == Bit_SET){
				transmitCommand(0x10);
			} else {
				transmitCommand(0x14);
			}

				/* ORIENTATION FUNCTION TAKING OVER STEP 7, 8 && Added step */
				ssd1306_setOrientation(ssd1306_parameters.viewOrientation);

//				/* STEP 7 */
//					// segment remap
//					transmitCommand(SSD1306_COMMANDS_HCONF_SetSegmentReMapCol0AsSeg0_1of1);
//
//				/* STEP 8 */
//					// Com output from high to SEG0
//					transmitCommand(SSD1306_COMMANDS_HCONF_SetComDirectionNormal_1of1);
//
//				/* STEP NOT DESCRIBED */
//				// set memory mode
//					//	A[1:0] = 00b, Horizontal Addressing Mode
//					//	A[1:0] = 01b, Vertical Addressing Mode
//					//	A[1:0] = 10b, Page Addressing Mode (RESET)
//					//	A[1:0] = 11b, Invalid
//
//					transmitCommand(SSD1306_COMMANDS_ADDRESS_SetMemoryAddressingMode_1of2);
//					transmitCommand(0x01);

		/* STEP 9 */
			// set com pins
			transmitCommand(SSD1306_COMMANDS_HCONF_SetComPinsHardwareConfig_1of2);
			transmitCommand(0x12);

		/* STEP 10 */
			// set contrast
			transmitCommand(SSD1306_COMMANDS_BASE_Contrast_1of2);
			if(ssd1306_parameters.externalVCC == Bit_SET){
				transmitCommand(0x9F);
			} else {
				transmitCommand(0xCF);
			}

		/* STEP 11 */
			// set precharge
			transmitCommand(SSD1306_COMMANDS_TDCONF_SetPrechargePeriod_1of2);
			if(ssd1306_parameters.externalVCC == Bit_SET){
				transmitCommand(0x22);
			} else {
				transmitCommand(0xF1);
			}

		/* STEP 12 */
			// set VCOMH deselect level
			transmitCommand(SSD1306_COMMANDS_TDCONF_SetVCOMHDeselectLevel_1of2);
			transmitCommand(0x40);

		/* STEP 13 */
			// set on and use data in RAM
			transmitCommand(SSD1306_COMMANDS_BASE_DisplayOn_UseRam_1of1);

		/* STEP 14 */
			// set normal state
			transmitCommand(SSD1306_COMMANDS_BASE_SetNormal_1of1);

		/* STEP 15 */
			memset(ssd1306_parameters.pixBuffer, 0, ssd1306_parameters.bufferSize);

		/* STEP 16 */
			transmitCommand(SSD1306_COMMANDS_BASE_DisplayOn_1of1);

		//transmitCommand(SSD1306_COMMANDS_BASE_DisplayOn_IgnoreRam_1of1); // Calling this function will result in a screen fully lit.

		// initialize timer for regular updating
		TIM_TimeBaseInitTypeDef tmp;
		/* Enable the global Interrupt */
		nvic_initInterrupt(
				OLED_TIMER_IRQn,
				0,
				1);

		rcc_setTIMClock(OLED_TIMER, ENABLE);
		/* Time base configuration */
		tmp.TIM_Period = 20000 - 1; // 1 MHz down to 50Hz (20 ms)
		tmp.TIM_Prescaler = 84 - 1; // 84 MHz Clock down to 1 MHz (adjust per your clock)
		tmp.TIM_ClockDivision = 0;
		tmp.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(OLED_TIMER, &tmp);
		/* TIM IT enable */
		TIM_ITConfig(OLED_TIMER, OLED_TIMER_ITFlag, ENABLE);
		/* TIM2 enable counter */
		TIM_Cmd(OLED_TIMER, ENABLE);
}
inline static void
setRST(BitAction newVal){
	GPIO_WriteBit(OLED_RST_PORT, (1<<OLED_RST_PIN), newVal);
}
inline static void
setDC(BitAction newVal){
	GPIO_WriteBit(OLED_DC_PORT, (1<<OLED_DC_PIN), newVal);
}
inline static void
setSS(BitAction newVal){
	GPIO_WriteBit(OLED_SS_PORT, (1<<OLED_SS_PIN), newVal);
}
inline static void
transmitCommand(uint8_t val){
	setDC(Bit_RESET);
	transmitByte(val);
}
inline static void
transmitByte(uint8_t val){
	setSS(Bit_RESET);
	SPI_I2S_SendData(OLED_SPI, val);
	while(SPI_I2S_GetFlagStatus(OLED_SPI, SPI_I2S_FLAG_TXE) == RESET);
	setSS(Bit_SET);
}
inline static void
ssd1306_initializeDMA(void){
	// DMA only used for transfer of DATA
	DMA_DeInit(OLED_TX_DMAStream);
	initialize_DMA(
			OLED_TX_DMAStream,
			OLED_TX_DMAChannel,
			OLED_DMA_PeripheralBaseAddr,
			(uint32_t)ssd1306_parameters.pixBuffer,
			OLED_DMA_DIR,
			ssd1306_parameters.bufferSize,
			OLED_DMA_PeripheralInc,
			OLED_DMA_MemoryInc,
			OLED_DMA_PeripheralDataSize,
			OLED_DMA_MemoryDataSize,
			OLED_DMA_Mode,
			OLED_DMA_Priority,
			OLED_DMA_FIFOMode,
			OLED_DMA_FIFOThreshold,
			OLED_DMA_MemoryBurst,
			OLED_DMA_PeripheralBurst
			);
}
inline static xypair_t
ssd1306_adjustCoordinate(uint8_t x, uint8_t y){
	xypair_t tmp;
	if( (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_LANDSCAPE_Normal) || (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_LANDSCAPE_UpsideDown) ){
		if (x>=ssd1306_parameters.pixels_width){
			x = ssd1306_parameters.pixels_width-1;
		}
		if (y>=ssd1306_parameters.pixels_height) {
			y = ssd1306_parameters.pixels_height-1;
		}

	} else if ( (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_PORTRAIT_CW) || (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_PORTRAIT_CCW) ){
		if (x>=ssd1306_parameters.pixels_height){
			x = ssd1306_parameters.pixels_height-1;
		}
		if (y>=ssd1306_parameters.pixels_width) {
			y = ssd1306_parameters.pixels_width-1;
		}
	}
	tmp.x = x;
	tmp.y = y;
	return tmp;
}
