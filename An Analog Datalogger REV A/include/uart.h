/*
 * uart.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "gpio.h"
#include "rcc.h"
#include "nvic.h"
#include "projectconfig.h"

#include <stdio.h>

// prototypes
void
initialize_UART(void);

void
uart_interruptHandlerTerminal(void);

void
uart_sendString(
		USART_TypeDef* USARTx,
		volatile char *s);



#endif /* UART_H_ */
