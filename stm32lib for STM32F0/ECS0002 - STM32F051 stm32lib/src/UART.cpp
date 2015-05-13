/*
 * UART.cpp
 *
 *  Created on: 5-dec.-2014
 *      Author: Robbe
 */

#include "UART.h"

namespace stm32lib {

UART::UART() {
	// TODO Auto-generated constructor stub

}

UART::UART(UART_t* defaultUART, IO* IOObj) {
	// TODO Auto-generated constructor stub
  setDefault(defaultUART);
  defaultIO = IOObj;
}



UART::~UART() {
	// TODO Auto-generated destructor stub
}


void UART::begin(UART_t* UARTStructObj, USART_TypeDef* USARTx, uint32_t baudrate, IOPin* rx, IOPin* tx){
  if(rx->PIN->init == false){
      defaultIO->begin(rx);
  }
  if(tx->PIN->init == false){
      defaultIO->begin(tx);
  }
  begin(UARTStructObj, USARTx, baudrate, rx->PIN, tx->PIN);
}
void UART::begin(UART_t* UARTStructObj, USART_TypeDef* USARTx, uint32_t baudrate, IO_t* rx, IO_t* tx){
	// prerequisites
	UARTStructObj->USARTx = USARTx;
	UARTStructObj->settings.USART_BaudRate = baudrate;

	UARTStructObj->settings.USART_WordLength = USART_WordLength_8b;
	UARTStructObj->settings.USART_StopBits = USART_StopBits_1;
	UARTStructObj->settings.USART_Parity = USART_Parity_No;
	UARTStructObj->settings.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UARTStructObj->settings.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	if(rx->init && tx->init){
		setRCCClockUSART(UARTStructObj->USARTx, true);
		USART_Init(UARTStructObj->USARTx, &UARTStructObj->settings);
		USART_Cmd(UARTStructObj->USARTx, ENABLE);
		NVIC_InitTypeDef NVIC_InitStructure;
		if(UARTStructObj->enable_IT_RXNE || UARTStructObj->enable_IT_TXE){
			NVIC_InitStructure.NVIC_IRQChannel = UARTStructObj->IRQnumber;
			NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}

		if(UARTStructObj->enable_IT_RXNE){
			USART_ITConfig(UARTStructObj->USARTx, USART_IT_RXNE, ENABLE);
		}
		if(UARTStructObj->enable_IT_TXE){
			USART_ITConfig(UARTStructObj->USARTx, USART_IT_TXE, ENABLE);
		}
		if(defaultStream != NULL){
			printf("\r\n\n\n\n\n\n\n\n\n[UART]\tInitialization of ");
			if(defaultStream->USARTx == USART1){
				printf("UART1...");
			} else if(defaultStream->USARTx == USART2){
				printf("UART2...");
			} else if(defaultStream->USARTx == USART3){
				printf("UART3...");
			} else if(defaultStream->USARTx == USART4){
				printf("UART4...");
			}
			printf("...Done\r\n");
		}

	}

}

void UART::begin(USART_TypeDef* USARTx, uint32_t baudrate, IO_t* rx, IO_t* tx){
	if(defaultStream != NULL){
		begin(defaultStream, USARTx, baudrate, rx, tx);
	}
}

void UART::begin(USART_TypeDef* USARTx, uint32_t baudrate, IOPin* rx, IOPin* tx){
  if(rx->PIN->init == false){
      defaultIO->begin(rx);
  }
  if(tx->PIN->init == false){
      defaultIO->begin(tx);
  }
  if(defaultStream != NULL){
	  begin(defaultStream, USARTx, baudrate, rx->PIN, tx->PIN);
  }
}

void UART::setDefault(UART_t* UARTObj){
	defaultStream = UARTObj;
}

void UART::setRCCClockUSART(USART_TypeDef* USARTx, bool newValue){
	FunctionalState tempState = (newValue)?(ENABLE):(DISABLE);
	if(USARTx == USART1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, tempState);
	} else if (USARTx == USART2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, tempState);
	} else if (USARTx == USART3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, tempState);
	} else if (USARTx == USART4){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART4, tempState);
	}
}

void UART::print(char c){
	if(defaultStream != NULL){
		while(USART_GetFlagStatus(defaultStream->USARTx, USART_FLAG_TXE) != SET);
		USART_SendData(defaultStream->USARTx, c);
	}
}
void UART::print(std::string str){
	uint32_t i;
	for(i=0; i<str.size(); i++){
		print(str.at(i));
	}
}

void UART::println(std::string str){
	print(str);
	print((char*)"\r\n");
}
void UART::print(int16_t dec){
	char buffer[10];
	snprintf(buffer,10,"%d",dec);
	print(buffer);
}

void UART::printb(bool state){
	if(state){
		print("true");
	}else{
		print("false");
	}
}

void UART::printbool(uint8_t dec){
	uint8_t i;
	for(i=8;i>0;i--){
		(dec & (1<<(i-1))) ? (print("1")) : (print("0"));
	}
}

} /* namespace stm32lib */
