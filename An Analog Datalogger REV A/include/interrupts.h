/*
 * interrupts.h
 *
 *  Created on: 16-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "stm32f4xx.h"
#include "projectconfig.h"
#include "delay.h"
#include "pushbuttons.h"
#include "uart.h"
#include "encoders.h"
#include "ssd1306.h"
#include "sdio.h"

void
NMI_Handler(void);
void
HardFault_Handler(void);
void
MemManage_Handler(void);
void
BusFault_Handler(void);
void
UsageFault_Handler(void);
void
SVC_Handler(void);
void
DebugMon_Handler(void);
void
PendSV_Handler(void);

void
SysTick_Handler(void);
void
EXTI15_10_IRQHandler(void);
void
USART2_IRQHandler(void);
void
TIM8_UP_TIM13_IRQHandler(void);
void
SDIO_IRQHandler(void);
void
SD_SDIO_DMA_IRQHANDLER(void);


#endif /* INTERRUPTS_H_ */
