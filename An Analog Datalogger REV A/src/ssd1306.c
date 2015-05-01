/*
 * ssd1306.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

// include the header file
	#include "ssd1306.h"

// local variables
	static SSD1306_OLED_t ssd1306_parameters; // stores the local variables of the ssd1306

// local prototypes
	// first initialization routine
	static void
	ssd1306_firstInit(void);
	
	// set Reset Pin
	inline static void
	setRST(BitAction newVal);
	
	// set DC pin
	inline static void
	setDC(BitAction newVal);
	
	// set Chip Select pin
	inline static void
	setSS(BitAction newVal);
	
	// transmit a command to the ssd1306
	inline static void
	transmitCommand(uint8_t val);
	
	// transmit a byte to the ssd1306
	inline static void
	transmitByte(uint8_t val);
	
	// initialize the DMA for the ssd1306
	inline static void
	ssd1306_initializeDMA(void);
	
	// alter a pixel on the ssd1306
	inline static void
	ssd1306_alterPixel(uint8_t x, uint8_t y, BitAction newVal);
	
	// alter a character on the ssd1306
	inline static xypair_t
	ssd1306_alterCharacter(uint8_t x, uint8_t y, uint8_t ch, struct FONT_DEF font, BitAction newVal);
	
	// alter a string on the ssd1306
	inline static xypair_t
	ssd1306_alterString(uint8_t x, uint8_t y, const char* str, struct FONT_DEF font, BitAction newVal);
	
	// alter a string with addition of a string border on the ssd1306
	inline static void
	ssd1306_alterStringBorder(uint8_t x, uint8_t y, const char* str, struct FONT_DEF font, uint8_t padding, BitAction newVal);
	
	// alter a line on the ssd1306
	inline static void
	ssd1306_alterLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, BitAction newval);
	
	// alter a circle on the ssd1306
	inline static void
	ssd1306_alterCircle(uint8_t x0, uint8_t y0, uint16_t radius, BitAction newVal);
	
	// alter an area on the ssd1306
	inline static void
	ssd1306_alterArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, BitAction newval);
	
	// check if the coordinate is within writing range of the screen. If not, clip the value
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
		// allocation of the buffer
		ssd1306_parameters.pixBuffer = malloc(ssd1306_parameters.bufferSize);
		
		// make sure the allocation is done correctly, return if not
		if(ssd1306_parameters.pixBuffer == 0x00){
			free(ssd1306_parameters.pixBuffer);
			printf("ERROR: INTIALIZING OLED SSD1306");
			exit(1);
			return;
		}

	// initialize the pins
		// DC PIN
			// do a standard initialization
			gpio_initStandard(
					OLED_DC_PORT,
					OLED_DC_MODE,
					OLED_DC_OTYPE,
					OLED_DC_PIN,
					OLED_DC_PULL,
					OLED_DC_SPEED);
			// standard output value
			GPIO_WriteBit(OLED_DC_PORT, (1<<OLED_DC_PIN), OLED_DC_INITSTATE);
			
		// RESET PIN
			// do a standard initialization
			gpio_initStandard(
					OLED_RST_PORT,
					OLED_RST_MODE,
					OLED_RST_OTYPE,
					OLED_RST_PIN,
					OLED_RST_PULL,
					OLED_RST_SPEED);
					
			// standard output value
			GPIO_WriteBit(OLED_RST_PORT, (1<<OLED_RST_PIN), OLED_RST_INITSTATE);
			
		// CHIP SELECT PIN
			// do a standard initialization
			gpio_initStandard(
					OLED_SS_PORT,
					OLED_SS_MODE,
					OLED_SS_OTYPE,
					OLED_SS_PIN,
					OLED_SS_PULL,
					OLED_SS_SPEED);
					
			// standard output value
			GPIO_WriteBit(OLED_SS_PORT, (1<<OLED_SS_PIN), OLED_SS_INITSTATE);
			
		// MOSI PIN
			// do a standard initialization
			gpio_initAF(
					OLED_MOSI_PORT,
					OLED_MOSI_MODE,
					OLED_MOSI_OTYPE,
					OLED_MOSI_PIN,
					OLED_MOSI_PULL,
					OLED_MOSI_SPEED,
					OLED_MOSI_AF);
					
		// SCK PIN
			// do a standard initialization
			gpio_initAF(
					OLED_SCK_PORT,
					OLED_SCK_MODE,
					OLED_SCK_OTYPE,
					OLED_SCK_PIN,
					OLED_SCK_PULL,
					OLED_SCK_SPEED,
					OLED_SCK_AF);
					
	// initialize the SPI driver
		// set the SPI clock to enable
		rcc_setSPIClock(OLED_SPI, ENABLE);
		
		// do an SPI initialization
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
			
		// enable the SPI
		SPI_Cmd(OLED_SPI, ENABLE);

	// initialize routine
	ssd1306_firstInit();

	menu_enable = 0x00;
}
// pixel buffer
void
ssd1306_transmitPixelBuffer(void){
	setDC(Bit_SET);
	setSS(Bit_RESET);
	SPI_Cmd(OLED_SPI, DISABLE);
	ssd1306_initializeDMA();
	dma_enableSPIRequest(OLED_DMA_TX_DMAStream, OLED_SPI, OLED_DMA_TX_DMARequest);
	SPI_Cmd(OLED_SPI, ENABLE);
	while (DMA_GetFlagStatus(OLED_DMA_TX_DMAStream, OLED_DMA_TX_TransferCompleteFlag)==RESET);
	dma_disableSPIRequest(OLED_DMA_TX_DMAStream, OLED_SPI, OLED_DMA_TX_DMARequest);
	DMA_Cmd(OLED_DMA_TX_DMAStream, DISABLE);
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
		menustructure_render();
		ssd1306_transmitPixelBuffer();
		leds_setLed(ledList_White1, DISABLE);
	}
}
// testfunctions
void
ssd1306_pixbufferTest(void){
	// test on direct buffer acces
	ssd1306_clearScreen();
	uint32_t currentTime = delay_getMillis();
		leds_setLed(ledList_Blue, ENABLE);
		memset(ssd1306_parameters.pixBuffer, 0, ssd1306_parameters.bufferSize);
		for(uint32_t i=0; i<ssd1306_parameters.bufferSize; i++){
			for(uint8_t j=0; j<8; j++){
				*(ssd1306_parameters.pixBuffer+i) |= (1<<j);
			}delay_milli(1);
		}
		delay_milli(500);
		for(uint32_t i=0; i<ssd1306_parameters.bufferSize; i++){
			for(uint8_t j=0; j<8; j++){
				*(ssd1306_parameters.pixBuffer+i) &= ~(1<<j);
			}delay_milli(1);
		}
		leds_setLed(ledList_Blue, DISABLE);
	printf("The direct pixeltest took %ldms\r\n", delay_getMilliDifferenceSimple(currentTime));
	delay_milli(2500);

	// test on pixel routine
	ssd1306_clearScreen();
	currentTime = delay_getMillis();
	xypair_t tmp = ssd1306_getScreenDimensions();
	leds_setLed(ledList_Green, ENABLE);
	for(uint8_t i=0; i<tmp.y; i++){
		for(uint8_t j=0; j<tmp.x; j++){
			ssd1306_setPixel(j,i);
		}delay_milli(1);
	}
	delay_milli(500);
	for(uint8_t i=0; i<tmp.y; i++){
		for(uint8_t j=0; j<tmp.x; j++){
			ssd1306_clearPixel(j,i);
		}delay_milli(1);
	}

	leds_setLed(ledList_Green, DISABLE);
	printf("The pixelRoutine pixeltest took %ld ms\r\n", delay_getMilliDifferenceSimple(currentTime));
	delay_milli(2500);
}
void
ssd1306_lineTest(void){
	// test on lines
	ssd1306_clearScreen();
	xypair_t tmp = ssd1306_getScreenDimensions();
	uint32_t currentTime = delay_getMillis();
	leds_setLed(ledList_Green, ENABLE);
	for(uint8_t i=0; i<100; i++){
		uint8_t x0, x1, y0, y1;
		x0 = RNG_GetRandomNumber() % tmp.x;
		x1 = RNG_GetRandomNumber() % tmp.x;
		y0 = RNG_GetRandomNumber() % tmp.y;
		y1 = RNG_GetRandomNumber() % tmp.y;

		ssd1306_setLine(x0,x1,y0,y1);
		delay_milli(2);
	}
	delay_milli(1000);
	ssd1306_setScreen();
	for(uint8_t i=0; i<100; i++){
		uint8_t x0, x1, y0, y1;
		x0 = RNG_GetRandomNumber() % tmp.x;
		x1 = RNG_GetRandomNumber() % tmp.x;
		y0 = RNG_GetRandomNumber() % tmp.y;
		y1 = RNG_GetRandomNumber() % tmp.y;

		ssd1306_clearLine(x0,x1,y0,y1);
		delay_milli(2);
	}
	leds_setLed(ledList_Green, DISABLE);
	printf("The linetest took %ld ms\r\n", delay_getMilliDifferenceSimple(currentTime));
	delay_milli(2500);
}
void
ssd1306_circleTest(void){
	// test on circles
	ssd1306_clearScreen();
	xypair_t tmp = ssd1306_getScreenDimensions();
	uint32_t currentTime = delay_getMillis();
	leds_setLed(ledList_Green, ENABLE);
	for(uint8_t i=0; i<100; i++){
		uint8_t x0, x1, rad;
		x0 = RNG_GetRandomNumber() % tmp.x;
		x1 = RNG_GetRandomNumber() % tmp.x;
		rad = RNG_GetRandomNumber() % tmp.y;

		ssd1306_setCircle(x0,x1,rad);
		delay_milli(2);
	}
	delay_milli(1000);
	ssd1306_setScreen();
	for(uint8_t i=0; i<100; i++){
		uint8_t x0, x1, rad;
		x0 = RNG_GetRandomNumber() % tmp.x;
		x1 = RNG_GetRandomNumber() % tmp.x;
		rad = RNG_GetRandomNumber() % tmp.y;

		ssd1306_clearCircle(x0,x1,rad);
		delay_milli(2);
	}
	leds_setLed(ledList_Green, DISABLE);
	printf("The circletest took %ld ms\r\n", delay_getMilliDifferenceSimple(currentTime));
	delay_milli(2500);
}
void
ssd1306_areaTest(void){
	// test on area
	ssd1306_clearScreen();
	xypair_t tmp = ssd1306_getScreenDimensions();
	uint32_t currentTime = delay_getMillis();
	leds_setLed(ledList_Green, ENABLE);
	for(uint8_t i=0; i<100; i++){
		uint8_t x0, x1, y0, y1;
		x0 = RNG_GetRandomNumber() % tmp.x;
		x1 = RNG_GetRandomNumber() % tmp.x;
		y0 = RNG_GetRandomNumber() % tmp.y;
		y1 = RNG_GetRandomNumber() % tmp.y;

		ssd1306_alterArea(x0,x1,y0,y1, Bit_SET);
		delay_milli(20);
		ssd1306_clearScreen();
	}
	leds_setLed(ledList_Green, DISABLE);
	printf("The areatest took %ld ms\r\n", delay_getMilliDifferenceSimple(currentTime));
	delay_milli(2500);
}
void
ssd1306_stringTest(void){
	// test on lines
	ssd1306_clearScreen();
	xypair_t tmp = ssd1306_getScreenDimensions();
	uint32_t currentTime = delay_getMillis();
	leds_setLed(ledList_Green, ENABLE);
	for(uint8_t i=0; i<100; i++){
		uint8_t x0, y0;
		x0 = RNG_GetRandomNumber() % tmp.x;
		y0 = RNG_GetRandomNumber() % tmp.y;
		switch(i%5){
		case 0: ssd1306_alterString(x0/2,y0,"TESTSTRING!!", Font_System3x6, Bit_SET); break;
		case 1: ssd1306_alterString(x0/2,y0,"Teststring!!", Font_System5x8, Bit_SET); break;
		case 2: ssd1306_alterString(x0/2,y0,"Teststring!!", Font_System7x8, Bit_SET); break;
		case 3: ssd1306_alterString(x0/2,y0,"Teststring!!", Font_8x8, Bit_SET); break;
		case 4: ssd1306_alterString(x0/2,y0,"Teststring!!", Font_8x8Thin, Bit_SET); break;
		}

		delay_milli(600);
		ssd1306_clearScreen();
	}
	leds_setLed(ledList_Green, DISABLE);
	printf("The set stringtest took %ld ms\r\n", delay_getMilliDifferenceSimple(currentTime));
	delay_milli(2500);

	ssd1306_setScreen();

	currentTime = delay_getMillis();
	leds_setLed(ledList_Green, ENABLE);
	for(uint8_t i=0; i<100; i++){
		uint8_t x0, y0;
		x0 = RNG_GetRandomNumber() % tmp.x;
		y0 = RNG_GetRandomNumber() % tmp.y;
		switch(i%5){
		case 0: ssd1306_alterString(x0/2,y0,"TESTSTRING!!", Font_System3x6, Bit_RESET); break;
		case 1: ssd1306_alterString(x0/2,y0,"Teststring!!", Font_System5x8, Bit_RESET); break;
		case 2: ssd1306_alterString(x0/2,y0,"Teststring!!", Font_System7x8, Bit_RESET); break;
		case 3: ssd1306_alterString(x0/2,y0,"Teststring!!", Font_8x8, Bit_RESET); break;
		case 4: ssd1306_alterString(x0/2,y0,"Teststring!!", Font_8x8Thin, Bit_RESET); break;
		}

		delay_milli(600);
		ssd1306_setScreen();
	}
	leds_setLed(ledList_Green, DISABLE);
	printf("The clear stringtest took %ld ms\r\n", delay_getMilliDifferenceSimple(currentTime));
	delay_milli(2500);

	ssd1306_clearScreen();

	currentTime = delay_getMillis();
	leds_setLed(ledList_Green, ENABLE);
	for(uint8_t i=0; i<100; i++){
		uint8_t x0, y0;
		x0 = RNG_GetRandomNumber() % tmp.x;
		y0 = RNG_GetRandomNumber() % tmp.y;
		switch(i%5){
		case 0: ssd1306_setStringWithBorder(x0/2,y0,"TESTSTRING!!", Font_System3x6, 2); break;
		case 1: ssd1306_setStringWithBorder(x0/2,y0,"Teststring!!", Font_System5x8, 2); break;
		case 2: ssd1306_setStringWithBorder(x0/2,y0,"Teststring!!", Font_System7x8, 2); break;
		case 3: ssd1306_setStringWithBorder(x0/2,y0,"Teststring!!", Font_8x8, 2); break;
		case 4: ssd1306_setStringWithBorder(x0/2,y0,"Teststring!!", Font_8x8Thin, 2); break;
		}

		delay_milli(600);
		ssd1306_clearScreen();
	}
	leds_setLed(ledList_Green, DISABLE);
	printf("The stringtest with border took %ld ms\r\n", delay_getMilliDifferenceSimple(currentTime));
	delay_milli(2500);

	ssd1306_clearScreen();

	currentTime = delay_getMillis();
	leds_setLed(ledList_Green, ENABLE);
	for(uint8_t i=0; i<100; i++){
		uint8_t x0, y0;
		x0 = RNG_GetRandomNumber() % tmp.x;
		y0 = RNG_GetRandomNumber() % tmp.y;
		switch(i%5){
		case 0: ssd1306_setStringInverted(x0/2,y0,"TESTSTRING!!", Font_System3x6, 2); break;
		case 1: ssd1306_setStringInverted(x0/2,y0,"Teststring!!", Font_System5x8, 2); break;
		case 2: ssd1306_setStringInverted(x0/2,y0,"Teststring!!", Font_System7x8, 2); break;
		case 3: ssd1306_setStringInverted(x0/2,y0,"Teststring!!", Font_8x8, 2); break;
		case 4: ssd1306_setStringInverted(x0/2,y0,"Teststring!!", Font_8x8Thin, 2); break;
		}

		delay_milli(600);
		ssd1306_clearScreen();
	}
	leds_setLed(ledList_Green, DISABLE);
	printf("The inverted stringtest took %ld ms\r\n", delay_getMilliDifferenceSimple(currentTime));
	delay_milli(2500);
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
	ssd1306_parameters.viewMode = newVal;
	if(newVal == SSD1306_VIEWMODE_NORMAL){
		transmitCommand(SSD1306_COMMANDS_BASE_SetNormal_1of1);
	} else if(newVal == SSD1306_VIEWMODE_INVERTED){
		transmitCommand(SSD1306_COMMANDS_BASE_SetInverse_1of1);
	}
}
xypair_t
ssd1306_getScreenDimensions(void){
	xypair_t tmp;
	if( (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_LANDSCAPE_Normal) || (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_LANDSCAPE_UpsideDown) ){
		tmp.x = ssd1306_parameters.pixels_width;
		tmp.y = ssd1306_parameters.pixels_height;
	} else if ( (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_PORTRAIT_CW) || (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_PORTRAIT_CCW) ){
		tmp.x = ssd1306_parameters.pixels_height;
		tmp.y = ssd1306_parameters.pixels_width;
	}
	return tmp;
}
xycorners_t
ssd1306_getAdjustedPoints(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
	xycorners_t tmp;
	xypair_t xy0 = ssd1306_adjustCoordinate(x0,y0);
	xypair_t xy1 = ssd1306_adjustCoordinate(x1,y1);
	tmp.topLeft.x = MIN(xy0.x,xy1.x); tmp.topLeft.y = MAX(xy0.y,xy1.y);
	tmp.topRight.x = MAX(xy0.x,xy1.x); tmp.topRight.y = MAX(xy0.y,xy1.y);
	tmp.bottomLeft.x = MIN(xy0.x,xy1.x); tmp.bottomLeft.y = MIN(xy0.y,xy1.y);
	tmp.bottomRight.x = MAX(xy0.x,xy1.x); tmp.bottomRight.y = MIN(xy0.y,xy1.y);
	return tmp;
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
	xypair_t tmp = ssd1306_getScreenDimensions();
	// not writing out of bounds
	if(x>=tmp.x || y>=tmp.y){ return; }

	if( (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_LANDSCAPE_Normal) || (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_LANDSCAPE_UpsideDown) ){
			// From 0,0 to 0,7; 1,0 to 1,7 .... From 127,0 to 127,7
			// From 0,8 to 0,15; 1,8 to 1,15 .... From 127,8 to 127,15
			// ...
			// From 0,55 to 0,63; 1,55 to 1,63 .... From 127,55 to 127,63
			if(newVal){
				ssd1306_parameters.pixBuffer[(x) + ( (y/8) * ssd1306_parameters.pixels_width)] |= (1<< (y%8) );
//				printf("SET LANDSCAPE x: %d, y: %d ; byte: %d ; bit: %d \r\n", x, y, (x) + ( (y/8) * ssd1306_parameters.pixels_width), y%8);
//				delay_milli(50);
			} else {
				ssd1306_parameters.pixBuffer[(x) + ( (y/8) * ssd1306_parameters.pixels_width)] &= ~(1<< (y%8) );
//				printf("RESET LANDSCAPE x: %d, y: %d ; byte: %d ; bit: %d \r\n", x, y, (x) + ( (y/8) * ssd1306_parameters.pixels_width), y%8);
//				delay_milli(50);
			}
	} else if ( (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_PORTRAIT_CW) || (ssd1306_parameters.viewOrientation == SSD1306_ORIENTATION_PORTRAIT_CCW) ){
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
ssd1306_setCharacter(uint8_t x, uint8_t y, uint8_t ch, struct FONT_DEF font){
	ssd1306_alterCharacter(x,y,ch,font,Bit_SET);
}
void
ssd1306_clearCharacter(uint8_t x, uint8_t y, uint8_t ch, struct FONT_DEF font){
	ssd1306_alterCharacter(x,y,ch,font,Bit_RESET);
}
inline static xypair_t
ssd1306_alterCharacter(uint8_t x, uint8_t y, uint8_t ch, struct FONT_DEF font, BitAction newVal){
	  uint8_t col, column[font.u8Width];
	  xypair_t tmp;

	  // Check if the requested character is available
	  if ((ch >= font.u8FirstChar) && (ch <= font.u8LastChar))
	  {
	    // Retrieve appropriate columns from font data
	    for (col = 0; col < font.u8Width; col++)
	    {
	      column[col] = font.au8FontTable[((ch - 32) * font.u8Width) + col];    // Get first column of appropriate character
	    }
	  }
	  else
	  {
	    // Requested character is not available in this font ... send a space instead
	    for (col = 0; col < font.u8Width; col++)
	    {
	      column[col] = 0xFF;    // Send solid space
	    }
	  }

	  // Render each column
	  uint16_t xoffset, yoffset;
	  for (xoffset = 0; xoffset < font.u8Width; xoffset++)
	  {
	    for (yoffset = 0; yoffset < (font.u8Height + 1); yoffset++)
	    {
	      uint8_t bit = 0x00;
	      bit = (column[xoffset] << (font.u8Height - (yoffset + 1)));     // Shift current row bit left
	      bit = (bit >> 7);                     // Shift current row but right (results in 0x01 for black, and 0x00 for white)
	      if (bit)
	      {
	        ssd1306_alterPixel(x + xoffset, y + font.u8Height - yoffset, newVal);
	      }
	    }
	  }

	  // set the tmp structure for the last written pixels
	  tmp.x = x+font.u8Width;
	  tmp.y = y;

	  return tmp;
}
xypair_t
ssd1306_setString(uint8_t x, uint8_t y, const char* str, struct FONT_DEF font){
	return ssd1306_alterString(x,y,str,font,Bit_SET);
}

xypair_t
ssd1306_setStringCentered(uint8_t y, uint8_t x0, uint8_t x1, const char* str, struct FONT_DEF font){
	// calculate new starting x
	uint8_t length = strlen(str);
	length = (font.u8Width*length) + (length-1);
	uint8_t dX = MAX(x1,x0)-MIN(x1,x0);
	uint8_t prefix = (dX-length)/2;
	return ssd1306_alterString(x0+prefix,y,str,font,Bit_SET);
}
xypair_t
ssd1306_setStringBelowPreviousSameFont(xypair_t xy, uint8_t spacing, const char* str, struct FONT_DEF font){
	return ssd1306_alterString(xy.x,xy.y-font.u8Height-spacing,str,font,Bit_SET);
}
xypair_t
	ssd1306_setStringBelowPreviousDifferentFont(xypair_t xy, uint8_t spacing, const char* str, struct FONT_DEF previousFont, struct FONT_DEF currentFont){
	return ssd1306_alterString(xy.x,xy.y-previousFont.u8Height-spacing,str,currentFont,Bit_SET);
}
xypair_t
ssd1306_clearString(uint8_t x, uint8_t y, const char* str, struct FONT_DEF font){
	return ssd1306_alterString(x,y,str,font,Bit_RESET);
}
inline static xypair_t
ssd1306_alterString(uint8_t x, uint8_t y, const char* str, struct FONT_DEF font, BitAction newVal){
	  uint8_t l;
	  xypair_t tmp;
	  for (l = 0; l < strlen(str); l++)
	  {
		  tmp = ssd1306_alterCharacter(x + (l * (font.u8Width + 1)), y, str[l], font, newVal);
	  }
	  return tmp;
}
inline static void
ssd1306_alterStringBorder(uint8_t x, uint8_t y, const char* str, struct FONT_DEF font, uint8_t padding, BitAction newVal){
	uint8_t stringLength = strlen(str);
	uint8_t stringWidth = stringLength*font.u8Width+(stringLength-1);
	uint8_t stringHeight = font.u8Height;
		// x - padding ; (x+linewidth+padding)
		// y - padding ; (y+linewidth+padding)
		ssd1306_alterLine(x-padding, y-padding, x-padding, y+stringHeight+padding, newVal);	// left line
		ssd1306_alterLine(x-padding, y-padding, x+padding+stringWidth, y-padding, newVal);	// bottom line
		ssd1306_alterLine(x+stringWidth+padding, y-padding, x+stringWidth+padding, y+stringHeight+padding, newVal);	// right line
		ssd1306_alterLine(x-padding, y+padding+stringHeight, x+padding+stringWidth, y+stringHeight+padding, newVal);	// top line
}
void
ssd1306_setStringWithBorder(uint8_t x, uint8_t y, const char* str, struct FONT_DEF font, uint8_t padding){
	// set border
	ssd1306_alterStringBorder(x,y,str,font,padding,Bit_SET);
	// set string
	ssd1306_alterString(x,y,str,font,Bit_SET);
}
void
ssd1306_clearStringWithBorder(uint8_t x, uint8_t y, const char* str, struct FONT_DEF font, uint8_t padding){
	// clear border
	ssd1306_alterStringBorder(x,y,str,font,padding,Bit_RESET);
	// clear string
	ssd1306_alterString(x,y,str,font,Bit_RESET);
}
xypair_t
ssd1306_setStringInverted(uint8_t x, uint8_t y, const char* str, struct FONT_DEF font, uint8_t padding){
	// set Area
	uint8_t stringLength = strlen(str);
	uint8_t stringWidth = stringLength*font.u8Width+(stringLength-1);
	uint8_t stringHeigth = font.u8Height;
	ssd1306_setArea(x-padding, y-padding+1, x+padding+stringWidth, y+padding+stringHeigth);
	// clear string
	return ssd1306_alterString(x,y,str,font,Bit_RESET);
}
xypair_t
ssd1306_setStringInvertedCentered(uint8_t y, uint8_t x0, uint8_t x1, const char* str, struct FONT_DEF font, uint8_t padding){
	// calculate new starting x
	uint8_t length = strlen(str);
	length = (font.u8Width*length) + (length-1);
	uint8_t xMin = MIN(x1,x0);
	uint8_t xMax = MAX(x1,x0);
	uint8_t dX = xMax-xMin;
	uint8_t prefix = (dX-length)/2;
	ssd1306_setArea(xMin-padding, y-padding+1, xMax+padding, y+padding+font.u8Height);
	return ssd1306_alterString(xMin+prefix,y,str,font,Bit_RESET);
}
xypair_t
ssd1306_setTextBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const char* str, struct FONT_DEF font, uint32_t shiftLines){
// old version
	xycorners_t tmp = ssd1306_getAdjustedPoints(x0,y0,x1,y1);
	xypair_t returnVal;

	uint32_t maxColumns = (tmp.bottomRight.x - tmp.bottomLeft.x) / (font.u8Width+1);
	uint32_t maxLines = (tmp.topLeft.y - tmp.bottomLeft.y) / (font.u8Height+1);
	uint32_t totLength = strlen(str);
	// clear the text block
		//ssd1306_alterArea(tmp.bottomLeft.x, tmp.bottomLeft.y, tmp.topRight.x, tmp.topRight.y, Bit_RESET);
	// write every line until the string is printed
		  uint32_t c, l;
		  // total lines * height of a line
		  uint32_t i = shiftLines*maxColumns;

		  for(l = 1; l < maxLines+1; l++){
			  for (c = 0; c < maxColumns; c++)
			  {
				  // TODO: interpret text with \r or \n
				  if (str[i] == '\n'){
					  c=maxColumns;
					  i++;
				  } else if (str[i] == '\t'){
					  i++;
					  ssd1306_alterCharacter(tmp.topLeft.x + (c * (font.u8Width + 1)), tmp.topLeft.y - (l * (font.u8Height+1)), ' ', font, ENABLE);c++;
					  ssd1306_alterCharacter(tmp.topLeft.x + (c * (font.u8Width + 1)), tmp.topLeft.y - (l * (font.u8Height+1)), ' ', font, ENABLE);c++;
					  ssd1306_alterCharacter(tmp.topLeft.x + (c * (font.u8Width + 1)), tmp.topLeft.y - (l * (font.u8Height+1)), ' ', font, ENABLE);c++;
					  returnVal = ssd1306_alterCharacter(tmp.topLeft.x + (c * (font.u8Width + 1)), tmp.topLeft.y - (l * (font.u8Height+1)), ' ', font, ENABLE);
				  } else if(str[i]!=0x00 && i<totLength){
					  returnVal = ssd1306_alterCharacter(tmp.topLeft.x + (c * (font.u8Width + 1)), tmp.topLeft.y - (l * (font.u8Height+1)), str[i++], font, ENABLE);
				  } else { break; }
			  }
		  }
	return returnVal;
}
void
ssd1306_setCheckBoxWithText(uint8_t x, uint8_t y, const char* str, struct FONT_DEF font, uint8_t checked){
	y+=3;
	ssd1306_setOutlineRounded(x,y,x+8,y+8,2);
	if(checked >0) {
	ssd1306_setArea(x+2,y+2,x+6,y+7);}
	ssd1306_setString(x+((font.u8Width+1)*2),y-1,str,font);
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
ssd1306_setCircle(uint8_t x, uint8_t y, uint16_t radius){
	ssd1306_alterCircle(x, y, radius, Bit_SET);
}
void
ssd1306_clearCircle(uint8_t x, uint8_t y, uint16_t radius){
	ssd1306_alterCircle(x, y, radius, Bit_RESET);
}
inline static void
ssd1306_alterCircle(uint8_t x0, uint8_t y0, uint16_t radius, BitAction newVal){
	// reference : http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	  int x = radius;
	  int y = 0;
	  int radiusError = 1-x;

	  while(x >= y)
	  {
	    ssd1306_alterPixel( x + x0,  y + y0, newVal);
	    ssd1306_alterPixel( y + x0,  x + y0, newVal);
	    ssd1306_alterPixel(-x + x0,  y + y0, newVal);
	    ssd1306_alterPixel(-y + x0,  x + y0, newVal);
	    ssd1306_alterPixel(-x + x0, -y + y0, newVal);
	    ssd1306_alterPixel(-y + x0, -x + y0, newVal);
	    ssd1306_alterPixel( x + x0, -y + y0, newVal);
	    ssd1306_alterPixel( y + x0, -x + y0, newVal);
	    y++;
	    if (radiusError<0)
	    {
	      radiusError += 2 * y + 1;
	    }
	    else
	    {
	      x--;
	      radiusError += 2 * (y - x) + 1;
	    }
	  }
}
// semi-circles
void
ssd1306_setSemiCircle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t radius){

}
void
ssd1306_setSemiCircleOutline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t radius){
	// reference : http://en.wikipedia.org/wiki/Midpoint_circle_algorithm

	if(x0==x1 || y0==y1) return;	// donkey proofing

	xycorners_t tmp = ssd1306_getAdjustedPoints(x0,y0,x1,y1);
	uint8_t xComp = (x0>x1)?(1):(0);
	uint8_t yComp = (y0>y1)?(1):(0);
	uint8_t quadrant = xComp<<4 | yComp;

	int x = radius;
	int y = 0;
	int radiusError = 1-x;

	while(x >= y)
	{
		switch(quadrant){
			case 0x10: // Quadrant 1
				ssd1306_alterPixel(x + tmp.bottomLeft.x,  y + tmp.bottomLeft.y, Bit_SET); // 1
				ssd1306_alterPixel(y + tmp.bottomLeft.x,  x + tmp.bottomLeft.y, Bit_SET); // 2
				break;
			case 0x00: // Quadrant 2
				ssd1306_alterPixel(-x + tmp.bottomRight.x,  y + tmp.bottomRight.y, Bit_SET); // 3
				ssd1306_alterPixel(-y + tmp.bottomRight.x,  x + tmp.bottomRight.y, Bit_SET); // 4
				break;
			case 0x01: // Quadrant 3
				ssd1306_alterPixel(-x + tmp.topRight.x, -y + tmp.topRight.y, Bit_SET); // 5
				ssd1306_alterPixel(-y + tmp.topRight.x, -x + tmp.topRight.y, Bit_SET); // 6
				break;
			case 0x11: // Quadrant 4
				ssd1306_alterPixel(x + tmp.topLeft.x, -y + tmp.topLeft.y, Bit_SET); // 7
				ssd1306_alterPixel(y + tmp.topLeft.x, -x + tmp.topLeft.y, Bit_SET); // 8
				break;
		}
	  y++;
	  if (radiusError<0)
	  {
		radiusError += 2 * y + 1;
	  }
	  else
	  {
		x--;
		radiusError += 2 * (y - x) + 1;
	  }
	}
}
// area's
void
ssd1306_setOutline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
	xycorners_t tmp = ssd1306_getAdjustedPoints(x0,y0,x1,y1);
	ssd1306_alterLine(tmp.bottomLeft.x, tmp.bottomLeft.y, tmp.topLeft.x, tmp.topLeft.y, Bit_SET);
	ssd1306_alterLine(tmp.topLeft.x, tmp.topLeft.y, tmp.topRight.x, tmp.topRight.y, Bit_SET);
	ssd1306_alterLine(tmp.topRight.x, tmp.topRight.y, tmp.bottomRight.x, tmp.bottomRight.y, Bit_SET);
	ssd1306_alterLine(tmp.bottomLeft.x, tmp.bottomLeft.y, tmp.bottomRight.x, tmp.bottomRight.y, Bit_SET);
}
void
ssd1306_setOutlineRounded(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t radius){
	xycorners_t tmp = ssd1306_getAdjustedPoints(x0,y0,x1,y1);
	ssd1306_alterLine(tmp.bottomLeft.x, tmp.bottomLeft.y+radius, tmp.topLeft.x, tmp.topLeft.y-radius, Bit_SET);
	ssd1306_setSemiCircleOutline(tmp.topLeft.x, tmp.topLeft.y-radius, tmp.topLeft.x+radius, tmp.topLeft.y, radius);
	ssd1306_alterLine(tmp.topLeft.x+radius, tmp.topLeft.y, tmp.topRight.x-radius, tmp.topRight.y, Bit_SET);
	ssd1306_setSemiCircleOutline(tmp.topRight.x, tmp.topRight.y-radius, tmp.topRight.x-radius, tmp.topRight.y, radius);
	ssd1306_alterLine(tmp.topRight.x, tmp.topRight.y-radius, tmp.bottomRight.x, tmp.bottomRight.y+radius, Bit_SET);
	ssd1306_setSemiCircleOutline(tmp.bottomRight.x, tmp.bottomRight.y+radius, tmp.bottomRight.x-radius, tmp.bottomRight.y, radius);
	ssd1306_alterLine(tmp.bottomLeft.x+radius, tmp.bottomLeft.y, tmp.bottomRight.x-radius, tmp.bottomRight.y, Bit_SET);
	ssd1306_setSemiCircleOutline(tmp.bottomLeft.x, tmp.bottomLeft.y+radius, tmp.bottomLeft.x+radius, tmp.bottomLeft.y, radius);
}
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

	for (uint8_t x = xMin; x <= xMax; ++x) {
		for (uint8_t y = yMin; y < yMax; ++y) {
			ssd1306_alterPixel(x,y,newval);
		}
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
			// set viewmode state
			ssd1306_setViewMode(ssd1306_parameters.viewMode);

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
				0xF,
				0);

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
	DMA_DeInit(OLED_DMA_TX_DMAStream);
	DMA_initNormal(
			OLED_DMA_TX_DMAStream,
			OLED_DMA_TX_DMAChannel,
			OLED_DMA_PeripheralBaseAddr,
			(uint32_t)ssd1306_parameters.pixBuffer,
			OLED_DMA_TX_DIR,
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

