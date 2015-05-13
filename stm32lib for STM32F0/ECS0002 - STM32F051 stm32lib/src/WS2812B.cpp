/*
 * WS2812B.cpp
 *
 *  Created on: 8-dec.-2014
 *      Author: Robbe
 */

#include "WS2812B.h"

namespace stm32lib {

WS2812B::WS2812B(IO_t* wsPin, uint32_t PixelCount, uint8_t* bufferptr) {
	// TODO Auto-generated constructor stub
  WSObj.OutputPin = wsPin;
  WSBitBuffer = bufferptr;
  WSObj.LEDCount = PixelCount;
}

WS2812B::~WS2812B() {
	// TODO Auto-generated destructor stub
	if(WSBitBuffer != NULL){
		free(WSBitBuffer);
	}
}

void WS2812B::begin(IOPin* Pin, TIM_TypeDef* TIMx, uint8_t TimerChannel, DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMACHANx){
  begin(Pin->PIN, TIMx, TimerChannel, DMAx, DMACHANx);
}

void WS2812B::begin(TIM_TypeDef* TIMx, uint8_t TimerChannel, DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMACHANx){
  begin(WSObj.OutputPin, TIMx, TimerChannel, DMAx, DMACHANx);
}

void WS2812B::begin(IO_t* Pin, TIM_TypeDef* TIMx, uint8_t TimerChannel, DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMACHANx){
	// make sure the internal structure is empty
	if(getInternalStructCheck() == true){
		WSObj.DMACHANx = NULL;
		WSObj.DMAx = NULL;
		WSObj.LEDCount = 0;
		WSObj.OutputPin = NULL;
		WSObj.TIMx = NULL;
	}

	// assign internal structure
		// some security
		if(DMACHANx == NULL || DMAx == NULL || Pin == NULL || TIMx == NULL || TimerChannel < 1 || TimerChannel > 4){
			return;
		}
		// assignment
		WSObj.DMACHANx = DMACHANx;
		WSObj.DMAx = DMAx;
		WSObj.OutputPin = Pin;
		WSObj.TIMx = TIMx;
		WSObj.timerChannel = TimerChannel;

	// make sure the IO pin is already initialized and that it's in AF mode
	if(Pin->init == false){
		return;
	} else if(Pin->GPIOInfo.GPIO_Mode != GPIO_Mode_AF){
		return;
	}

	// allocate memory for buffers
	//uint32_t sizebit = getBufferSize();
	//uint32_t sizecolor = getPixelCount()*3;
	//WSBitBuffer = new uint8_t[sizebit];
	//WSColorBuffer = new uint8_t[sizecolor];

	initDone = true;

	// enable clocks
	setRCCPeriphDMA(WSObj.DMAx, true);
	setRCCPeriphTIM(WSObj.TIMx, true);

	// init DMA
	initDMA();
	// init TIMER
	initTimer();
	// init TIMER OC
	initTimerOC();
	// init TIMER ARR
	initTimerARR();
	// init NVIC
	initNVIC();
	// init DMA IT
	initDMAIT();


}

bool WS2812B::getInitDone(void){
	return initDone;
}

bool WS2812B::getInternalStructCheck(void){
	if(WSObj.OutputPin == NULL){
		return false;
	}
	if(WSObj.DMACHANx == NULL){
		return false;
	}
	if(WSObj.DMAx == NULL){
		return false;
	}
	if(WSObj.TIMx == NULL){
		return false;
	}

	return (true);
}

void WS2812B::setRCCPeriphDMA(DMA_TypeDef* DMAx, bool newVal){
	FunctionalState TempNewVal = (newVal)?(ENABLE):(DISABLE);
	if(DMAx == DMA1){
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, TempNewVal);
	}

}

void WS2812B::setRCCPeriphTIM(TIM_TypeDef* TIMx, bool newVal){
	FunctionalState TempNewVal = (newVal)?(ENABLE):(DISABLE);
	if(TIMx == TIM1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, TempNewVal);
	} else if (TIMx == TIM2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, TempNewVal);
	} else if (TIMx == TIM3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, TempNewVal);
	} else if (TIMx == TIM6){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, TempNewVal);
	} else if (TIMx == TIM7){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, TempNewVal);
	} else if (TIMx == TIM14){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, TempNewVal);
	} else if (TIMx == TIM15){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, TempNewVal);
	} else if (TIMx == TIM16){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, TempNewVal);
	} else if (TIMx == TIM17){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, TempNewVal);
	}
}

uint32_t WS2812B::getDMAFlag(void){
	if(initDone == true){
		if(WSObj.DMACHANx == DMA1_Channel1){
			return DMA1_FLAG_TC1;
		} else if(WSObj.DMACHANx == DMA1_Channel2){
			return DMA1_FLAG_TC2;
		} else if(WSObj.DMACHANx == DMA1_Channel3){
			return DMA1_FLAG_TC3;
		} else if(WSObj.DMACHANx == DMA1_Channel4){
			return DMA1_FLAG_TC4;
		} else if(WSObj.DMACHANx == DMA1_Channel5){
			return DMA1_FLAG_TC5;
		} else {
			return 0;
		}
	}
	return 0;
}
uint32_t WS2812B::getTIMDIERVal(void){
	if(initDone == true){
		if(WSObj.timerChannel == 1){
			return TIM_DMA_CC1;
		} else if(WSObj.timerChannel == 2){
			return TIM_DMA_CC2;
		} else if(WSObj.timerChannel == 3){
			return TIM_DMA_CC3;
		} else if(WSObj.timerChannel == 4){
			return TIM_DMA_CC4;
		} else {
			return 0;
		}
	}
	return 0;
}

uint32_t WS2812B::getBufferSize(void){
	return (getPixelCount()*24)+PWM_DEADTIME;
}

void WS2812B::initDMA(void){
	DMA_InitTypeDef tempStruct;
	// deinit DMA channel
	DMA_DeInit(WSObj.DMACHANx);
	// fill in struct

	if(WSObj.timerChannel == 1){
		tempStruct.DMA_PeripheralBaseAddr = (uint32_t)&WSObj.TIMx->CCR1;
	} else if (WSObj.timerChannel == 2){
		tempStruct.DMA_PeripheralBaseAddr = (uint32_t)&WSObj.TIMx->CCR2;
	} else if (WSObj.timerChannel == 3){
		tempStruct.DMA_PeripheralBaseAddr = (uint32_t)&WSObj.TIMx->CCR3;
	} else if (WSObj.timerChannel == 4){
		tempStruct.DMA_PeripheralBaseAddr = (uint32_t)&WSObj.TIMx->CCR4;
	}
	tempStruct.DMA_MemoryBaseAddr = (uint32_t)WSBitBuffer;
	tempStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	tempStruct.DMA_BufferSize = (uint16_t)getBufferSize();
	tempStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	tempStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	tempStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	tempStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	tempStruct.DMA_Mode = DMA_Mode_Normal;
	tempStruct.DMA_Priority = DMA_Priority_VeryHigh;
	tempStruct.DMA_M2M = DMA_M2M_Disable;
	// init DMA channel
	DMA_Init(WSObj.DMACHANx, &tempStruct);
}
void WS2812B::initTimer(void){
	TIM_TimeBaseInitTypeDef tempStruct;
	// fill in struct
	tempStruct.TIM_Prescaler = 0;
	tempStruct.TIM_CounterMode = TIM_CounterMode_Up;
	tempStruct.TIM_Period = (SystemCoreClock / OscillatorFreq) - 1;
	tempStruct.TIM_ClockDivision = 0;
	tempStruct.TIM_RepetitionCounter = 0;
	// init TIMER
	TIM_TimeBaseInit(WSObj.TIMx, &tempStruct);

}
void WS2812B::initTimerOC(void){
	TIM_OCInitTypeDef tempStruct;
	// fill in struct
	tempStruct.TIM_OCMode = TIM_OCMode_PWM1;
	tempStruct.TIM_OutputState = TIM_OutputState_Enable;
	tempStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	tempStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	tempStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	tempStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	tempStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	tempStruct.TIM_Pulse = 0;
	// init TIMER OC
	if(WSObj.timerChannel == 1){
		TIM_OC1Init(WSObj.TIMx, &tempStruct);
		TIM_OC1PreloadConfig(WSObj.TIMx, TIM_OCPreload_Enable);
	} else if (WSObj.timerChannel == 2){
		TIM_OC2Init(WSObj.TIMx, &tempStruct);
		TIM_OC2PreloadConfig(WSObj.TIMx, TIM_OCPreload_Enable);
	} else if (WSObj.timerChannel == 3){
		TIM_OC3Init(WSObj.TIMx, &tempStruct);
		TIM_OC3PreloadConfig(WSObj.TIMx, TIM_OCPreload_Enable);
	} else if (WSObj.timerChannel == 4){
		TIM_OC4Init(WSObj.TIMx, &tempStruct);
		TIM_OC4PreloadConfig(WSObj.TIMx, TIM_OCPreload_Enable);
	}
}
void WS2812B::initTimerARR(void){
	TIM_ARRPreloadConfig(WSObj.TIMx, ENABLE);
	WSObj.TIMx->BDTR |= TIM_BDTR_MOE; // enable PWM outputs
}
void WS2812B::initNVIC(void){
	NVIC_InitTypeDef tempStruct;
	// fill in struct
	if(WSObj.DMACHANx == DMA1_Channel1){
		tempStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	} else if(WSObj.DMACHANx == DMA1_Channel2 || WSObj.DMACHANx == DMA1_Channel3){
		tempStruct.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
	} else if(WSObj.DMACHANx == DMA1_Channel4 || WSObj.DMACHANx == DMA1_Channel5){
		tempStruct.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;
	}
	tempStruct.NVIC_IRQChannelCmd = ENABLE;
	tempStruct.NVIC_IRQChannelPriority = 0;
	// init NVIC
	NVIC_Init(&tempStruct);
}
void WS2812B::initDMAIT(void){
	WSObj.DMAx->IFCR = getDMAFlag();
	DMA_ITConfig(WSObj.DMACHANx, DMA_IT_TC, ENABLE);
}

void WS2812B::setTimerCounter(bool newVal){
	if(newVal == true){
		WSObj.TIMx->CR1 |= TIM_CR1_CEN;
	} else {
		WSObj.TIMx->CR1 &= ~TIM_CR1_CEN;
	}
}

void WS2812B::setDMACC(bool newVal){
	if(newVal == true){
		WSObj.DMACHANx->CCR |= DMA_CCR_EN;
	} else {
		WSObj.DMACHANx->CCR &= ~DMA_CCR_EN;
	}
}

void WS2812B::show(void){
	interruptState = true;
	WSObj.DMACHANx->CNDTR = getBufferSize();
	WSObj.DMAx->IFCR = getDMAFlag();		// Clear DMA flag
	setDMACC(true); // Dmay_chx enable
	WSObj.TIMx->DIER |= getTIMDIERVal(); 	// TIM_DMACMD CCx
	setTimerCounter(true); // Enable timer counter
	while(interruptState == true);
}

void WS2812B::interruptHandler(void){
	if (DMA_GetFlagStatus(getDMAFlag()) != RESET){
		setDMACC(false);
		setTimerCounter(false);
		WSObj.TIMx->DIER &= ~getTIMDIERVal(); 		// TIM_DMACMD CC1 disable
		WSObj.DMAx->IFCR = (getDMAFlag());			// Clear DMA flag
		interruptState = false;
	}
}

void WS2812B::setPixelColor(uint32_t pixelNumber, uint32_t color, bool swap){
	if (pixelNumber > 0 && pixelNumber <= getPixelCount()){
		//uint8_t* ptr = WSColorBuffer;
		uint32_t newcolor;
		if(swap){
			newcolor = ((color & 0xFF0000)>>8) | ((color & 0xFF00)<<8) | ((color & 0xFF));
		}else{
			newcolor = color;
		}
		uint8_t* pwmptr = WSBitBuffer;
		int8_t i=23;
		//ptr += (pixelNumber-1)*3;
		//*(ptr) = (newcolor & 0xFF0000)>>16;
		//*(ptr+1) = (newcolor & 0xFF00)>>8;
		//*(ptr+2) = (newcolor & 0xFF);

		pwmptr += (24*(pixelNumber-1));
		while (i>15){
			(newcolor & (1<<i)) ? *pwmptr = PWM_VAL_HIGH : *pwmptr = PWM_VAL_LOW;
			pwmptr++;i--;
		}
		while (i>7){
			(newcolor & (1<<i)) ? *pwmptr = PWM_VAL_HIGH : *pwmptr = PWM_VAL_LOW;
			pwmptr++;i--;
		}
		while (i>-1){
			(newcolor & (1<<i)) ? *pwmptr = PWM_VAL_HIGH : *pwmptr = PWM_VAL_LOW;
			pwmptr++;i--;
		}

		pwmptr = WSBitBuffer;
		pwmptr += (24*(getPixelCount()));
		i = PWM_DEADTIME;
		while(i>0){
			*pwmptr = PWM_VAL_OFF;
			i--;pwmptr++;
		}
	}
}
void WS2812B::setAllColor(uint32_t color){
	uint32_t pixels = getPixelCount();
	while(pixels>0){
		setPixelColor(pixels--,color, true);
	}
}
void WS2812B::setRainbowScroll(DELAY* DelayObj,uint32_t delay){
	  uint16_t i, j;

	  for(j=0; j<256; j++) {
	    for(i=1; i<getPixelCount()+1; i++) {
	    	setPixelColor((uint8_t)i, getWheel((i+j) & 255),0);
	    }
	   show();
	   DelayObj->milli(delay);
	  }
}

uint32_t WS2812B::getWheel(uint8_t wheelPos){
	  if(wheelPos < 85) {
	   return getColor(wheelPos * 3, 255 - wheelPos * 3, 0);
	  } else if(wheelPos < 170) {
		  wheelPos -= 85;
	   return getColor(255 - wheelPos * 3, 0, wheelPos * 3);
	  } else {
		  wheelPos -= 170;
	   return getColor(0, wheelPos * 3, 255 - wheelPos * 3);
	  }
}
uint32_t WS2812B::getColor(uint8_t red, uint8_t green, uint8_t blue){
	return ((uint32_t)red << 16) | ((uint32_t)green <<  8) | blue;
}
uint32_t WS2812B::getTransmitTimeInMillis(void){
	uint32_t returnVal = 0;
	uint32_t periodTime = 1000000000 / OscillatorFreq;
	if(getInternalStructCheck() == true){
		returnVal = getBufferSize() * periodTime;
	}
	return returnVal;
}
uint32_t WS2812B::getPixelCount(void){
	return WSObj.LEDCount;
}

} /* namespace stm32lib */
