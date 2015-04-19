/*
 * dma.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "dma.h"

// prototypes
static inline void
dma_SPIRequest(
		DMA_Stream_TypeDef* DMAx,
		SPI_TypeDef* SPIx,
		uint16_t SPI_I2S_DMAReq,
		FunctionalState newVal);

// implementation
void
initialize_DMA(DMA_Stream_TypeDef* DMAx,
		uint32_t DMA_Channel,
		uint32_t DMA_PeripheralBaseAddr,
		uint32_t DMA_Memory0BaseAddr,
		uint32_t DMA_DIR,
		uint32_t DMA_BufferSize,
		uint32_t DMA_PeripheralInc,
		uint32_t DMA_MemoryInc,
		uint32_t DMA_PeripheralDataSize,
		uint32_t DMA_MemoryDataSize,
		uint32_t DMA_Mode,
		uint32_t DMA_Priority,
		uint32_t DMA_FIFOMode,
		uint32_t DMA_FIFOThreshold,
		uint32_t DMA_MemoryBurst,
		uint32_t DMA_PeripheralBurst){
	rcc_setDMAClock(DMAx, ENABLE);
	DMA_InitTypeDef tmp;
	tmp.DMA_BufferSize = DMA_BufferSize;
	tmp.DMA_FIFOMode = DMA_FIFOMode ;
	tmp.DMA_FIFOThreshold = DMA_FIFOThreshold ;
	tmp.DMA_MemoryBurst = DMA_MemoryBurst ;
	tmp.DMA_MemoryDataSize = DMA_MemoryDataSize;
	tmp.DMA_MemoryInc = DMA_MemoryInc;
	tmp.DMA_Mode = DMA_Mode;

	tmp.DMA_PeripheralBaseAddr = DMA_PeripheralBaseAddr;
	tmp.DMA_PeripheralBurst = DMA_PeripheralBurst;
	tmp.DMA_PeripheralDataSize = DMA_PeripheralDataSize;
	tmp.DMA_MemoryDataSize = DMA_MemoryDataSize;
	tmp.DMA_PeripheralInc = DMA_PeripheralInc;
	tmp.DMA_Priority = DMA_Priority;

	/* Configure Tx DMA */
	tmp.DMA_Channel = DMA_Channel;
	tmp.DMA_DIR = DMA_DIR;
	tmp.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;
	DMA_Init(DMAx, &tmp);
	DMA_Cmd(DMAx, ENABLE);
}


void
dma_enableSPIRequest(
		DMA_Stream_TypeDef* DMAx,
		SPI_TypeDef* SPIx,
		uint16_t SPI_I2S_DMAReq){
	dma_SPIRequest(DMAx, SPIx, SPI_I2S_DMAReq, ENABLE);
}
void
dma_disableSPIRequest(
		DMA_Stream_TypeDef* DMAx,
		SPI_TypeDef* SPIx,
		uint16_t SPI_I2S_DMAReq){
	dma_SPIRequest(DMAx, SPIx, SPI_I2S_DMAReq, DISABLE);
}

static inline void
dma_SPIRequest(
		DMA_Stream_TypeDef* DMAx,
		SPI_TypeDef* SPIx,
		uint16_t SPI_I2S_DMAReq,
		FunctionalState newVal){
	DMA_Cmd(DMAx, newVal);
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq, newVal);
}
