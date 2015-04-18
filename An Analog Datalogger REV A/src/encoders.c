/*
 * encoders.c
 *
 *  Created on: 17-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "encoders.h"

// prototypes
static void
encoder_resetEncoderOne(void);
static void
encoder_resetEncoderTwo(void);

// variables used to hold the information.
// get parameters through functions

static volatile int16_t encoder_parameterEncoderOneDifference;
static volatile int16_t encoder_parameterEncoderTwoDifference;

static volatile int16_t encoder_parameterEncoderOneNew;
static volatile int16_t encoder_parameterEncoderOneOld;

static volatile int16_t encoder_parameterEncoderTwoNew;
static volatile int16_t encoder_parameterEncoderTwoOld;

void
initialize_encoderOne(void){
	rcc_setGPIOClock(ENC_1_A_PORT, ENABLE);
	gpio_initAF(
			ENC_1_A_PORT,
			ENC_1_A_MODE,
			ENC_1_A_OTYPE,
			ENC_1_A_PIN,
			ENC_1_A_PULL,
			ENC_1_A_SPEED,
			ENC_1_A_AF);
	rcc_setGPIOClock(ENC_1_B_PORT, ENABLE);
	gpio_initAF(
			ENC_1_B_PORT,
			ENC_1_B_MODE,
			ENC_1_B_OTYPE,
			ENC_1_B_PIN,
			ENC_1_B_PULL,
			ENC_1_B_SPEED,
			ENC_1_B_AF);
	rcc_setTIMClock(ENC_1_TIMER, ENABLE);
	TIM_EncoderInterfaceConfig (ENC_1_TIMER, TIM_EncoderMode_TI12,
								TIM_ICPolarity_Rising,
								TIM_ICPolarity_Rising);
	TIM_SetAutoreload (ENC_1_TIMER, 0xffff);
	TIM_Cmd (ENC_1_TIMER, ENABLE);
	encoder_resetEncoderOne();
}

void
initialize_encoderTwo(void){
	rcc_setGPIOClock(ENC_2_A_PORT, ENABLE);
	gpio_initAF(
			ENC_2_A_PORT,
			ENC_2_A_MODE,
			ENC_2_A_OTYPE,
			ENC_2_A_PIN,
			ENC_2_A_PULL,
			ENC_2_A_SPEED,
			ENC_2_A_AF);
	rcc_setGPIOClock(ENC_2_B_PORT, ENABLE);
	gpio_initAF(
			ENC_2_B_PORT,
			ENC_2_B_MODE,
			ENC_2_B_OTYPE,
			ENC_2_B_PIN,
			ENC_2_B_PULL,
			ENC_2_B_SPEED,
			ENC_2_B_AF);
	rcc_setTIMClock(ENC_2_TIMER, ENABLE);
	TIM_EncoderInterfaceConfig (ENC_2_TIMER, TIM_EncoderMode_TI12,
								TIM_ICPolarity_Rising,
								TIM_ICPolarity_Rising);
	TIM_SetAutoreload (ENC_2_TIMER, 0xffff);
	TIM_Cmd (ENC_2_TIMER, ENABLE);
	encoder_resetEncoderTwo();
}

static void
encoder_resetEncoderOne(void){
	__disable_irq();
	encoder_parameterEncoderOneOld = 0;
	encoder_parameterEncoderOneNew = 0;
	encoder_parameterEncoderOneDifference = 0;
	TIM_SetCounter(ENC_1_TIMER, 0);
	__enable_irq();
}

static void
encoder_resetEncoderTwo(void){
	__disable_irq();
	encoder_parameterEncoderTwoOld = 0;
	encoder_parameterEncoderTwoNew = 0;
	encoder_parameterEncoderTwoDifference = 0;
	TIM_SetCounter(ENC_2_TIMER, 0);
	__enable_irq();
}

void
encoder_readEncoderOne(void){
	encoder_parameterEncoderOneOld = encoder_parameterEncoderOneNew;
	encoder_parameterEncoderOneNew = TIM_GetCounter(ENC_1_TIMER);
	encoder_parameterEncoderOneDifference = encoder_parameterEncoderOneNew - encoder_parameterEncoderOneOld;
}
void
encoder_readEncoderTwo(void){
	encoder_parameterEncoderTwoOld = encoder_parameterEncoderTwoNew;
	encoder_parameterEncoderTwoNew = TIM_GetCounter(ENC_2_TIMER);
	encoder_parameterEncoderTwoDifference = encoder_parameterEncoderTwoNew - encoder_parameterEncoderTwoOld;
}

int16_t
encoder_getEncoderOneDifference(void){
	return encoder_parameterEncoderOneDifference;
}
int16_t
encoder_getEncoderTwoDifference(void){
	return encoder_parameterEncoderTwoDifference;
}

int16_t
encoder_getEncoderOneValue(void){
	return encoder_parameterEncoderOneNew;
}
int16_t
encoder_getEncoderTwoValue(void){
	return encoder_parameterEncoderTwoNew;
}
