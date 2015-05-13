/*
 * HC595.h
 *
 *  Created on: 4-dec.-2014
 *      Author: Robbe
 */

#ifndef HC595_H_
#define HC595_H_

#include <stdio.h>
#include "IO.h"
#include "IOPin.h"
#include "UART.h"

namespace stm32lib {

typedef struct HC595_t{
	IO_t* SCK;
	IO_t* DATA;
	IO_t* LATCH;
	uint8_t* datablock;
	uint8_t datasize;
} HC595_t;

class HC595 {
public:
	HC595(IO*, HC595_t*, IO_t* SCK, IO_t* DATA, IO_t* LATCH, uint8_t* dataBlock, uint8_t datasize);
	virtual ~HC595();

	void transmit(bool MSBfirst, bool inverted);
	void transmitnoLatch(bool MSBfirst, bool inverted);

	void latch();
	void setStruct(HC595_t*);

	void setState(uint8_t channel, bool);


private:

	HC595_t* _STRUCT_HC595 = 0;
	IO* _OBJ_IO = 0;

	bool init = false;
	void setStatesInit(HC595_t*);
	void setStatesClose(HC595_t*);
	void transmit(HC595_t*, uint8_t*, uint8_t, bool MSBfirst, bool inverted);
	void latch(HC595_t*);
};

} /* namespace stm32lib */

#endif /* HC595_H_ */
