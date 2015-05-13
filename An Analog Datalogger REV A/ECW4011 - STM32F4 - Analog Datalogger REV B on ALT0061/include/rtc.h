/*
 * rtc.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef RTC_H_
#define RTC_H_

#include "stm32f4xx.h"


#include <stdio.h>

#include "projectconfig.h"
#include "rcc.h"
#include "delay.h"
#include "ssd1306.h"
#include "general.h"

extern RTC_TimeTypeDef  RTC_TimeStructure;
extern RTC_DateTypeDef RTC_DateStructure;

extern RTC_TimeTypeDef  RTC_TimeStructure_new;
extern RTC_DateTypeDef RTC_DateStructure_new;

void
initialize_RTC(void);

void
rtc_printTimeTerminal(void);

void
rtc_setTimeToOLED(void);

void
rtc_setTimestructure(timeRegistration_t* timeStructure, uint32_t timeInSeconds);

char*
rtc_getTimestructureString(timeRegistration_t* timeStructure);

#endif /* RTC_H_ */
