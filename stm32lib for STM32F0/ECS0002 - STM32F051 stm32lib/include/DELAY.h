/*
 * DELAY.h
 *
 *  Created on: 4-dec.-2014
 *      Author: Robbe
 */
#include <stdio.h>
#include "stm32f0xx_rcc.h"
#include "stm32f0xx.h"

#ifndef DELAY_H_
#define DELAY_H_

namespace stm32lib {

typedef enum delaySelection{
	DELAY_Milliseconds
}delaySelection;

class DELAY {
public:
	// constructor && destructor
	DELAY(volatile uint32_t*);
	virtual ~DELAY();
	// initializing functions
	void begin(delaySelection);

	// interrupt function
	void interruptHandler(void);

	// timing functions
		// gets
		uint32_t getMillis(void);
		uint32_t getMicros(void);
		// sets
		void setMillis(uint32_t);
		// starts
		void startMilli(uint32_t);
		void milli(uint32_t);
		void delayMicro(uint32_t);
		void delayMilli(uint32_t);
		void waitDelayMicro(uint32_t startTime, uint32_t delayVal);
		void waitDelayMilli(uint32_t startTime, uint32_t delayVal);
		// stops
		void stop(void);

private:
	volatile uint32_t* delayStorage;
	volatile uint32_t timingStorage = 0;
	bool milliSeconds = false;
};

} /* namespace stm32lib */

#endif /* DELAY_H_ */
