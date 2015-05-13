/*
 * IOPIN->cpp
 *
 *  Created on: 22-jan.-2015
 *      Author: Robbe
 */

#include "IOPIN.h"

namespace stm32lib
{

  IOPin::IOPin(GPIO_TypeDef* GPIOx, uint16_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef type, GPIOPuPd_TypeDef pull, GPIOSpeed_TypeDef speed){
    PIN->GPIOx = GPIOx;
    PIN->init = false;
    PIN->GPIOInfo.GPIO_Mode = mode;
    PIN->GPIOInfo.GPIO_OType = type;
    PIN->GPIOInfo.GPIO_Pin = (1<<pin);
    PIN->GPIOInfo.GPIO_PuPd = pull;
    PIN->GPIOInfo.GPIO_Speed = speed;
  }
  IOPin::IOPin(GPIO_TypeDef* GPIOx, uint16_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef type, GPIOPuPd_TypeDef pull, GPIOSpeed_TypeDef speed, uint8_t GPIO_AF){
    PIN->GPIO_AF = GPIO_AF;
    PIN->GPIOx = GPIOx;
    PIN->init = false;
    PIN->GPIOInfo.GPIO_Mode = mode;
    PIN->GPIOInfo.GPIO_OType = type;
    PIN->GPIOInfo.GPIO_Pin = (1<<pin);
    PIN->GPIOInfo.GPIO_PuPd = pull;
    PIN->GPIOInfo.GPIO_Speed = speed;
  }


  IOPin::~IOPin ()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace stm32lib */
