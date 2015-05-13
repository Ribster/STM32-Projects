/*
 * OSCILLATOR.cpp
 *
 *  Created on: 4-dec.-2014
 *      Author: Robbe
 */

#include "OSCILLATOR.h"

namespace stm32lib {

OSCILLATOR::~OSCILLATOR() {
	// TODO Auto-generated destructor stub
}

void OSCILLATOR::begin(OSC_t* OSCStructObj, uint32_t oscFreq, TIM_TypeDef* TIMx, uint8_t channel){
	setRCCClock(TIMx, true);
	// prerequisites
	OSCStructObj->OscFreq = oscFreq;
	OSCStructObj->TIMx = TIMx;
	OSCStructObj->TIMOCChannel = channel;

	OSCStructObj->TIMInfo.TIM_ClockDivision = 0;
	OSCStructObj->TIMInfo.TIM_CounterMode = TIM_CounterMode_Up;
	OSCStructObj->TIMInfo.TIM_Period = SystemCoreClock / OSCStructObj->OscFreq;
	OSCStructObj->TIMInfo.TIM_Prescaler = 0;
	OSCStructObj->TIMInfo.TIM_RepetitionCounter = 0;

	OSCStructObj->TIMOCInfo.TIM_Pulse = (OSCStructObj->TIMInfo.TIM_Period/2);
	OSCStructObj->TIMOCInfo.TIM_OCMode = TIM_OCMode_PWM2;
	OSCStructObj->TIMOCInfo.TIM_OutputState = TIM_OutputState_Enable;
	OSCStructObj->TIMOCInfo.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_TimeBaseInit(OSCStructObj->TIMx, &OSCStructObj->TIMInfo);
	start(OSCStructObj);
	switch (OSCStructObj->TIMOCChannel) {
		case 1:
			TIM_OC1Init(OSCStructObj->TIMx, &OSCStructObj->TIMOCInfo);
			TIM_OC1PreloadConfig(OSCStructObj->TIMx, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(OSCStructObj->TIMx, &OSCStructObj->TIMOCInfo);
			TIM_OC2PreloadConfig(OSCStructObj->TIMx, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(OSCStructObj->TIMx, &OSCStructObj->TIMOCInfo);
			TIM_OC3PreloadConfig(OSCStructObj->TIMx, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(OSCStructObj->TIMx, &OSCStructObj->TIMOCInfo);
			TIM_OC4PreloadConfig(OSCStructObj->TIMx, TIM_OCPreload_Enable);
			break;
	}
}

void OSCILLATOR::begin(uint32_t oscFreq, TIM_TypeDef* TIMx, uint8_t channel){
  begin(_OBJ_OSCILLATOR_, oscFreq, TIMx, channel);
}

void OSCILLATOR::start(OSC_t* OSCStructObj){
  TIM_Cmd(OSCStructObj->TIMx, ENABLE);
}
void OSCILLATOR::stop(OSC_t* OSCStructObj){
  TIM_Cmd(OSCStructObj->TIMx, DISABLE);
}

void OSCILLATOR::start(void){
  start(_OBJ_OSCILLATOR_);
}
void OSCILLATOR::stop(void){
  stop(_OBJ_OSCILLATOR_);
}

void OSCILLATOR::setRCCClock(TIM_TypeDef* TIMx, bool newVal){
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

} /* namespace stm32lib */
