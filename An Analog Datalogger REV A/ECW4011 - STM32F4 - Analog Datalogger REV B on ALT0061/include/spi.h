/*
 * spi.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"

#include <stdio.h>

#include "gpio.h"
#include "rcc.h"

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
		uint16_t SPI_CRCPolynomial);

#endif /* SPI_H_ */
