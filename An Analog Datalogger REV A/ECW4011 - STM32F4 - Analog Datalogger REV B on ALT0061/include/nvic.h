/*
 * nvic.h
 *
 *  Created on: 15-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef NVIC_H_
#define NVIC_H_

#include "stm32f4xx.h"

// prototype
void
nvic_initInterrupt(uint8_t NVIC_IRQChannel,
		uint8_t NVIC_IRQChannelPreemptionPriority,
		uint8_t NVIC_IRQChannelSubPriority
		);

#endif /* NVIC_H_ */
