/*
 * RCCConfig.cpp
 *
 *  Created on: 26-nov.-2014
 *      Author: Robbe
 */

#include "RCCConfig.h"

namespace stm32lib {

RCCConfig::RCCConfig() {
	// TODO Auto-generated constructor stub

}

RCCConfig::~RCCConfig() {
	// TODO Auto-generated destructor stub
}

uint32_t RCCConfig::getRCCPeriphUSART(USART_TypeDef* USARTx){
	if(USARTx == USART1){
		return RCC_APB2Periph_USART1;
	} else if (USARTx == USART2){
		return RCC_APB1Periph_USART2;
	} else if (USARTx == USART3){
		return RCC_APB1Periph_USART3;
	} else if (USARTx == USART4){
		return RCC_APB1Periph_USART4;
	}
	return 0;
}
uint32_t RCCConfig::getRCCPeriphTIMER(TIM_TypeDef* TIMx){
	if(TIMx == TIM1){
		return (RCC_APB2Periph_TIM1);
	} else if(TIMx == TIM2){
		return (RCC_APB1Periph_TIM2);
	} else if(TIMx == TIM3){
		return (RCC_APB1Periph_TIM3);
	} else if(TIMx == TIM6){
		return (RCC_APB1Periph_TIM6);
	} else if(TIMx == TIM7){
		return (RCC_APB1Periph_TIM7);
	} else if(TIMx == TIM14){
		return (RCC_APB1Periph_TIM14);
	} else if(TIMx == TIM15){
		return (RCC_APB2Periph_TIM15);
	} else if(TIMx == TIM16){
		return (RCC_APB2Periph_TIM16);
	} else if(TIMx == TIM17){
		return (RCC_APB2Periph_TIM17);
	}
	return 0;
}

uint32_t RCCConfig::getRCCPeriphSPI(SPI_TypeDef* SPIx){
	if(SPIx == SPI1){
		return (RCC_APB2Periph_SPI1);
	} else if (SPIx == SPI2){
		return (RCC_APB1Periph_SPI2);
	}
	return 0;
}

void RCCConfig::SystemCoreClockHSIPLL2(uint32_t RCC_PLLMul_x) {
	RCC_DeInit();										//reset rcc
	RCC_PLLCmd(DISABLE);								//disable PLL
	RCC_HSICmd(ENABLE);									//enable hsi;
	RCC_HCLKConfig(RCC_SYSCLK_Div1);					//set sysclk divider
	//RCC_PCLK1Config(RCC_HCLK_Div1);						//set pclk1/2 dividers
	//RCC_PCLK2Config(RCC_HCLK_Div1);
	/**

	* @brief  Configures the PLL clock source and multiplication factor.
	* @note   This function must be used only when the PLL is disabled.
	*
	* @param  RCC_PLLSource: specifies the PLL entry clock source.
	*          This parameter can be one of the following values:
	*            @arg RCC_PLLSource_HSI_Div2: HSI oscillator clock selected as PLL clock source
	*            @arg RCC_PLLSource_PREDIV1: PREDIV1 clock selected as PLL clock entry
	*            @arg RCC_PLLSource_HSI48 HSI48 oscillator clock selected as PLL clock source, applicable only for STM32F072 devices
	*            @arg RCC_PLLSource_HSI: HSI clock selected as PLL clock entry, applicable only for STM32F072 devices
	* @note   The minimum input clock frequency for PLL is 2 MHz (when using HSE as
	*         PLL source).
	*
	* @param  RCC_PLLMul: specifies the PLL multiplication factor, which drive the PLLVCO clock
	*          This parameter can be RCC_PLLMul_x where x:[2,16]
	*
	* @retval None
	*/
	RCC_PLLConfig(RCC_CFGR_PLLSRC_HSI_Div2, RCC_PLLMul_x);	//configure pll / divider. _x=[2..16]
	RCC_PLLCmd(ENABLE);										//enable pll
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) continue;	//wait for pll to be ready
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);				//set pll as sysclk
	while (RCC_GetSYSCLKSource() != RCC_CFGR_SWS_PLL/*0x08*/) continue;		//wait for PLL to be ready

	SystemCoreClockUpdate();								//update SystemCoreClock
}

} /* namespace stm32lib */
