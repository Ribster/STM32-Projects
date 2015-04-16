/*
 * delay.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"

// constants
#define SYSTICK_FREQUENCY_HZ	1000

// variables
static volatile uint32_t delay_microBuffer;
extern uint32_t SystemCoreClock;

// prototypes

void
delay_milli(volatile uint32_t waitVal);
void
delay_setup(void);
void
delay_interruptHandler(void);


#endif /* DELAY_H_ */
