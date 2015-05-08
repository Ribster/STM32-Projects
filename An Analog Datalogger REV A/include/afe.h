/*
 * afe.h
 *
 *  Created on: 21-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef AFE_H_
#define AFE_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#include "projectconfig.h"

#include "gpio.h"
#include "rcc.h"
#include "dma.h"
#include "nvic.h"
#include "delay.h"
#include "general.h"
#include "sdio.h"

#include "string.h"

#include <stdio.h>

extern volatile uint8_t afe_DMA_RX_Buffer[AFE_DMA_CAPTURES][AFE_DMA_BLOCKS][AFE_DMA_BLOCKSIZE];
extern volatile uint8_t afe_DMA_TX_Buffer[AFE_DMA_BLOCKS][AFE_DMA_BLOCKSIZE];

extern volatile uint8_t afe_busy;
extern volatile uint8_t afe_enableRecording;
extern volatile uint32_t afe_recordingTime;
extern volatile timeRegistration_t afe_recordedTime;
extern volatile uint32_t afe_recordingPacketCount;
extern volatile uint8_t afe_currentPacket;
extern volatile uint8_t afe_currentCapture;
extern volatile uint8_t afe_timeStorage[4];
extern volatile char afe_writingFolderString[13];

extern DMA_InitTypeDef afe_struct;

// prototypes
void
initialize_AFE(void);

void
afe_startRecording(void);

void
afe_stopRecording(void);

void
afe_startReadout(void);

void
afe_endReadout(void);

void
afe_read(void);

void
afe_fileWrite(void);

void
afe_getNewFolderName(void);

void
afe_incrementFolderName(void);

void
afe_createNewFolder(void);

void
afe_writeInfoHeader(void);

void
afe_writeInfoFooter(void);

#endif /* AFE_H_ */
