/*
 * gpio.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_usart.h"

#include "rcc.h"
#include "nvic.h"

// prototypes
void
gpio_initStandard(GPIO_TypeDef *GPIOx,
		GPIOMode_TypeDef GPIOMode,
		GPIOOType_TypeDef GPIOOtype,
		uint32_t pin,
		GPIOPuPd_TypeDef GPIOPull,
		GPIOSpeed_TypeDef GPIOSpeed);

void
gpio_initAF(GPIO_TypeDef *GPIOx,
		GPIOMode_TypeDef GPIOMode,
		GPIOOType_TypeDef GPIOOtype,
		uint32_t pin,
		GPIOPuPd_TypeDef GPIOPull,
		GPIOSpeed_TypeDef GPIOSpeed,
		uint8_t GPIOAF);

void
gpio_initInterruptEXTI(GPIO_TypeDef *GPIOx,
		uint32_t pin,
		EXTITrigger_TypeDef trigger,
		EXTIMode_TypeDef mode,
		uint8_t NVIC_IRQChannelPreemptionPriority,
		uint8_t NVIC_IRQChannelSubPriority);

uint8_t
gpio_getEXTIPortSourceGPIO(GPIO_TypeDef *GPIOx);

#endif /* GPIO_H_ */
