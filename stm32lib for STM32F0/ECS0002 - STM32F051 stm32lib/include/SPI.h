/*
 * SPI.h
 *
 *  Created on: 9-dec.-2014
 *      Author: Robbe
 */

#ifndef SPI_H_
#define SPI_H_

#include "IO.h"
#include "UART.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_dma.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>

namespace stm32lib {

typedef struct SPI_t{
	IO_t* pin_SCK;
	IO_t* pin_MOSI;
	IO_t* pin_MISO;
	IO_t* pin_SS;

	SPI_TypeDef* SPIx=NULL;
	SPI_InitTypeDef* SPIconfig=NULL;

	DMA_TypeDef* DMAx=NULL;
	DMA_InitTypeDef* DMARXConfig=NULL;
	DMA_Channel_TypeDef* DMARXChannel=NULL;
	DMA_InitTypeDef* DMATXConfig=NULL;
	DMA_Channel_TypeDef* DMATXChannel=NULL;

	volatile uint8_t* ReceiveBuffer=NULL;
	uint32_t ReceiveSize=0;
	volatile uint8_t* TransmitBuffer=NULL;
	uint32_t TransmitSize=0;
	uint32_t TransmitSizeMax=0;

	bool initDone;
} SPI_t;

class SPI {
public:
	SPI(IO*, SPI_t*, SPI_TypeDef*, UART*, UART_t*);
	virtual ~SPI();

	void begin(SPI_t* SPIStruct, IO_t* SCK, IO_t* MOSI, IO_t* MISO, IO_t* SS, SPI_TypeDef* SPIx, DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMARXChannel, DMA_Channel_TypeDef* DMATXChannel, uint8_t* receivebuff, uint32_t receivesize, uint8_t* transmitbuff, uint32_t transmitsize);
	void begin(SPI_t* SPIStruct, IO_t* SCK, IO_t* MOSI, IO_t* MISO, IO_t* SS, SPI_TypeDef* SPIx, DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMARXChannel, DMA_Channel_TypeDef* DMATXChannel, uint8_t* receivebuff, uint32_t receivesize, uint8_t* transmitbuff, uint32_t transmitsize, uint16_t SPI_CPHA, uint16_t SPI_CPOL);
	void begin(SPI_t* SPIStruct);
	void begin(SPI_t* SPIStruct, uint16_t SPI_CPHA, uint16_t SPI_CPOL, uint16_t SPI_BaudRatePrescaler, IO_t* SCK, IO_t* MOSI, IO_t* MISO, IO_t* SS);
	void begin(SPI_t* SPIStruct, uint16_t SPI_CPHA, uint16_t SPI_CPOL, uint16_t SPI_BaudRatePrescaler, IOPin* SCK, IOPin* MOSI, IOPin* MISO, IOPin* SS);
	void beginSimple(SPI_t* SPIStruct, uint16_t SPI_BaudRatePrescaler);
	void setDefaultSPI(SPI_t*);
	void setDefaultUART(UART_t*, UART*);
	void setDefaultIO(IO*);

	uint8_t print(SPI_TypeDef*,uint8_t);
	void print(SPI_TypeDef*,uint8_t*, uint32_t);
	void print(uint8_t*, uint32_t);
	void printnoSS(uint8_t*, uint32_t);
	uint8_t print(uint8_t);
	uint8_t printnoSS(uint8_t);
	void print(volatile uint8_t*, volatile uint8_t*, uint32_t);
	void printreceive(volatile uint8_t*, uint32_t);
	void print();
	void setSS(bool);
private:
	SPI_t* SPIObj = NULL;
	UART_t* UARTStruct = NULL;
	UART* UARTObj = NULL;
	IO* IOObj = NULL;
	void setRCCClock(SPI_TypeDef*, bool);
	void setRCCClock(DMA_TypeDef*, bool);
	uint32_t getChannelNumber(DMA_Channel_TypeDef*);
	uint32_t getTransferCompleteFlag(uint32_t);
	uint32_t getTransferCompleteFlag(DMA_Channel_TypeDef*);

	bool getDefaultSPISet(void);
	bool getDefaultUARTSet(void);
	bool getDefaultIOSet(void);
	bool getPinsInitSet(SPI_t*);
	bool getSPISet(SPI_t*);
	bool getDMASet(SPI_t*);
	bool getDMARXChannelSet(SPI_t*);
	bool getDMATXChannelSet(SPI_t*);
	bool getReceiveBufferSet(SPI_t*);
	bool getTransmitBufferSet(SPI_t*);

	void printOBJName(void);
	void printMessageUART(std::string);
};

} /* namespace stm32lib */

#endif /* SPI_H_ */
