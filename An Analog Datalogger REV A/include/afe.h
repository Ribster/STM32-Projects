/*
 * afe.h
 *
 *  Created on: 21-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef AFE_H_
#define AFE_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#include "projectconfig.h"

#include "gpio.h"
#include "rcc.h"
#include "dma.h"
#include "nvic.h"
#include "delay.h"

#include "string.h"

#include <stdio.h>

extern volatile uint8_t afe_DMA_RX_Buffer[AFE_DMA_CAPTURES][AFE_DMA_BLOCKS][AFE_DMA_BLOCKSIZE];
extern volatile uint8_t afe_DMA_TX_Buffer[AFE_DMA_BLOCKS][AFE_DMA_BLOCKSIZE];

extern volatile uint8_t afe_busy;
extern volatile uint8_t afe_currentPacket;
extern volatile uint8_t afe_currentCaptureTransmission;
extern volatile uint8_t afe_currentCaptureLogging;

extern DMA_InitTypeDef afe_struct;


// prototypes
void
initialize_AFE(void);

void
afe_read(void);

void
afe_startReadout(void);

void
afe_endReadout(void);

void
afe_fileWrite(void);

#endif /* AFE_H_ */
