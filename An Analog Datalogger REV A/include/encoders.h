/*
 * encoders.h
 *
 *  Created on: 17-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef ENCODERS_H_
#define ENCODERS_H_

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "projectconfig.h"
#include "gpio.h"
#include "rcc.h"

void
initialize_encoderOne(void);
void
initialize_encoderTwo(void);

void
encoder_readEncoderOne(void);
void
encoder_readEncoderTwo(void);

int16_t
encoder_getEncoderOneDifference(void);
int16_t
encoder_getEncoderTwoDifference(void);

int16_t
encoder_getEncoderOneValue(void);
int16_t
encoder_getEncoderTwoValue(void);


#endif /* ENCODERS_H_ */
