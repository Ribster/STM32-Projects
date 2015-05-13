/*
 * IO.h
 *
 *  Created on: 4-dec.-2014
 *      Author: Robbe
 */

#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

#include "stdint.h"

#include "IOPin.h"

#ifndef IO_H_
#define IO_H_



namespace stm32lib {

class IO {
public:
	IO();
	virtual ~IO();
public:
	// functions used for IO initialization
	void begin(GPIO_TypeDef*, uint16_t, GPIOMode_TypeDef, GPIOOType_TypeDef, GPIOPuPd_TypeDef, GPIOSpeed_TypeDef);
	void begin(IO_t*, GPIO_TypeDef*, uint16_t, GPIOMode_TypeDef, GPIOOType_TypeDef, GPIOPuPd_TypeDef, GPIOSpeed_TypeDef);
	void begin(GPIO_TypeDef*, uint16_t, GPIOMode_TypeDef, GPIOOType_TypeDef, GPIOPuPd_TypeDef, GPIOSpeed_TypeDef, uint8_t GPIO_AF);
	void begin(IO_t*, GPIO_TypeDef*, uint16_t, GPIOMode_TypeDef, GPIOOType_TypeDef, GPIOPuPd_TypeDef, GPIOSpeed_TypeDef, uint8_t GPIO_AF);
	void begin(IO_t*);
	void begin(IO_t*, IOPin*);
	void begin(IO_t*, uint8_t GPIO_AF);
	void begin(IOPin*);

	void initEXTI();

private:
	uint32_t getRCCPeriphGPIO(GPIO_TypeDef* GPIOx);
public:
	// functions that control IO
	void setValue(IO_t*, bool);
	void toggleValue(IO_t*);
	bool getValue(IO_t*);

	void setValue(IOPin*, bool);
	void toggleValue(IOPin*);
	bool getValue(IOPin*);

};

} /* namespace stm32lib */

#endif /* IO_H_ */
