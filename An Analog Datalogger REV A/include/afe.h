/*
 * afe.h
 *
 *  Created on: 21-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef AFE_H_
#define AFE_H_

#include "projectconfig.h"

#include "gpio.h"
#include "rcc.h"
#include "dma.h"
#include "nvic.h"
#include "delay.h"

#include "string.h"

#include <stdio.h>

// prototypes
void
initialize_AFE(void);

#endif /* AFE_H_ */
