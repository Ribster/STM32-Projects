/*
 * uart.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "uart.h"

// implementation
void
initialize_UART(void){
	// initialize the GPIO's
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;


	gpio_initAF(
			UART2_PORT_RX,
			UART2_MODE_RX,
			UART2_OTYPE_RX,
			UART2_PIN_RX,
			UART2_PULL_RX,
			UART2_SPEED_RX,
			UART2_AF_RX);
	gpio_initAF(
			UART2_PORT_TX,
			UART2_MODE_TX,
			UART2_OTYPE_TX,
			UART2_PIN_TX,
			UART2_PULL_TX,
			UART2_SPEED_TX,
			UART2_AF_TX);
	rcc_setUSARTClock(TERMINAL_UART, ENABLE);

	USART_InitStruct.USART_BaudRate = TERMINAL_BAUDRATE;				// the baudrate is set to the value we passed into this init function
	USART_InitStruct.USART_WordLength = TERMINAL_WORDLENGTH;// we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = TERMINAL_STOPBITS;		// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = TERMINAL_PARITY;		// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = TERMINAL_FLOWCONTROL; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = TERMINAL_MODE; // we want to enable the transmitter and the receiver
	USART_Init(TERMINAL_UART, &USART_InitStruct);

	USART_ITConfig(TERMINAL_UART, TERMINAL_IT_RX, ENABLE); // enable the USARTx receive interrupt

	NVIC_InitStructure.NVIC_IRQChannel = TERMINAL_UART_IRQN;		 // we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TERMINAL_IT_RX_IRQChannelPreemptionPriority;// this sets the priority group of the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TERMINAL_IT_RX_IRQChannelSubPriority;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(TERMINAL_UART, ENABLE);

	// set the buffer length to 0
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    printf("\r\nTesting uart after initialization!!\r\n");
}

void
uart_interruptHandlerTerminal(void){

}
