/*
 * rcc.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef RCC_H_
#define RCC_H_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"

// prototype
void
rcc_setGPIOClock(GPIO_TypeDef* GPIOx, FunctionalState newVal);

void
rcc_setUSARTClock(USART_TypeDef* USARTx, FunctionalState newVal);

void
rcc_setTIMClock(TIM_TypeDef* TIMx, FunctionalState newVal);

void
rcc_setSPIClock(SPI_TypeDef* SPIx, FunctionalState newVal);

void
rcc_setDMAClock(DMA_Stream_TypeDef* DMAx, FunctionalState newVal);

#endif /* RCC_H_ */
