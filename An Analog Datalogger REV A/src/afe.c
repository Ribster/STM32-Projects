/*
 * afe.c
 *
 *  Created on: 21-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "afe.h"

volatile uint8_t afe_DMA_TX_Buffer[AFE_DMA_BufferSize];
volatile uint8_t afe_DMA_RX_Buffer[AFE_DMA_BufferSize];

void
initialize_AFE(void){

	// clear RX & TX buffer
	memset((void*)afe_DMA_RX_Buffer, 0, sizeof(afe_DMA_RX_Buffer) );
	//memset((void*)afe_DMA_TX_Buffer, 0, AFE_DMA_BufferSize);
	for(uint32_t j=0;j<sizeof(afe_DMA_TX_Buffer);j++){
		afe_DMA_TX_Buffer[j] = j%256;
	}


	rcc_setDMAClock(AFE_DMA_RX_DMAStream, ENABLE);
	rcc_setDMAClock(AFE_DMA_TX_DMAStream, ENABLE);

	NVIC_EnableIRQ(AFE_DMA_TX_DMAIRQ);
	NVIC_EnableIRQ(AFE_DMA_RX_DMAIRQ);
	nvic_initInterrupt(AFE_DMA_RX_DMAIRQ,
			0,
			1);

	nvic_initInterrupt(AFE_DMA_TX_DMAIRQ,
			0,
			0);

	// intialize pins

		// MISO PIN
		gpio_initAF(
				AFE_MISO_PORT,
				AFE_MISO_MODE,
				AFE_MISO_OTYPE,
				AFE_MISO_PIN,
				AFE_MISO_PULL,
				AFE_MISO_SPEED,
				AFE_MISO_AF);
		// MOSI PIN
		gpio_initAF(
				AFE_MOSI_PORT,
				AFE_MOSI_MODE,
				AFE_MOSI_OTYPE,
				AFE_MOSI_PIN,
				AFE_MOSI_PULL,
				AFE_MOSI_SPEED,
				AFE_MOSI_AF);
		// SCK PIN
		//rcc_setGPIOClock(AFE_SCK_PORT, ENABLE);
		gpio_initAF(
				AFE_SCK_PORT,
				AFE_SCK_MODE,
				AFE_SCK_OTYPE,
				AFE_SCK_PIN,
				AFE_SCK_PULL,
				AFE_SCK_SPEED,
				AFE_SCK_AF);

		// CS PIN
		gpio_initStandard(
				AFE_CS_PORT,
				AFE_CS_MODE,
				AFE_CS_OTYPE,
				AFE_CS_PIN,
				AFE_CS_PULL,
				AFE_CS_SPEED);
		//GPIO_WriteBit(AFE_CS_PORT, (1<<AFE_CS_PIN), AFE_CS_INITSTATE);
		// INT PIN
		gpio_initStandard(
				AFE_INT_PORT,
				AFE_INT_MODE,
				AFE_INT_OTYPE,
				AFE_INT_PIN,
				AFE_INT_PULL,
				AFE_INT_SPEED);
		//GPIO_WriteBit(AFE_INT_PORT, (1<<AFE_INT_PIN), AFE_INT_INITSTATE);
		// SYNC 1
		gpio_initStandard(
				AFE_SYNC1_PORT,
				AFE_SYNC1_MODE,
				AFE_SYNC1_OTYPE,
				AFE_SYNC1_PIN,
				AFE_SYNC1_PULL,
				AFE_SYNC1_SPEED);
		GPIO_WriteBit(AFE_SYNC1_PORT, (1<<AFE_SYNC1_PIN), AFE_SYNC1_INITSTATE);
		// SYNC 2
		gpio_initStandard(
				AFE_SYNC2_PORT,
				AFE_SYNC2_MODE,
				AFE_SYNC2_OTYPE,
				AFE_SYNC2_PIN,
				AFE_SYNC2_PULL,
				AFE_SYNC2_SPEED);

		// SYNC 3
		gpio_initStandard(
				AFE_SYNC3_PORT,
				AFE_SYNC3_MODE,
				AFE_SYNC3_OTYPE,
				AFE_SYNC3_PIN,
				AFE_SYNC3_PULL,
				AFE_SYNC3_SPEED);

		// SYNC EXT
		gpio_initStandard(
				AFE_EXT_SYNC_PORT,
				AFE_EXT_SYNC_MODE,
				AFE_EXT_SYNC_OTYPE,
				AFE_EXT_SYNC_PIN,
				AFE_EXT_SYNC_PULL,
				AFE_EXT_SYNC_SPEED);





	// initialize SPI
		//rcc_setSPIClock(AFE_SPI, ENABLE);
		initialize_SPI(
				AFE_SPI,
				AFE_SPI_Direction,
				AFE_SPI_Mode,
				AFE_SPI_DataSize,
				AFE_SPI_CPOL,
				AFE_SPI_CPHA,
				AFE_SPI_NSS,
				AFE_SPI_BaudRatePrescaler,
				AFE_SPI_FirstBit,
				AFE_SPI_CRCPolynomial);
		//SPI_Cmd(AFE_SPI, ENABLE);
}

void
afe_read(void){
	DMA_DeInit(AFE_DMA_TX_DMAStream);
	DMA_DeInit(AFE_DMA_RX_DMAStream);

	DMA_InitTypeDef tmp;
	tmp.DMA_BufferSize = (uint16_t)sizeof(afe_DMA_TX_Buffer);
	tmp.DMA_FIFOMode = AFE_DMA_FIFOMode ;
	tmp.DMA_FIFOThreshold = AFE_DMA_FIFOThreshold ;
	tmp.DMA_MemoryBurst = AFE_DMA_MemoryBurst ;
	tmp.DMA_MemoryDataSize = AFE_DMA_MemoryDataSize;
	tmp.DMA_MemoryInc = AFE_DMA_MemoryInc;
	tmp.DMA_Mode = AFE_DMA_Mode;

	tmp.DMA_PeripheralBaseAddr = AFE_DMA_PeripheralBaseAddr;
	tmp.DMA_PeripheralBurst = AFE_DMA_PeripheralBurst;
	tmp.DMA_PeripheralDataSize = AFE_DMA_PeripheralDataSize;
	tmp.DMA_MemoryDataSize = AFE_DMA_MemoryDataSize;
	tmp.DMA_PeripheralInc = AFE_DMA_PeripheralInc;
	tmp.DMA_Priority = AFE_DMA_Priority;

	tmp.DMA_Channel = AFE_DMA_TX_DMAChannel;
	tmp.DMA_DIR = AFE_DMA_TX_DIR;
	tmp.DMA_Memory0BaseAddr = (uint32_t)(afe_DMA_TX_Buffer);
	//DMA_Cmd(AFE_DMA_TX_DMAStream, DISABLE);
	while (AFE_DMA_TX_DMAStream->CR & DMA_SxCR_EN);
	DMA_Init(AFE_DMA_TX_DMAStream, &tmp);

	// RX STREAM


	tmp.DMA_BufferSize = (uint16_t)sizeof(afe_DMA_RX_Buffer);
	tmp.DMA_FIFOMode = AFE_DMA_FIFOMode ;
	tmp.DMA_FIFOThreshold = AFE_DMA_FIFOThreshold ;
	tmp.DMA_MemoryBurst = AFE_DMA_MemoryBurst ;
	tmp.DMA_MemoryDataSize = AFE_DMA_MemoryDataSize;
	tmp.DMA_MemoryInc = AFE_DMA_MemoryInc;
	tmp.DMA_Mode = AFE_DMA_Mode;

	tmp.DMA_PeripheralBaseAddr = AFE_DMA_PeripheralBaseAddr;
	tmp.DMA_PeripheralBurst = AFE_DMA_PeripheralBurst;
	tmp.DMA_PeripheralDataSize = AFE_DMA_PeripheralDataSize;
	tmp.DMA_MemoryDataSize = AFE_DMA_MemoryDataSize;
	tmp.DMA_PeripheralInc = AFE_DMA_PeripheralInc;
	tmp.DMA_Priority = AFE_DMA_Priority;

	tmp.DMA_Channel = AFE_DMA_RX_DMAChannel;
	tmp.DMA_DIR = AFE_DMA_RX_DIR;
	tmp.DMA_Memory0BaseAddr = (uint32_t)(afe_DMA_RX_Buffer);
	//DMA_Cmd(AFE_DMA_RX_DMAStream, DISABLE);
	while (AFE_DMA_RX_DMAStream->CR & DMA_SxCR_EN);
	DMA_Init(AFE_DMA_RX_DMAStream, &tmp);


	/* Enable SPI DMA RX TX Requests */
	SPI_I2S_DMACmd(AFE_SPI, SPI_I2S_DMAReq_Tx, ENABLE);

	SPI_I2S_DMACmd(AFE_SPI, SPI_I2S_DMAReq_Rx, ENABLE);


	DMA_ITConfig(AFE_DMA_TX_DMAStream, DMA_IT_TC | DMA_IT_HT, ENABLE);
	DMA_ITConfig(AFE_DMA_RX_DMAStream, DMA_IT_TC | DMA_IT_HT, ENABLE);

//	SPI_I2S_ClearFlag(AFE_SPI, SPI_I2S_FLAG_TXE);
//	SPI_I2S_ClearFlag(AFE_SPI, SPI_I2S_FLAG_RXNE);


	/* Enable DMA SPI TX Stream */
	DMA_Cmd(AFE_DMA_TX_DMAStream,ENABLE);

	/* Enable DMA SPI RX Stream */
	DMA_Cmd(AFE_DMA_RX_DMAStream,ENABLE);

	/* Enable the SPI peripheral */

	SPI_Cmd(AFE_SPI, ENABLE);
	delay_milli(1);

	GPIO_WriteBit(AFE_CS_PORT, AFE_CS_PIN, Bit_RESET);
	AFE_INT_PORT->ODR ^= (1<<AFE_INT_PIN);
}
