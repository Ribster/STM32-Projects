/*
 * leds.h
 *
 *  Created on: 13-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef LEDS_PROJECT_H_
#define LEDS_PROJECT_H_
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "projectconfig.h"
#include "delay.h"
#include "gpio.h"

// prototypes
void
initialize_LEDS(void);
void
leds_ledTest(void);
void
leds_setLed(uint8_t ref,
		FunctionalState newState);
void
leds_toggleLed(uint8_t ref);



#endif /* LEDS_H_ */
