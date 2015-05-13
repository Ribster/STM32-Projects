/*
 * gpio.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "gpio.h"

// implementation
void
gpio_initStandard(GPIO_TypeDef *GPIOx,
		GPIOMode_TypeDef GPIOMode,
		GPIOOType_TypeDef GPIOOtype,
		uint32_t pin,
		GPIOPuPd_TypeDef GPIOPull,
		GPIOSpeed_TypeDef GPIOSpeed){
	rcc_setGPIOClock(GPIOx, ENABLE);
	GPIO_InitTypeDef tmp;
	tmp.GPIO_Mode = GPIOMode;
	tmp.GPIO_OType = GPIOOtype;
	tmp.GPIO_Pin = (1<<pin);
	tmp.GPIO_PuPd = GPIOPull;
	tmp.GPIO_Speed = GPIOSpeed;
	GPIO_Init(GPIOx, &tmp);
}

void
gpio_initAF(GPIO_TypeDef *GPIOx,
		GPIOMode_TypeDef GPIOMode,
		GPIOOType_TypeDef GPIOOtype,
		uint32_t pin,
		GPIOPuPd_TypeDef GPIOPull,
		GPIOSpeed_TypeDef GPIOSpeed,
		uint8_t GPIOAF){
	  // set alternate function
	  GPIO_PinAFConfig(GPIOx, pin&0x0F, GPIOAF);
	  // configure pin && pinclock
	  gpio_initStandard(GPIOx, GPIOMode, GPIOOtype, pin, GPIOPull, GPIOSpeed);
}

void
gpio_initInterruptEXTI(GPIO_TypeDef *GPIOx,
		uint32_t pin,
		EXTITrigger_TypeDef trigger,
		EXTIMode_TypeDef mode,
		uint8_t NVIC_IRQChannelPreemptionPriority,
		uint8_t NVIC_IRQChannelSubPriority){
	uint8_t nvic_IRQChannel;
	EXTI_InitTypeDef tmp;

	// enable the GPIO clock
	rcc_setGPIOClock(GPIOx, ENABLE);
	// enable the sysclock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	// enable exti line
	SYSCFG_EXTILineConfig(gpio_getEXTIPortSourceGPIO(GPIOx), pin&0x0F);
	// fill struct and initialize
	tmp.EXTI_Line = (1<<pin);
	tmp.EXTI_Mode = mode;
	tmp.EXTI_Trigger = trigger;
	tmp.EXTI_LineCmd = ENABLE;
	EXTI_Init(&tmp);
	// get IRQ channel
	switch(pin&0x0F){
		case 0: nvic_IRQChannel = EXTI0_IRQn; break;
		case 1: nvic_IRQChannel = EXTI1_IRQn; break;
		case 2: nvic_IRQChannel = EXTI2_IRQn; break;
		case 3: nvic_IRQChannel = EXTI3_IRQn; break;
		case 4: nvic_IRQChannel = EXTI4_IRQn; break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9: nvic_IRQChannel = EXTI9_5_IRQn; break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15: nvic_IRQChannel = EXTI15_10_IRQn; break;
	}
	// set the interrupt on
	nvic_initInterrupt(
			nvic_IRQChannel,
			NVIC_IRQChannelPreemptionPriority,
			NVIC_IRQChannelSubPriority);
}

uint8_t
gpio_getEXTIPortSourceGPIO(GPIO_TypeDef *GPIOx){
	if(GPIOx == GPIOA){
		return EXTI_PortSourceGPIOA;
	} else if(GPIOx == GPIOB){
		return EXTI_PortSourceGPIOB;
	} else if(GPIOx == GPIOC){
		return EXTI_PortSourceGPIOC;
	} else if(GPIOx == GPIOD){
		return EXTI_PortSourceGPIOD;
	} else if(GPIOx == GPIOE){
		return EXTI_PortSourceGPIOE;
	} else if(GPIOx == GPIOF){
		return EXTI_PortSourceGPIOF;
	} else if(GPIOx == GPIOG){
		return EXTI_PortSourceGPIOG;
	} else if(GPIOx == GPIOH){
		return EXTI_PortSourceGPIOH;
	} else if(GPIOx == GPIOI){
		return EXTI_PortSourceGPIOI;
	} else if(GPIOx == GPIOJ){
		return EXTI_PortSourceGPIOJ;
	} else if(GPIOx == GPIOK){
		return EXTI_PortSourceGPIOK;
	}
	return 0x00;
}
