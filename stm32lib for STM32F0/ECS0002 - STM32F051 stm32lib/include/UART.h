/*
 * UART.h
 *
 *  Created on: 5-dec.-2014
 *      Author: Robbe
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f0xx_usart.h"
#include "stm32f0xx_rcc.h"
#include "IO.h"
#include "IOPin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>

namespace stm32lib {

typedef struct UART_t{
	uint8_t IRQnumber;
	USART_TypeDef* USARTx;
	USART_InitTypeDef settings;
	bool enable_IT_TXE;
	bool enable_IT_RXNE;
}UART_t;

class UART {
public:
	UART();
	UART(UART_t*, IO*);
	virtual ~UART();

	void begin(UART_t*, USART_TypeDef* USARTx, uint32_t baudrate, IO_t* rx, IO_t* tx);
	void begin(UART_t*, USART_TypeDef* USARTx, uint32_t baudrate, IOPin* rx, IOPin* tx);
	void begin(USART_TypeDef* USARTx, uint32_t baudrate, IO_t* rx, IO_t* tx);
	void begin(USART_TypeDef* USARTx, uint32_t baudrate, IOPin* rx, IOPin* tx);
	void setDefault(UART_t*);

	void setRCCClockUSART(USART_TypeDef*, bool newValue);

	void print(char c);
	void print(std::string);
	void println(std::string);
	void print(int16_t);
	void printbool(uint8_t);
	void printb(bool);
private:
	UART_t* defaultStream = NULL;
	IO* defaultIO = NULL;
};

} /* namespace stm32lib */

#endif /* UART_H_ */
