/*
 * initialization.h
 *
 *  Created on: 13-apr.-2015
 *      Author: Robbe Van Assche
 */


#ifndef INITIALIZATION_PROJECT_H_
#define INITIALIZATION_PROJECT_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "gpio.h"
#include "rcc.h"
#include "uart.h"
#include "leds.h"
#include "pushbuttons.h"
#include "projectconfig.h"
#include "encoders.h"
#include "rtc.h"
#include "ssd1306.h"
#include "rng.h"

// prototype
void
initialization(void);

#endif /* INITIALIZATION_H_ */
