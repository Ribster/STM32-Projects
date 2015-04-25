/*
 * nvic.c
 *
 *  Created on: 15-apr.-2015
 *      Author: Robbe Van Assche
 */
#include "nvic.h"

// implementation
void
nvic_initInterrupt(uint8_t NVIC_IRQChannel,
		uint8_t NVIC_IRQChannelPreemptionPriority,
		uint8_t NVIC_IRQChannelSubPriority
		){
	NVIC_InitTypeDef tmp;
	tmp.NVIC_IRQChannel = NVIC_IRQChannel;
	tmp.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;
	tmp.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;
	tmp.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&tmp);

}
