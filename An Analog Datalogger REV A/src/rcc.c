/*
 * rcc.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "rcc.h"

// implementation
void
rcc_setGPIOClock(GPIO_TypeDef* GPIOx, FunctionalState newVal){
  if(GPIOx == GPIOA){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, newVal);
  } else if (GPIOx == GPIOB){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, newVal);
  } else if (GPIOx == GPIOC){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, newVal);
  } else if (GPIOx == GPIOD){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, newVal);
  } else if (GPIOx == GPIOE){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, newVal);
  } else if (GPIOx == GPIOF){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, newVal);
  } else if (GPIOx == GPIOG){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, newVal);
  } else if (GPIOx == GPIOH){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, newVal);
  } else if (GPIOx == GPIOI){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, newVal);
  } else if (GPIOx == GPIOJ){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOJ, newVal);
  } else if (GPIOx == GPIOK){
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOK, newVal);
  }
}

void
rcc_setUSARTClock(USART_TypeDef* USARTx, FunctionalState newVal){
  // set selected clock for U(S)ART
  if(USARTx == USART1){
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, newVal);
  }
  else if(USARTx == USART2){
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, newVal);
  }
  else if(USARTx == USART3){
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, newVal);
  }
  else if(USARTx == UART4){
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, newVal);
  }
  else if(USARTx == UART5){
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, newVal);
  }
  else if(USARTx == USART6){
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, newVal);
  }
  else if(USARTx == UART7){
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, newVal);
  }
  else if(USARTx == UART8){
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, newVal);
  }
}

void
rcc_setTIMClock(TIM_TypeDef* TIMx, FunctionalState newVal){
	if(TIMx == TIM2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, newVal);
	} else if(TIMx == TIM3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, newVal);
	} else if(TIMx == TIM4){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, newVal);
	} else if(TIMx == TIM5){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, newVal);
	} else if(TIMx == TIM6){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, newVal);
	} else if(TIMx == TIM7){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, newVal);
	} else if(TIMx == TIM12){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, newVal);
	} else if(TIMx == TIM13){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, newVal);
	} else if(TIMx == TIM14){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, newVal);
	}
	else if(TIMx == TIM1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, newVal);
	} else if(TIMx == TIM8){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, newVal);
	} else if(TIMx == TIM9){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, newVal);
	} else if(TIMx == TIM10){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, newVal);
	} else if(TIMx == TIM11){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, newVal);
	}
}
