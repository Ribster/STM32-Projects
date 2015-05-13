/*
 * SPI.cpp
 *
 *  Created on: 9-dec.-2014
 *      Author: Robbe
 */

#include "SPI.h"

namespace stm32lib {

SPI::SPI(IO* i, SPI_t* ss, SPI_TypeDef* spix, UART* u, UART_t* us) {
	setDefaultIO(i);
	setDefaultSPI(ss);
	setDefaultUART(us, u);
	ss->SPIx = spix;
	if(ss->SPIconfig == 0){
		ss->SPIconfig = new SPI_InitTypeDef;
	}
}

SPI::~SPI() {
	// TODO Auto-generated destructor stub
}

uint8_t SPI::print(SPI_TypeDef* SPIStruct, uint8_t ch){
    while (SPI_I2S_GetFlagStatus(SPIStruct, SPI_I2S_FLAG_TXE) == RESET); // Wait while DR register is not empty
    SPI_SendData8(SPIStruct, ch); // Send byte to SPI
    while (SPI_I2S_GetFlagStatus(SPIStruct, SPI_I2S_FLAG_RXNE) == RESET); // Wait to receive byte
    return SPI_ReceiveData8(SPIStruct); // Read byte from SPI bus
}

void SPI::print(SPI_TypeDef* SPIStruct, uint8_t* buffer, uint32_t size){
	uint32_t i;
	for(i=0; i<size; i++){
		print(SPIStruct,buffer[i]);
	}
}

void SPI::printnoSS(uint8_t* buffer, uint32_t size){
	print(SPIObj->SPIx, buffer, size);
}

void SPI::print(uint8_t* buffer, uint32_t size){
	setSS(false);
	print(SPIObj->SPIx, buffer, size);
	setSS(true);
}

uint8_t SPI::printnoSS(uint8_t ch){
	return print(SPIObj->SPIx, ch);
}

uint8_t SPI::print(uint8_t ch){
	uint8_t tmp;
	setSS(false);
	tmp = printnoSS(ch);
	setSS(true);
	return tmp;
}

void SPI::print(void){
	print(SPIObj->TransmitBuffer, SPIObj->ReceiveBuffer, SPIObj->TransmitSize);
}

void SPI::setSS(bool newVal){
	IOObj->setValue(SPIObj->pin_SS, newVal);
}

void SPI::print(volatile uint8_t* transmitbuffer, volatile uint8_t* receivebuffer, uint32_t size){
	uint32_t i;
	setSS(false);
	for(i=0; i<size; i++){
		*(receivebuffer+i) = print(SPIObj->SPIx, *(transmitbuffer+i));
	}
	setSS(true);
}
void SPI::printreceive(volatile uint8_t* receivebuffer, uint32_t size){
	uint32_t i;
	setSS(false);
	for(i=0; i<size; i++){
		*(receivebuffer+i) = print(0xFF);
	}
	setSS(true);
}

void SPI::begin(SPI_t* SPIStruct, IO_t* SCK, IO_t* MOSI, IO_t* MISO, IO_t* SS, SPI_TypeDef* SPIx, DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMARXChannel, DMA_Channel_TypeDef* DMATXChannel, uint8_t* receivebuff, uint32_t receivesize, uint8_t* transmitbuff, uint32_t transmitsize){
	begin(SPIStruct, SCK, MOSI, MISO, SS, SPIx, DMAx, DMARXChannel, DMATXChannel, receivebuff, receivesize, transmitbuff, transmitsize, SPI_CPHA_1Edge, SPI_CPOL_Low);
}

void SPI::begin(SPI_t* SPIStruct, IO_t* SCK, IO_t* MOSI, IO_t* MISO, IO_t* SS, SPI_TypeDef* SPIx, DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMARXChannel, DMA_Channel_TypeDef* DMATXChannel, uint8_t* receivebuff, uint32_t receivesize, uint8_t* transmitbuff, uint32_t transmitsize, uint16_t SPI_CPHA, uint16_t SPI_CPOL){
	SPIStruct->pin_SCK = SCK;
	SPIStruct->pin_MOSI = MOSI;
	SPIStruct->pin_MISO = MISO;
	SPIStruct->pin_SS = SS;

	SPIStruct->SPIx = SPIx;
	SPIStruct->SPIconfig->SPI_CPHA = SPI_CPHA;
	SPIStruct->SPIconfig->SPI_CPOL = SPI_CPOL;

	SPIStruct->DMAx = DMAx;
	SPIStruct->DMARXChannel = DMARXChannel;
	SPIStruct->DMATXChannel = DMATXChannel;

	SPIStruct->ReceiveBuffer = receivebuff;
	SPIStruct->ReceiveSize = receivesize;
	SPIStruct->TransmitBuffer = transmitbuff;
	SPIStruct->TransmitSize = transmitsize;

	begin(SPIStruct);

}

void SPI::begin(SPI_t* SPIStruct){

	// check if default IO object is set
	if( !(getDefaultIOSet()) ){
		printMessageUART("Error on default IO object");
		return;
	}

	// check if pins are initialized
	if(!(getPinsInitSet(SPIStruct)) ){
		printMessageUART("Error on pins init in struct");
		return;
	} else {
		IOObj->setValue(SPIStruct->pin_SS, true);
	}



	// check if SPI is set
	if(!(getSPISet(SPIStruct)) ){
		printMessageUART("Error on spi init in struct");
		return;
	}

	// check if DMA is set
	if(!(getDMASet(SPIStruct)) ){
		printMessageUART("Error on DMA in struct");
		return;
	}

	// check if DMARX channel or DMATX channel is set
	if(!(getDMARXChannelSet(SPIStruct) || getDMATXChannelSet(SPIStruct)) ){
		printMessageUART("Error on RX && TX channel in struct");
		return;
	}

	// check if receive buffer is set
	if(getDMARXChannelSet(SPIStruct)){
		if(!(getReceiveBufferSet(SPIStruct)) ){
			printMessageUART("Error on receive buffer in struct");
			return;
		}
	}

	// check if transmitbuffer is set
	if(getDMATXChannelSet(SPIStruct)){
		if(!(getTransmitBufferSet(SPIStruct)) ){
			printMessageUART("Error on transmit buffer in struct");
			return;
		}
	}

	// enable SPI clock
	setRCCClock(SPIStruct->SPIx, true);
	// enable DMA clock
	setRCCClock(SPIStruct->DMAx, true);









	/*
	// deinit
	SPI_I2S_DeInit(SPIStruct->SPIx);

	// init SPI
	SPI_StructInit(SPIStruct->SPIconfig);
	SPIStruct->SPIconfig->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPIStruct->SPIconfig->SPI_CRCPolynomial = 7;
	SPIStruct->SPIconfig->SPI_DataSize = SPI_DataSize_8b;
	SPIStruct->SPIconfig->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPIStruct->SPIconfig->SPI_FirstBit = SPI_FirstBit_MSB;
	SPIStruct->SPIconfig->SPI_Mode = SPI_Mode_Master;
	SPIStruct->SPIconfig->SPI_NSS = SPI_NSS_Hard;
	SPI_Init(SPIStruct->SPIx, SPIStruct->SPIconfig);

	if(getDMARXChannelSet(SPIStruct)){
		// SPI->memory
		SPI_RxFIFOThresholdConfig(SPIStruct->SPIx, SPI_RxFIFOThreshold_QF);
		DMA_DeInit(SPIStruct->DMARXChannel);
		DMA_StructInit(SPIStruct->DMARXConfig);
		SPIStruct->DMARXConfig->DMA_PeripheralBaseAddr = (uint32_t)(&(SPIStruct->SPIx->DR));
		SPIStruct->DMARXConfig->DMA_MemoryBaseAddr = (uint32_t)SPIStruct->ReceiveBuffer;
		SPIStruct->DMARXConfig->DMA_DIR = DMA_DIR_PeripheralSRC;
		SPIStruct->DMARXConfig->DMA_BufferSize = (uint16_t)SPIStruct->ReceiveSize;
		SPIStruct->DMARXConfig->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		SPIStruct->DMARXConfig->DMA_MemoryInc = DMA_MemoryInc_Enable;
		SPIStruct->DMARXConfig->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		SPIStruct->DMARXConfig->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		SPIStruct->DMARXConfig->DMA_Mode = DMA_Mode_Normal;
		SPIStruct->DMARXConfig->DMA_Priority = DMA_Priority_High;
		SPIStruct->DMARXConfig->DMA_M2M = DMA_M2M_Disable;
		DMA_Init(SPIStruct->DMARXChannel, SPIStruct->DMARXConfig);
		DMA_Cmd(SPIStruct->DMARXChannel, ENABLE);
	}

	if(getDMATXChannelSet(SPIStruct)){
		// memory->SPI
		DMA_DeInit(SPIStruct->DMATXChannel);
		DMA_StructInit(SPIStruct->DMATXConfig);
		SPIStruct->DMATXConfig->DMA_BufferSize = (uint16_t)SPIStruct->TransmitSize;
		SPIStruct->DMATXConfig->DMA_DIR =  DMA_DIR_PeripheralDST;
		SPIStruct->DMATXConfig->DMA_M2M = DMA_M2M_Disable;
		SPIStruct->DMATXConfig->DMA_MemoryBaseAddr = (uint32_t)SPIStruct->TransmitBuffer;
		SPIStruct->DMATXConfig->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		SPIStruct->DMATXConfig->DMA_MemoryInc = DMA_MemoryInc_Enable;
		SPIStruct->DMATXConfig->DMA_Mode = DMA_Mode_Normal;
		SPIStruct->DMATXConfig->DMA_PeripheralBaseAddr = (uint32_t)(&(SPIStruct->SPIx->DR));
		SPIStruct->DMATXConfig->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		SPIStruct->DMATXConfig->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		SPIStruct->DMATXConfig->DMA_Priority = DMA_Priority_High;
		DMA_Init(SPIStruct->DMATXChannel, SPIStruct->DMATXConfig);
		DMA_Cmd(SPIStruct->DMATXChannel, ENABLE);
	}


	// enable SPI
	SPI_Cmd(SPIStruct->SPIx, ENABLE);

	SPI_SSOutputCmd(SPIStruct->SPIx, ENABLE);


	if(getDMARXChannelSet(SPIStruct)){
		// enable SPI dma request
		SPI_I2S_DMACmd(SPIStruct->SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
	}
	if(getDMATXChannelSet(SPIStruct)){
		// enable SPI dma request
		SPI_I2S_DMACmd(SPIStruct->SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
	}

	//IOObj->setValue(SPIStruct->pin_SS, false);


	if(getDMARXChannelSet(SPIStruct)){
		while(DMA_GetFlagStatus(getTransferCompleteFlag(SPIStruct->DMARXChannel)) == RESET);
	}
	if(getDMATXChannelSet(SPIStruct)){
		while(DMA_GetFlagStatus(getTransferCompleteFlag(SPIStruct->DMATXChannel)) == RESET);
	}

	while(SPI_I2S_GetFlagStatus(SPIStruct->SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while(SPI_I2S_GetFlagStatus(SPIStruct->SPIx, SPI_I2S_FLAG_BSY) == SET);


	if(getDMATXChannelSet(SPIStruct)){
		if(SPIStruct->DMATXChannel == DMA1_Channel1){
			DMA_ClearFlag(DMA1_FLAG_GL1);
		} else if(SPIStruct->DMATXChannel == DMA1_Channel2){
			DMA_ClearFlag(DMA1_FLAG_GL2);
		} else if(SPIStruct->DMATXChannel == DMA1_Channel3){
			DMA_ClearFlag(DMA1_FLAG_GL3);
		} else if(SPIStruct->DMATXChannel == DMA1_Channel4){
			DMA_ClearFlag(DMA1_FLAG_GL4);
		} else if(SPIStruct->DMATXChannel == DMA1_Channel5){
			DMA_ClearFlag(DMA1_FLAG_GL5);
		}

	}
	if(getDMARXChannelSet(SPIStruct)){
		if(SPIStruct->DMARXChannel == DMA1_Channel1){
			DMA_ClearFlag(DMA1_FLAG_GL1);
		} else if(SPIStruct->DMARXChannel == DMA1_Channel2){
			DMA_ClearFlag(DMA1_FLAG_GL2);
		} else if(SPIStruct->DMARXChannel == DMA1_Channel3){
			DMA_ClearFlag(DMA1_FLAG_GL3);
		} else if(SPIStruct->DMARXChannel == DMA1_Channel4){
			DMA_ClearFlag(DMA1_FLAG_GL4);
		} else if(SPIStruct->DMARXChannel == DMA1_Channel5){
			DMA_ClearFlag(DMA1_FLAG_GL5);
		}
	}

	if(getDMATXChannelSet(SPIStruct)){
		DMA_Cmd(SPIStruct->DMATXChannel, DISABLE);
	}
	if(getDMARXChannelSet(SPIStruct)){
		DMA_Cmd(SPIStruct->DMARXChannel, DISABLE);
	}

	IOObj->setValue(SPIStruct->pin_SS, false);

	SPI_Cmd(SPIStruct->SPIx, DISABLE);


	if(getDMARXChannelSet(SPIStruct)){
		SPI_I2S_DMACmd(SPIStruct->SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
	}
	if(getDMATXChannelSet(SPIStruct)){
		SPI_I2S_DMACmd(SPIStruct->SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
	}

*/
	SPIStruct->initDone = true;

}

void SPI::beginSimple(SPI_t* SPIStruct, uint16_t SPI_BaudRatePrescaler){

	// check if default IO object is set
	if( !(getDefaultIOSet()) ){
		printMessageUART("Error on default IO object");
		return;
	}

	// check if pins are initialized
	if(!(getPinsInitSet(SPIStruct)) ){
		printMessageUART("Error on pins init in struct");
		return;
	}



	// check if SPI is set
	if(!(getSPISet(SPIStruct)) ){
		printMessageUART("Error on spi init in struct");
		return;
	}


	// enable SPI clock
	setRCCClock(SPIStruct->SPIx, true);


	// deinit
	SPI_I2S_DeInit(SPIStruct->SPIx);

	// init SPI
	SPIStruct->SPIconfig->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
	SPIStruct->SPIconfig->SPI_CRCPolynomial = 7;
	SPIStruct->SPIconfig->SPI_DataSize = SPI_DataSize_8b;
	SPIStruct->SPIconfig->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPIStruct->SPIconfig->SPI_FirstBit = SPI_FirstBit_MSB;
	SPIStruct->SPIconfig->SPI_Mode = SPI_Mode_Master;
	SPIStruct->SPIconfig->SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPIStruct->SPIx, SPIStruct->SPIconfig);

	SPIStruct->TransmitSizeMax = SPIStruct->TransmitSize;
	if(SPIStruct->ReceiveSize > SPIStruct->TransmitSizeMax){
		SPIStruct->TransmitSizeMax = SPIStruct->ReceiveSize;
	}

    SPI_RxFIFOThresholdConfig(SPIStruct->SPIx, SPI_RxFIFOThreshold_QF);
    SPI_SSOutputCmd(SPIStruct->SPIx, DISABLE);
	// enable SPI
	SPI_Cmd(SPIStruct->SPIx, ENABLE);
	IOObj->setValue(SPIStruct->pin_SS, true);
	/*
	IOObj->setValue(SPIStruct->pin_SS, false);
	print(SPIStruct->TransmitBuffer, SPIStruct->ReceiveBuffer, SPIStruct->TransmitSize);
	IOObj->setValue(SPIStruct->pin_SS, true);

	UARTObj->print("[SPI] Transmitted data: ");
	uint8_t i;
	for(i=0;i<SPIStruct->TransmitSize; i++){
		UARTObj->print((int16_t)*(SPIStruct->TransmitBuffer+i));
		if(i< (SPIStruct->TransmitSize-1) ){
			UARTObj->print(", ");
		}
	}
	UARTObj->println("");
	UARTObj->print("[SPI] Received data: ");
	for(i=0;i<SPIStruct->ReceiveSize; i++){
		UARTObj->print((int16_t)*(SPIStruct->ReceiveBuffer+i));
		if(i< (SPIStruct->ReceiveSize-1) ){
			UARTObj->print(", ");
		}
	}
	UARTObj->println("");
	*/
	SPIStruct->initDone = true;
}

void SPI::begin(SPI_t* SPIStruct, uint16_t SPI_CPHA, uint16_t SPI_CPOL, uint16_t SPI_BaudRatePrescaler, IO_t* SCK, IO_t* MOSI, IO_t* MISO, IO_t* SS){
	SPIStruct->pin_SCK = SCK;
	SPIStruct->pin_MOSI = MOSI;
	SPIStruct->pin_MISO = MISO;
	SPIStruct->pin_SS = SS;
	SPIStruct->SPIconfig->SPI_CPHA = SPI_CPHA;
	SPIStruct->SPIconfig->SPI_CPOL = SPI_CPOL;
	beginSimple(SPIStruct, SPI_BaudRatePrescaler);
}

void SPI::begin(SPI_t* SPIStruct, uint16_t SPI_CPHA, uint16_t SPI_CPOL, uint16_t SPI_BaudRatePrescaler, IOPin* SCK, IOPin* MOSI, IOPin* MISO, IOPin* SS){
	SPIStruct->pin_SCK = SCK->PIN;
	SPIStruct->pin_MOSI = MOSI->PIN;
	SPIStruct->pin_MISO = MISO->PIN;
	SPIStruct->pin_SS = SS->PIN;
	SPIStruct->SPIconfig->SPI_CPHA = SPI_CPHA;
	SPIStruct->SPIconfig->SPI_CPOL = SPI_CPOL;
	beginSimple(SPIStruct, SPI_BaudRatePrescaler);
}

uint32_t SPI::getChannelNumber(DMA_Channel_TypeDef* channelx){
	if(channelx == DMA1_Channel1){
		return 1;
	} else if(channelx == DMA1_Channel2){
		return 2;
	} else if(channelx == DMA1_Channel3){
		return 3;
	} else if(channelx == DMA1_Channel4){
		return 4;
	} else if(channelx == DMA1_Channel5){
		return 5;
	}

	return 0;
}


uint32_t SPI::getTransferCompleteFlag(uint32_t channelx){
	if(channelx == 1){
		return (DMA1_FLAG_TC1);
	} else if (channelx == 2){
		return (DMA1_FLAG_TC2);
	} else if (channelx == 3){
		return (DMA1_FLAG_TC3);
	} else if (channelx == 4){
		return (DMA1_FLAG_TC4);
	} else if (channelx == 5){
		return (DMA1_FLAG_TC5);
	}
	return 0;
}
uint32_t SPI::getTransferCompleteFlag(DMA_Channel_TypeDef* channelx){
	return getTransferCompleteFlag(getChannelNumber(channelx));
}

void SPI::setRCCClock(SPI_TypeDef* SPIx, bool newVal){
	FunctionalState TempNewVal = (newVal)?(ENABLE):(DISABLE);
	if(SPIx == SPI1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, TempNewVal);
	} else if (SPIx == SPI2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, TempNewVal);
	}
}

void SPI::setRCCClock(DMA_TypeDef* DMAx, bool newVal){
	FunctionalState TempNewVal = (newVal)?(ENABLE):(DISABLE);
	if(DMAx == DMA1){
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, TempNewVal);
	}
}

void SPI::setDefaultSPI(SPI_t* SPIStruct){
	SPIObj = SPIStruct;
}
void SPI::setDefaultUART(UART_t* UARTStr, UART* UARTO){
	UARTStruct = UARTStr;
	UARTObj = UARTO;
}

void SPI::setDefaultIO(IO* IOO){
	IOObj = IOO;
}

bool SPI::getDefaultSPISet(void){
	return !(SPIObj == NULL);
}
bool SPI::getDefaultUARTSet(void){
	return !(UARTObj == NULL && UARTStruct == NULL);
}

bool SPI::getDefaultIOSet(void){
	return !(IOObj == NULL);
}

bool SPI::getPinsInitSet(SPI_t* SPIStruct){
	return (SPIStruct->pin_MISO->init && SPIStruct->pin_MOSI->init && SPIStruct->pin_SCK->init && SPIStruct->pin_SS->init);
}
bool SPI::getSPISet(SPI_t* SPIStruct){
	return !(SPIStruct->SPIx == NULL);
}
bool SPI::getDMASet(SPI_t* SPIStruct){
	return !(SPIStruct->DMAx == NULL);
}

bool SPI::getDMARXChannelSet(SPI_t* SPIStruct){
	return !(SPIStruct->DMARXChannel == NULL);
}
bool SPI::getDMATXChannelSet(SPI_t* SPIStruct){
	return !(SPIStruct->DMATXChannel == NULL);
}
bool SPI::getReceiveBufferSet(SPI_t* SPIStruct){
	return !(SPIStruct->ReceiveBuffer == NULL);
}
bool SPI::getTransmitBufferSet(SPI_t* SPIStruct){
	return !(SPIStruct->TransmitBuffer == NULL);
}

void SPI::printOBJName(void){
	if(getDefaultUARTSet()){
		printf("[SPI] ");
	}
}

void SPI::printMessageUART(std::string sendString){
	if(getDefaultUARTSet()){
		printOBJName();
		printf("%s \r\n",sendString.c_str());
	}
}

} /* namespace stm32lib */
