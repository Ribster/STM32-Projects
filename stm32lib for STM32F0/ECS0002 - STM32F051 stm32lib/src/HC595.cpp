/*
 * HC595.cpp
 *
 *  Created on: 4-dec.-2014
 *      Author: Robbe
 */

#include "HC595.h"

namespace stm32lib {

HC595::HC595(IO* objIO, HC595_t* strHC, IO_t* SCK, IO_t* DATA, IO_t* LATCH, uint8_t* dataBlock, uint8_t datasize) {
  _OBJ_IO = objIO;
  _STRUCT_HC595 = strHC;
  _STRUCT_HC595->DATA = DATA;
  _STRUCT_HC595->LATCH = LATCH;
  _STRUCT_HC595->SCK = SCK;
  _STRUCT_HC595->datasize = datasize;
  _STRUCT_HC595->datablock = dataBlock;
}

HC595::~HC595() {

}



void HC595::transmit(bool MSBfirst, bool inverted){
	if(_STRUCT_HC595 == NULL){
		return;
	}
	setStatesInit(_STRUCT_HC595);
	transmit(_STRUCT_HC595, _STRUCT_HC595->datablock, _STRUCT_HC595->datasize, MSBfirst, inverted);
	latch(_STRUCT_HC595);
	setStatesClose(_STRUCT_HC595);
}

void HC595::transmitnoLatch(bool MSBfirst, bool inverted){
	if(_STRUCT_HC595 == NULL){
		return;
	}
	setStatesInit(_STRUCT_HC595);
	transmit(_STRUCT_HC595, _STRUCT_HC595->datablock, _STRUCT_HC595->datasize, MSBfirst, inverted);
	setStatesClose(_STRUCT_HC595);
}


void HC595::transmit(HC595_t* HCObj, uint8_t* datablock, uint8_t datasize, bool MSBfirst, bool inverted){
	// used variables
	uint8_t i, j;

	for(i=0; i<datasize; i++, datablock++){

		if(MSBfirst){
			for(j=8;j>0;j--){
				if(inverted){
					_OBJ_IO->setValue(HCObj->DATA, ! (*datablock & (1<< (j-1) )) );
				} else {
					_OBJ_IO->setValue(HCObj->DATA, *datablock & (1<< (j-1) ));
				}

				_OBJ_IO->setValue(HCObj->SCK, false);
				_OBJ_IO->setValue(HCObj->SCK, true);
			}
		} else {
			for(j=0;j<8;j++){
				if(inverted){
					_OBJ_IO->setValue(HCObj->DATA, ! (*datablock & (1<<j)) );
				} else {
					_OBJ_IO->setValue(HCObj->DATA, *datablock & (1<<j));
				}

				_OBJ_IO->setValue(HCObj->SCK, false);
				_OBJ_IO->setValue(HCObj->SCK, true);
			}
		}

	}
}

void HC595::latch(HC595_t* HCObj){
	_OBJ_IO->setValue(HCObj->LATCH, false);
	_OBJ_IO->setValue(HCObj->LATCH, true);
}

void HC595::latch(){
	latch(_STRUCT_HC595);
}

void HC595::setStatesInit(HC595_t* HCObj){
	_OBJ_IO->setValue(HCObj->LATCH, true);
	_OBJ_IO->setValue(HCObj->SCK, false);
	_OBJ_IO->setValue(HCObj->DATA, false);
}
void HC595::setStatesClose(HC595_t* HCObj){
	_OBJ_IO->setValue(HCObj->SCK, false);
	_OBJ_IO->setValue(HCObj->DATA, false);
}


void HC595::setStruct(HC595_t* HCObj){
	_STRUCT_HC595 = HCObj;
}

void HC595::setState(uint8_t channel, bool newVal){
  // channel span is 1 ... n
  if(channel == 0){
      return;
  }
  if(channel > _STRUCT_HC595->datasize*8){
      return;
  }
  uint8_t tmp = channel;
  uint8_t* ptr = _STRUCT_HC595->datablock;
  while(tmp>8){
      tmp-=8;
      ptr++;
  }
  if(newVal){
      *ptr |= (1<<(tmp-1));
  } else {
      *ptr &= ~(1<<(tmp-1));
  }

}

} /* namespace stm32lib */
