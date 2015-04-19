/*
 * spi.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "spi.h"

void
initialize_SPI(SPI_TypeDef* SPIx,
		uint16_t SPI_Direction,
		uint16_t SPI_Mode,
		uint16_t SPI_DataSize,
		uint16_t SPI_CPOL,
		uint16_t SPI_CPHA,
		uint16_t SPI_NSS,
		uint16_t SPI_BaudRatePrescaler,
		uint16_t SPI_FirstBit,
		uint16_t SPI_CRCPolynomial){
	SPI_InitTypeDef tmp;

	// enable the clock
	rcc_setSPIClock(SPIx, ENABLE);

	// set the spi
	tmp.SPI_Direction = SPI_Direction;
	tmp.SPI_Mode = SPI_Mode;
	tmp.SPI_DataSize = SPI_DataSize;
	tmp.SPI_CPOL = SPI_CPOL;
	tmp.SPI_CPHA = SPI_CPHA;
	tmp.SPI_NSS = SPI_NSS;
	tmp.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
	tmp.SPI_FirstBit = SPI_FirstBit;
	tmp.SPI_CRCPolynomial = SPI_CRCPolynomial;
	SPI_Init(SPIx, &tmp);
	// wait for the flag to clear
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
}
