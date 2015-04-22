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

#ifdef DBG

#endif

	// intialize pins
		// CS PIN
		gpio_initStandard(
				AFE_CS_PORT,
				AFE_CS_MODE,
				AFE_CS_OTYPE,
				AFE_CS_PIN,
				AFE_CS_PULL,
				AFE_CS_SPEED);
		GPIO_WriteBit(AFE_CS_PORT, (1<<AFE_CS_PIN), AFE_CS_INITSTATE);
		// INT PIN
		gpio_initStandard(
				AFE_INT_PORT,
				AFE_INT_MODE,
				AFE_INT_OTYPE,
				AFE_INT_PIN,
				AFE_INT_PULL,
				AFE_INT_SPEED);
		GPIO_WriteBit(AFE_INT_PORT, (1<<AFE_INT_PIN), AFE_INT_INITSTATE);
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
		gpio_initAF(
				AFE_SCK_PORT,
				AFE_SCK_MODE,
				AFE_SCK_OTYPE,
				AFE_SCK_PIN,
				AFE_SCK_PULL,
				AFE_SCK_SPEED,
				AFE_SCK_AF);

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
	// initialize DMA
		// TX STREAM
		DMA_initNormal(
				AFE_DMA_TX_DMAStream,
				AFE_DMA_TX_DMAChannel,
				AFE_DMA_PeripheralBaseAddr,
				(uint32_t)afe_DMA_TX_Buffer,
				AFE_DMA_TX_DIR,
				AFE_DMA_BufferSize,
				AFE_DMA_PeripheralInc,
				AFE_DMA_MemoryInc,
				AFE_DMA_PeripheralDataSize,
				AFE_DMA_MemoryDataSize,
				AFE_DMA_Mode,
				AFE_DMA_Priority,
				AFE_DMA_FIFOMode,
				AFE_DMA_FIFOThreshold,
				AFE_DMA_MemoryBurst,
				AFE_DMA_PeripheralBurst
				);
		// RX STREAM
		DMA_initNormal(
				AFE_DMA_RX_DMAStream,
				AFE_DMA_RX_DMAChannel,
				AFE_DMA_PeripheralBaseAddr,
				(uint32_t)afe_DMA_RX_Buffer,
				AFE_DMA_RX_DIR,
				AFE_DMA_BufferSize,
				AFE_DMA_PeripheralInc,
				AFE_DMA_MemoryInc,
				AFE_DMA_PeripheralDataSize,
				AFE_DMA_MemoryDataSize,
				AFE_DMA_Mode,
				AFE_DMA_Priority,
				AFE_DMA_FIFOMode,
				AFE_DMA_FIFOThreshold,
				AFE_DMA_MemoryBurst,
				AFE_DMA_PeripheralBurst
				);
		// USE DMA MODE
		  GPIO_WriteBit(AFE_CS_PORT, AFE_CS_PIN, Bit_RESET);
		  GPIO_WriteBit(AFE_INT_PORT, AFE_INT_PIN, Bit_RESET);
		  /* Enable DMA SPI TX Stream */
		  DMA_Cmd(AFE_DMA_TX_DMAStream,ENABLE);

		  /* Enable DMA SPI RX Stream */
		  DMA_Cmd(AFE_DMA_RX_DMAStream,ENABLE);

		  /* Enable SPI DMA TX Requests */
		  SPI_I2S_DMACmd(AFE_SPI, SPI_I2S_DMAReq_Tx, ENABLE);

		  /* Enable SPI DMA RX Requests */
		  SPI_I2S_DMACmd(AFE_SPI, SPI_I2S_DMAReq_Rx, ENABLE);

		  // clear TX buffer
		  memset((void*)afe_DMA_TX_Buffer, 0, AFE_DMA_BufferSize);



		  /* Enable the SPI peripheral */
		  SPI_Cmd(AFE_SPI, ENABLE);

		  while(DMA_GetFlagStatus(AFE_DMA_RX_DMAStream,AFE_DMA_RX_TransferHalfCompleteFlag)==RESET);


		  printf("Halftime Complete from AFE\r\n");

		  while (DMA_GetFlagStatus(AFE_DMA_TX_DMAStream,AFE_DMA_TX_TransferCompleteFlag)==RESET);
		  while (DMA_GetFlagStatus(AFE_DMA_RX_DMAStream,AFE_DMA_RX_TransferCompleteFlag)==RESET);

		  GPIO_WriteBit(AFE_CS_PORT, AFE_CS_PIN, Bit_SET);
		  GPIO_WriteBit(AFE_INT_PORT, AFE_INT_PIN, Bit_SET);

		  printf("Received data from AFE\r\n");

		  /* Disable DMA SPI TX Stream */
		  DMA_Cmd(AFE_DMA_TX_DMAStream,DISABLE);

		  /* Disable DMA SPI RX Stream */
		  DMA_Cmd(AFE_DMA_RX_DMAStream,DISABLE);

		  /* Disable SPI DMA TX Requsts */
		  SPI_I2S_DMACmd(AFE_SPI, SPI_I2S_DMAReq_Tx, DISABLE);

		  /* Disable SPI DMA RX Requsts */
		  SPI_I2S_DMACmd(AFE_SPI, SPI_I2S_DMAReq_Rx, DISABLE);

		  /* Disable the SPI peripheral */
		  SPI_Cmd(AFE_SPI, DISABLE);

		  printf("RX buffer: \r\n");
		  for(uint32_t i= 0; i<AFE_DMA_BufferSize; i++){
			  if(i!=0){
				  printf(", ");
			  }
			  printf("%04ld:0x%x", i+1, afe_DMA_RX_Buffer[i]);
			  if(i%1000==0){
				  printf("\r\n");
			  }
		  }
		  //fwrite((void*)afe_DMA_RX_Buffer, AFE_DMA_BufferSize, 1, stdout);
		  printf("\r\n");
}
