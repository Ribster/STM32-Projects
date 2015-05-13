/*
 * IO.cpp
 *
 *  Created on: 4-dec.-2014
 *      Author: Robbe
 */

#include "IO.h"


namespace stm32lib {

IO::IO() {
	// TODO Auto-generated constructor stub

}

IO::~IO() {
	// TODO Auto-generated destructor stub
}

void IO::begin(GPIO_TypeDef* GPIOx, uint16_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed){
  IO_t tmp;
  begin(&tmp, GPIOx, pin, mode, otype, pupd, speed);
}

void IO::begin(GPIO_TypeDef* GPIOx, uint16_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed, uint8_t GPIO_AF){
  IO_t tmp;
  begin(&tmp, GPIOx, pin, mode, otype, pupd, speed, GPIO_AF);
}

void IO::begin(IO_t* structObj, GPIO_TypeDef* GPIOx, uint16_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed){

	structObj->GPIOx = GPIOx;
	structObj->GPIOInfo.GPIO_Mode = mode;
	structObj->GPIOInfo.GPIO_OType = otype;
	structObj->GPIOInfo.GPIO_Pin = (1<<pin);
	structObj->GPIOInfo.GPIO_PuPd = pupd;
	structObj->GPIOInfo.GPIO_Speed = speed;

	begin(structObj);
}

void IO::begin(IO_t* structObj, GPIO_TypeDef* GPIOx, uint16_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed, uint8_t GPIO_AF){
  // alternate function setting
  if(mode == GPIO_Mode_AF){
	  GPIO_PinAFConfig(GPIOx, pin, GPIO_AF);
  }
  // preset user defined settings && initialize
  begin(structObj, GPIOx, pin, mode, otype, pupd, speed);
}

void IO::begin(IO_t* structObj){
  // set the clock on
  RCC_AHBPeriphClockCmd(getRCCPeriphGPIO(structObj->GPIOx), ENABLE);
  // initialize the gpio
  GPIO_Init(structObj->GPIOx, &structObj->GPIOInfo);
  structObj->init = true;
}

void IO::begin(IO_t* structObj, IOPin* ioClass){
  structObj->GPIOInfo.GPIO_Mode = ioClass->PIN->GPIOInfo.GPIO_Mode;
  structObj->GPIOInfo.GPIO_OType = ioClass->PIN->GPIOInfo.GPIO_OType;
  structObj->GPIOInfo.GPIO_Pin = ioClass->PIN->GPIOInfo.GPIO_Pin;
  structObj->GPIOInfo.GPIO_PuPd = ioClass->PIN->GPIOInfo.GPIO_PuPd;
  structObj->GPIOInfo.GPIO_Speed = ioClass->PIN->GPIOInfo.GPIO_Speed;

  structObj->GPIO_AF = ioClass->PIN->GPIO_AF;
  structObj->GPIOx = ioClass->PIN->GPIOx;

  if(structObj->GPIOInfo.GPIO_Mode == GPIO_Mode_AF){
      GPIO_PinAFConfig(structObj->GPIOx, structObj->GPIOInfo.GPIO_Pin, structObj->GPIOInfo.GPIO_Mode);
  }

  begin(structObj);
}

void IO::begin(IO_t* structObj, uint8_t GPIO_AF){
  // set the GPIO_AF
  if(structObj->GPIOInfo.GPIO_Mode == GPIO_Mode_AF){
      GPIO_PinAFConfig(structObj->GPIOx, structObj->GPIOInfo.GPIO_Pin, GPIO_AF);
  }
  begin(structObj);
}

void IO::begin(IOPin* pinClass){
  if(pinClass->PIN->GPIOInfo.GPIO_Mode == GPIO_Mode_AF){
      // alternate function
      GPIO_PinAFConfig(pinClass->PIN->GPIOx, pinClass->PIN->GPIOInfo.GPIO_Pin, pinClass->PIN->GPIO_AF);
      begin(pinClass->PIN);
  } else {
      begin(pinClass->PIN);
  }
}

uint32_t IO::getRCCPeriphGPIO(GPIO_TypeDef* GPIOx){
	if(GPIOx == GPIOA){
		return (RCC_AHBPeriph_GPIOA);
	} else if (GPIOx == GPIOB){
		return (RCC_AHBPeriph_GPIOB);
	} else if (GPIOx == GPIOC){
		return (RCC_AHBPeriph_GPIOC);
	} else if (GPIOx == GPIOD){
		return (RCC_AHBPeriph_GPIOD);
	} else if (GPIOx == GPIOE){
		return (RCC_AHBPeriph_GPIOE);
	} else if (GPIOx == GPIOF){
		return (RCC_AHBPeriph_GPIOF);
	}

	return 0;
}

void IO::setValue(IO_t* structObj, bool newVal){
  if(newVal == true){
	  GPIO_SetBits(structObj->GPIOx, structObj->GPIOInfo.GPIO_Pin);
  } else {
	  GPIO_ResetBits(structObj->GPIOx, structObj->GPIOInfo.GPIO_Pin);
  }
}
void IO::toggleValue(IO_t* structObj){
  if(getValue(structObj)){
	  GPIO_ResetBits(structObj->GPIOx, structObj->GPIOInfo.GPIO_Pin);
  } else {
	  GPIO_SetBits(structObj->GPIOx, structObj->GPIOInfo.GPIO_Pin);
  }
}

bool IO::getValue(IO_t* structObj){
  return GPIO_ReadInputDataBit(structObj->GPIOx, structObj->GPIOInfo.GPIO_Pin);
}

void IO::setValue(IOPin* structObj, bool newVal){
  setValue(structObj->PIN, newVal);
}
void IO::toggleValue(IOPin* structObj){
  toggleValue(structObj->PIN);
}

bool IO::getValue(IOPin* structObj){
  return getValue(structObj->PIN);
}

} /* namespace stm32lib */
