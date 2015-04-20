/*
 * delay.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"

#include "rtc.h"

// constants
#define SYSTICK_FREQUENCY_HZ	1000

// variables
static volatile uint32_t delay_microBuffer;
static volatile uint64_t delay_totalBuffer;
extern uint32_t SystemCoreClock;

// prototypes

void
delay_milli(volatile uint32_t waitVal);
void
delay_setup(void);
void
delay_interruptHandler(void);
uint32_t
delay_getMillis(void);
uint32_t
delay_getMilliDifference(uint32_t t0, uint32_t t1);
uint32_t
delay_getMilliDifferenceSimple(uint32_t t0);


#endif /* DELAY_H_ */
