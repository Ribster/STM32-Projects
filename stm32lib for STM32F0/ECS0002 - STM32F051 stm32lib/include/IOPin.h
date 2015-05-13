/*
 * IOPin.h
 *
 *  Created on: 22-jan.-2015
 *      Author: Robbe
 */

#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

#include "stdint.h"

#ifndef IOPIN_H_
#define IOPIN_H_

namespace stm32lib
{

  typedef struct IO_t{
  	GPIO_InitTypeDef GPIOInfo;
  	GPIO_TypeDef* GPIOx;
  	uint8_t GPIO_AF;
  	bool init;
  }IO_t;

  class IOPin
  {
  public:
    IOPin(GPIO_TypeDef*, uint16_t, GPIOMode_TypeDef, GPIOOType_TypeDef, GPIOPuPd_TypeDef, GPIOSpeed_TypeDef);
    IOPin(GPIO_TypeDef*, uint16_t, GPIOMode_TypeDef, GPIOOType_TypeDef, GPIOPuPd_TypeDef, GPIOSpeed_TypeDef, uint8_t GPIO_AF);
    IO_t *PIN = new IO_t();
    virtual
    ~IOPin ();
  };

} /* namespace stm32lib */

#endif /* IOPIN_H_ */
