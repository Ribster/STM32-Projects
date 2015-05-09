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

extern volatile uint8_t afe_busy;	// flag for AFE that it is busy
extern volatile uint8_t afe_enableRecording;	// recording option is enabled
extern volatile uint32_t afe_recordingTime;		// recording time in seconds
extern volatile timeRegistration_t afe_recordedTime;	// recording time in a time structure
extern volatile uint32_t afe_recordingPacketCount;		// recording packetcount
extern volatile uint8_t afe_currentPacket;		// current packet for DMA
extern volatile uint8_t afe_currentCapture;		// current capture for DMA
extern volatile uint8_t afe_loggingCapture;		// current capture for SD write
extern volatile uint8_t afe_timeStorage[4];		// writing stamp SD write
extern volatile char afe_writingFolderString[13];		// folder for SD write
extern volatile char afe_writingFileString[13];		// file for SD write
extern DMA_InitTypeDef afe_struct;	// structure for setting DMA options

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
