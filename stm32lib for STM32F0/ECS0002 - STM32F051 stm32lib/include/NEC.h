/*
 * NEC.h
 *
 *  Created on: 26-jan.-2015
 *      Author: Robbe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "IO.h"
#include "HC595.h"
#include "OSCILLATOR.h"

#include "stm32f0xx_tim.h"

#ifndef NEC_H_
#define NEC_H_

namespace stm32lib
{
  typedef struct NEC_t{
    uint8_t ref;
    uint8_t bytes[4] = {0x00, 0xFF, 0x00, 0x00};
  } NEC_t;


  class NEC
  {
  public:
    // CTOR && DTOR
    NEC (TIM_TypeDef* timerObject, HC595* shiftObject, OSCILLATOR* oscillatorObject, NEC_t* commandList, uint8_t commandSize, NEC_t* channelList, uint32_t channelSize, uint8_t channelShift, uint8_t* channelBuffer, IO* objectIO, IO_t* pinLED):
      _OBJ_TIMx(timerObject), _OBJ_HC595(shiftObject), _OBJ_IO(objectIO), _PIN_LED(pinLED), _OBJ_OSCILLATOR(oscillatorObject), _LIST_CMDS(commandList), _LIST_CMDS_size(commandSize), _LIST_CHAN(channelList), _LIST_CHAN_size(channelSize), _LIST_CHAN_shift(channelShift), _LIST_CHAN_BUFFER(channelBuffer){};
    ~NEC ();



    // set Routines
    void setCommand(uint8_t commandRef, uint8_t thirdByte);
    void setChannel(uint8_t commandRef, uint8_t channelRef);
    void setChannelAll(uint8_t commandRef);

    // print Routines
    void printCommandList(void);
    void printChannelList(void);
    void printChannelBuffer(uint8_t channel);
    void printChannelBufferAll(void);
    void printStructure(NEC_t* structure, uint8_t ref);

    // transmit Routines
    void transmit(void);

    // channel buffer routines
    void fillChannelBuffer(uint8_t channel);
    void fillChannelBufferAll(void);
    bool getChannelBit(uint8_t channel, uint8_t bufflocation, uint8_t bitlocation);

    // interrupt
    void interruptHandler(void);

    // initialize routines
    void initializeTimer(void);

    // delay routines
    void setDelayMicro(uint32_t);

  private:
    // initialize routines
    void initializeTimer(TIM_TypeDef*);

    void setBit(uint8_t* startAddress, uint8_t byteShift, uint8_t bit, bool newState);


    // objects used in class
    TIM_TypeDef* _OBJ_TIMx = 0;
      // channel span for HC595 is from 1 ... n
    HC595* _OBJ_HC595 = 0;
    IO* _OBJ_IO = 0;
    IO_t* _PIN_LED = 0;
    OSCILLATOR* _OBJ_OSCILLATOR = 0;
    NEC_t* _LIST_CMDS = 0;
    uint8_t _LIST_CMDS_size = 0;
    NEC_t* _LIST_CHAN = 0;
    uint8_t _LIST_CHAN_size = 0;
    uint8_t _LIST_CHAN_shift = 0;
      // buffer that holds the on/off states of each channel
    uint8_t* _LIST_CHAN_BUFFER = 0;
    // constants
    #define HC595_BUFFERDIM 12	// (2 + 3*8*4) / 8 = 12,25 -> 13
    #define HC595_BUFFERBITS 96
    #define HC595_BUFFERDELAY_1 9000-120
    #define HC595_BUFFERDELAY_2 4500-120
    #define HC595_BUFFERDELAY_X 560-165
    // variables
    volatile uint32_t timeCounter = 0;
  };

  //_OBJ_DELAY(delayObject), _OBJ_HC595(shiftObject), _LIST_CMDS(commandList), _LIST_CMDS_size(commandSize), _channelSize(channelSize)

} /* namespace stm32lib */

#endif /* NEC_H_ */
