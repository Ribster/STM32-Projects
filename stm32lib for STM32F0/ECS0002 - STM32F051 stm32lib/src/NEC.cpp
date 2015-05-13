/*
 * NEC.cpp
 *
 *  Created on: 26-jan.-2015
 *      Author: Robbe
 */

#include "NEC.h"

namespace stm32lib
{

  NEC::~NEC ()
  {
    // TODO Auto-generated destructor stub
  }

  void NEC::setCommand(uint8_t commandRef, uint8_t thirdByte){
    if(commandRef>= _LIST_CMDS_size){
	return;
    }
    _LIST_CMDS[commandRef].bytes[2] = thirdByte;
    _LIST_CMDS[commandRef].bytes[3] = ~thirdByte;
    _LIST_CMDS[commandRef].ref = commandRef;
  }

  void NEC::setChannel(uint8_t commandRef, uint8_t channelRef){
    // commands spanning from 0-n-1
    if(commandRef>= _LIST_CMDS_size){
	return;
    }
    // channels spanning from 1-n
    if(channelRef> _LIST_CHAN_size){
	return;
    } else if(channelRef == 0){
	return;
    }
    _LIST_CHAN[channelRef-1].ref = _LIST_CMDS[commandRef].ref;

    _LIST_CHAN[channelRef-1].bytes[0] = _LIST_CMDS[commandRef].bytes[0];
    _LIST_CHAN[channelRef-1].bytes[1] = _LIST_CMDS[commandRef].bytes[1];
    _LIST_CHAN[channelRef-1].bytes[2] = _LIST_CMDS[commandRef].bytes[2];
    _LIST_CHAN[channelRef-1].bytes[3] = _LIST_CMDS[commandRef].bytes[3];
  }

  void NEC::setChannelAll(uint8_t commandRef){
    uint8_t i=1;
    for(;i<_LIST_CHAN_size+1; i++){
	setChannel(commandRef, i);
    }
  }

  // print Routines
  void NEC::printCommandList(void){
    uint8_t i = 0;
    for(;i<_LIST_CMDS_size; i++){
	printStructure(_LIST_CMDS, i);
    }
  }

  void NEC::printChannelList(void){
    uint8_t i;
    for(i=0;i<_LIST_CHAN_size; i++){
	printf("channel %02d ", i+1);
	printStructure(_LIST_CHAN, i);
    }
  }

  void NEC::printChannelBuffer(uint8_t channel){
    uint8_t i,j;
    uint8_t* startptr = &_LIST_CHAN_BUFFER[(int)(channel-1)];
    if(channel>1){
	startptr += HC595_BUFFERDIM * (channel-1);
    }
    fillChannelBufferAll();

    printf("Channel %d: ", channel);

    for(i=0;i<HC595_BUFFERDIM;i++){
	for(j=0;j<8;j++){
	    printf("%d", (*(startptr+i) & (1<<j))?(1):(0) );
	}
    }

    printf("\r\n");
  }

  void NEC::printChannelBufferAll(void){
    uint8_t i;
    for(i=0;i<_LIST_CHAN_size; i++){
	printChannelBuffer(i+1);
    }
  }

  void NEC::printStructure(NEC_t* structure, uint8_t ref){
    printf("command %02d payload 0x%02x 0x%02x 0x%02x 0x%02x \r\n", structure[ref].ref, structure[ref].bytes[0], structure[ref].bytes[1], structure[ref].bytes[2], structure[ref].bytes[3]);
  }

  // transmit Routines
  void NEC::transmit(void){
    // start by filling the channel buffer with the correct data
    _OBJ_IO->setValue(_PIN_LED, true);

    //printChannelBufferAll();

    //printChannelList();

    fillChannelBufferAll();

    uint8_t i, j, k;

    // start IR
    _OBJ_OSCILLATOR->start();

    // high for 9 ms
    for(k=1; k<_LIST_CHAN_size+1; k++){
	_OBJ_HC595->setState(k + _LIST_CHAN_shift, true);
    }
    _OBJ_HC595->transmit(true, false);
    setDelayMicro(HC595_BUFFERDELAY_1);

    // low for 4,5ms
    for(k=1; k<_LIST_CHAN_size+1; k++){
	_OBJ_HC595->setState(k + _LIST_CHAN_shift, false);
    }
    _OBJ_HC595->transmit(true, false);
    setDelayMicro(HC595_BUFFERDELAY_2);



    for(i=0; i<HC595_BUFFERDIM; i++){
	for(j=0; j<8; j++){
	    // set the bits for each channel
	    for(k=1; k<_LIST_CHAN_size+1; k++){
		_OBJ_HC595->setState(k + _LIST_CHAN_shift, getChannelBit(k, i, j));
	    }
	    // transmit
	    _OBJ_HC595->transmit(true, false);
	    // delay
	    setDelayMicro(HC595_BUFFERDELAY_X);
	}
    }


    for(k=1; k<_LIST_CHAN_size+1; k++){
	_OBJ_HC595->setState(k + _LIST_CHAN_shift, true);
    }
    // transmit
    _OBJ_HC595->transmit(true, false);

    // delay
    setDelayMicro(HC595_BUFFERDELAY_X);

    for(k=1; k<_LIST_CHAN_size+1; k++){
	_OBJ_HC595->setState(k + _LIST_CHAN_shift, false);
    }
    // transmit
    _OBJ_HC595->transmit(true, false);

    // stop IR
    _OBJ_OSCILLATOR->stop();

    _OBJ_IO->setValue(_PIN_LED, false);

  }

  // channel buffer routines
  void NEC::fillChannelBuffer(uint8_t channel){
    // setting the start of the buffer for the selected channel
    uint8_t* startptr = &_LIST_CHAN_BUFFER[(int)(channel-1)];
    if(channel>1){
	startptr += HC595_BUFFERDIM * (channel-1);
    }
    NEC_t* structptr = &_LIST_CHAN[channel-1];
    uint8_t i, j, cnt, buff;
    // cnt -> bitcounter
    // i -> arraycounter
    // j -> bitcounter in array

    for(i=0, cnt=0; i<4; i++){
      // run over each byte
      buff = structptr->bytes[i];
	// run over each bit
	for(j=8; j>0; j--){
	    // set 1 high 1 low

	    setBit(startptr, cnt/8, cnt%8, true);
	    cnt++;
	    setBit(startptr, cnt/8, cnt%8, false);
	    cnt++;
	    if(buff&(1<<(j-1))){
		 // set 2 low
		  setBit(startptr, cnt/8, cnt%8, false);
		  cnt++;
		  setBit(startptr, cnt/8, cnt%8, false);
		  cnt++;
	    }
	}
    }


  }

  void NEC::setBit(uint8_t* startAddress, uint8_t byteShift, uint8_t bit, bool newState){
    if(newState){
	*(startAddress + byteShift) |= (1<<bit);
    } else {
	*(startAddress + byteShift) &= ~(1<<bit);
    }
  }

  void NEC::fillChannelBufferAll(void){
    uint8_t i;
    for(i=1;i<_LIST_CHAN_size+1;i++){
	fillChannelBuffer(i);
    }
  }

  bool NEC::getChannelBit(uint8_t channel, uint8_t bufflocation, uint8_t bitlocation){
    uint8_t* startptr = &_LIST_CHAN_BUFFER[(channel-1)];
    if(channel>1){
	startptr += (HC595_BUFFERDIM * (channel-1));
    }
    return ( *(startptr + bufflocation) & (1<<bitlocation) ? (true) : (false) );
  }

  // interrupt
  void NEC::interruptHandler(void){
    if(timeCounter>0u){
      timeCounter--;
    }
  }

  // initialize routines
  void NEC::initializeTimer(void){
    initializeTimer(_OBJ_TIMx);
  }

  // delay routines
  void NEC::setDelayMicro(uint32_t newVal){
	timeCounter = newVal/10;
	TIM_Cmd(_OBJ_TIMx, ENABLE);
	while(timeCounter>0);
	TIM_Cmd(_OBJ_TIMx, DISABLE);
  }

// PRIVATE ----

  // initialize routines
  void NEC::initializeTimer(TIM_TypeDef* TIMx){
	TIM_TimeBaseInitTypeDef tstruct;
	NVIC_InitTypeDef istruct;

	if (TIMx == TIM2){
	  istruct.NVIC_IRQChannel = TIM2_IRQn;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	} else if (TIMx == TIM3){
	  istruct.NVIC_IRQChannel = TIM3_IRQn;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	tstruct.TIM_ClockDivision = 0;
	tstruct.TIM_CounterMode = TIM_CounterMode_Up;
	tstruct.TIM_Period = SystemCoreClock/100000;
	tstruct.TIM_Prescaler = 0;
	tstruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMx, &tstruct);
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);

	istruct.NVIC_IRQChannelCmd = ENABLE;
	istruct.NVIC_IRQChannelPriority = 1;
	NVIC_Init(&istruct);
  }

} /* namespace stm32lib */
