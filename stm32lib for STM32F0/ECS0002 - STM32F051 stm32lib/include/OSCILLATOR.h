/*
 * OSCILLATOR.h
 *
 *  Created on: 4-dec.-2014
 *      Author: Robbe
 */

#include "IO.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"

#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_



namespace stm32lib {

  typedef struct OSC_t{
    TIM_TimeBaseInitTypeDef TIMInfo;
    TIM_OCInitTypeDef TIMOCInfo;
    TIM_TypeDef* TIMx;
    uint8_t TIMOCChannel;
    uint32_t OscFreq;
  } OSC_t;

class OSCILLATOR {
public:
	OSCILLATOR(OSC_t* oscillatorObject): _OBJ_OSCILLATOR_(oscillatorObject){};
	virtual ~OSCILLATOR();

	void begin(OSC_t*, uint32_t, TIM_TypeDef*, uint8_t);
	void begin(uint32_t, TIM_TypeDef*, uint8_t);
	void start(OSC_t*);
	void stop(OSC_t*);
	void start(void);
	void stop(void);
private:
	void setRCCClock(TIM_TypeDef*, bool);
	OSC_t* _OBJ_OSCILLATOR_ = 0;
};

} /* namespace stm32lib */

#endif /* OSCILLATOR_H_ */
