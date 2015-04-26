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

extern volatile uint8_t afe_DMA_TX_Buffer[AFE_DMA_BufferSize];
extern volatile uint8_t afe_DMA_RX_Buffer[AFE_DMA_BufferSize];

// prototypes
void
initialize_AFE(void);

void
afe_read(void);

#endif /* AFE_H_ */
