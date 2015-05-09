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
#ifdef DBG
	printf("Hardfault!!");
#endif
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
#ifdef DBG
	printf("Memory Manage exception!!");
#endif
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
#ifdef DBG
	printf("Busfault!!");
#endif
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
#ifdef DBG
	printf("UsageFault!!");
#endif
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
#ifdef DBG
	printf("SVC Handler!!");
#endif
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
#ifdef DBG
	printf("Debug Monitor!!");
#endif
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
#ifdef DBG
	printf("PendSVC!!");
#endif
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
	//printf("EXTI interrupt! \r\n");
	pushbuttons_interruptHandler_UP();
	pushbuttons_interruptHandler_DOWN();
	pushbuttons_interruptHandler_LEFT();
	pushbuttons_interruptHandler_RIGHT();
	pushbuttons_interruptHandler_MIDDLE();
	pushbuttons_interruptHandler_ENTER();
}
void
USART2_IRQHandler(void){
#ifdef DBG
	printf("USART2 interrupt! \r\n");
#endif
	uart_interruptHandlerTerminal();
}
void
TIM8_UP_TIM13_IRQHandler(void){
#ifdef DBGIO
	printf("ssd1306 interrupt! \r\n");
#endif
	ssd1306_interruptHandler();
}

void
SDIO_IRQHandler(void)
{
#ifdef DBGIO
	printf("SDIO interrupt! \r\n");
#endif
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
#ifdef DBGIO
	printf("SDIO DMA interrupt! \r\n");
#endif
	SD_ProcessDMAIRQ();
}

// AFE_DMA_RX
void
DMA2_Stream2_IRQHandler(void){
#ifdef DBGIO
	printf("AFE DMA RX interrupt! \r\n");
#endif
	if(DMA_GetITStatus(AFE_DMA_RX_DMAStream, DMA_IT_TCIF2) == SET){
#ifdef DBGIO
		printf(" RX TC\r\n");
#endif
		DMA_ClearFlag(AFE_DMA_RX_DMAStream, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(AFE_DMA_RX_DMAStream, DMA_IT_TCIF2);
		DMA_ITConfig(AFE_DMA_RX_DMAStream, DMA_IT_TC, DISABLE);

		/* Enable DMA SPI RX Stream */
		DMA_Cmd(AFE_DMA_RX_DMAStream,DISABLE);

		SPI_I2S_DMACmd(AFE_SPI, SPI_I2S_DMAReq_Rx, DISABLE);
	}
}

// AFE_DMA_TX
void
DMA2_Stream3_IRQHandler(void){
#ifdef DBGIO
	printf("AFE DMA TX interrupt! \r\n");
#endif
	if(DMA_GetITStatus(AFE_DMA_TX_DMAStream, DMA_IT_TCIF3) == SET){
#ifdef DBGIO
		printf(" TX TC\r\n");
#endif
		DMA_ClearFlag(AFE_DMA_TX_DMAStream, DMA_FLAG_TCIF3);
		DMA_ClearITPendingBit(AFE_DMA_TX_DMAStream, DMA_IT_TCIF3);
		DMA_ITConfig(AFE_DMA_TX_DMAStream, DMA_IT_TC, DISABLE);

		/* Enable DMA SPI TX Stream */
		DMA_Cmd(AFE_DMA_TX_DMAStream,DISABLE);

		SPI_I2S_DMACmd(AFE_SPI, SPI_I2S_DMAReq_Tx, DISABLE);

		while (AFE_DMA_RX_DMAStream->CR & DMA_SxCR_EN);
		while (AFE_DMA_TX_DMAStream->CR & DMA_SxCR_EN);

		SPI_Cmd(AFE_SPI, DISABLE);

		//GPIO_WriteBit(AFE_CS_PORT, AFE_CS_PIN, Bit_SET);

		afe_busy = 0x00;
			leds_setLed(ledList_Blue, DISABLE);

		afe_endReadout();


	}
}

void OTG_FS_IRQHandler(void)
{
	//printf("OTG interrupt! \r\n");
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}

void OTG_FS_WKUP_IRQHandler(void)
{
	printf("OTG wakeup interrupt! \r\n");
  if(USB_OTG_dev.cfg.low_power)
  {
    *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ;
    SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);
}
