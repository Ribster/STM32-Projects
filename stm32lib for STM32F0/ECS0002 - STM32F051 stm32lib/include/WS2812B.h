/*
 * WS2812B.h
 *
 *  Created on: 8-dec.-2014
 *      Author: Robbe
 */

#ifndef WS2812B_H_
#define WS2812B_H_

#include "stm32f0xx_rcc.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_dma.h"
#include "stm32f0xx_gpio.h"
#include "IO.h"
#include "IOPin.h"
#include "UART.h"
#include "DELAY.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string>

namespace stm32lib {

typedef struct WS2812B_t{
	IO_t* OutputPin;
	TIM_TypeDef* TIMx;
	DMA_TypeDef* DMAx;
	DMA_Channel_TypeDef* DMACHANx;
	uint32_t LEDCount;
	uint8_t timerChannel;
} WS2812B_t;

class WS2812B {
public:
	WS2812B(IO_t* wsPin, uint32_t PixelCount, uint8_t* bufferptr);
	virtual ~WS2812B();

	void begin(IO_t* Pin, TIM_TypeDef* TIMx, uint8_t TimerChannel, DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMACHANx);
	void begin(IOPin* Pin, TIM_TypeDef* TIMx, uint8_t TimerChannel, DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMACHANx);
	void begin(TIM_TypeDef* TIMx, uint8_t TimerChannel, DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMACHANx);

	void show(void);

	void interruptHandler(void);

	void setPixelColor(uint32_t pixelNumber, uint32_t color, bool);
	void setAllColor(uint32_t color);
	void setRainbowScroll(DELAY*,uint32_t delay);

	uint32_t getWheel(uint8_t wheelPos);
	uint32_t getColor(uint8_t red, uint8_t green, uint8_t blue);
	uint32_t getTransmitTimeInMillis(void);
	uint32_t getPixelCount(void);



private:
	bool getInternalStructCheck(void);
	bool getInitDone(void);

	uint32_t getTIMDIERVal(void);
	uint32_t getBufferSize(void);

	void setTimerCounter(bool);
	void setDMACC(bool);
	uint32_t getDMAFlag(void);

	void setRCCPeriphDMA(DMA_TypeDef*, bool);
	void setRCCPeriphTIM(TIM_TypeDef*, bool);

	void initDMA(void);
	void initTimer(void);
	void initTimerOC(void);
	void initTimerARR(void);
	void initNVIC(void);
	void initDMAIT(void);

	WS2812B_t WSObj;
	const uint16_t
			PWM_VAL_OFF = 0,
			PWM_VAL_HIGH = 38,
			PWM_VAL_LOW = 19,
			PWM_DEADTIME = 41;
	const uint32_t
			OscillatorFreq = 800000u;
	uint8_t
		*WSBitBuffer = NULL; // holds separate bits for stream

	bool initDone = false;
public:
	//uint8_t
	//	*WSColorBuffer = NULL; // holds led color values
	volatile bool interruptState = false;
};

} /* namespace stm32lib */

#endif /* WS2812B_H_ */
