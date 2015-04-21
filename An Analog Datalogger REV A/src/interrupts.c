/*
 * interrupts.c
 *
 *  Created on: 16-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "interrupts.h"

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	printf("Hardfault!!");
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
	printf("Memory Manage exception!!");
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
	printf("Busfault!!");
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
	printf("UsageFault!!");
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
	printf("SVC Handler!!");
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
	printf("Debug Monitor!!");
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
	printf("PendSVC!!");
}

void
SysTick_Handler(void){
	// the delay function
	delay_interruptHandler();
	encoder_readEncoderOne();
	encoder_readEncoderTwo();
}

void
EXTI15_10_IRQHandler(void){
	pushbuttons_interruptHandler_UP();
	pushbuttons_interruptHandler_DOWN();
	pushbuttons_interruptHandler_LEFT();
	pushbuttons_interruptHandler_RIGHT();
	pushbuttons_interruptHandler_MIDDLE();
	pushbuttons_interruptHandler_ENTER();
}
void
USART2_IRQHandler(void){
	uart_interruptHandlerTerminal();
}
void
TIM8_UP_TIM13_IRQHandler(void){
	ssd1306_interruptHandler();
}

/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void
SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

/**
  * @brief  This function handles DMA2 Stream3 or DMA2 Stream6 global interrupts
  *         requests.
  * @param  None
  * @retval None
  */
void
SD_SDIO_DMA_IRQHANDLER(void){
	SD_ProcessDMAIRQ();
}
