/*
 * RCCConfig.h
 *
 *  Created on: 26-nov.-2014
 *      Author: Robbe
 */

#ifndef RCCCONFIG_H_
#define RCCCONFIG_H_

#include "stm32f0xx_rcc.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_spi.h"

namespace stm32lib {

class RCCConfig {
public:
	RCCConfig();
	virtual ~RCCConfig();

	void SystemCoreClockHSIPLL2(uint32_t RCC_PLLMul_x);
	uint32_t getRCCPeriphUSART(USART_TypeDef*);
	uint32_t getRCCPeriphTIMER(TIM_TypeDef*);
	uint32_t getRCCPeriphSPI(SPI_TypeDef*);
};

} /* namespace stm32lib */

#endif /* RCCCONFIG_H_ */
