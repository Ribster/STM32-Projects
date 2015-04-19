/*
 * dma.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef DMA_H_
#define DMA_H_

#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_dma2d.h"
#include "stm32f4xx_spi.h"
#include "gpio.h"
#include "rcc.h"

// prototypes

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
		uint32_t DMA_PeripheralBurst);

void
dma_enableSPIRequest(
		DMA_Stream_TypeDef* DMAx,
		SPI_TypeDef* SPIx,
		uint16_t SPI_I2S_DMAReq);
void
dma_disableSPIRequest(
		DMA_Stream_TypeDef* DMAx,
		SPI_TypeDef* SPIx,
		uint16_t SPI_I2S_DMAReq);

#endif /* DMA_H_ */
